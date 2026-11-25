# Binary Persistence Format — Version 1

**Project:** Personal Finance Manager
**Owner:** TV1 — Core & Integration
**Sprint:** Sprint 3 — Persistence & Recurring Transactions
**Status:** Design specification; implementation and verification pending.

## 1. Overview

The application uses binary files to persist financial data. Fixed-size records store numeric fields and references to variable-length strings stored in separate pool files.

The format is designed to:

- Support consistent serialization across macOS and Windows.
- Preserve IDs, balances, references, and object states.
- Avoid compiler-dependent memory layouts.
- Support future random-access record operations.
- Provide shared serialization contracts for all modules.

Sprint 3 focuses on basic Binary Persistence. Full application persistence, random-access updates, and multi-file consistency protection will be completed in later sprints.

## 2. General Serialization Rules

### 2.1. Data Types

| Data | On-disk type | Size (bytes) |
|---|---|---:|
| Object ID | `int32_t` | 4 |
| Monetary value | `int64_t` | 8 |
| String offset | `uint32_t` | 4 |
| String length | `uint32_t` | 4 |
| Record count | `uint32_t` | 4 |
| Record size | `uint32_t` | 4 |
| Format version | `uint32_t` | 4 |
| Boolean flag | `uint8_t` | 1 |
| Transaction type | `uint8_t` | 1 |
| Date component | `int32_t` | 4 |
| Date | Three `int32_t` values | 12 |

### 2.2. Encoding Rules

- All multi-byte integers use little-endian byte order.
- Signed integers use two's complement representation.
- Monetary values represent whole VND amounts.
- Boolean flags use `0` for false and `1` for true.
- Transaction types use `0` for Income and `1` for Expense.
- Dates are serialized in day, month, year order.
- No compiler-generated padding is stored.
- All files are opened in binary mode.
- Variable-length strings use UTF-8 encoding.

### 2.3. In-memory and On-disk Representations

In-memory record structures and on-disk record layouts are independent.

Existing C++ record structures may contain compiler-generated padding. Their memory sizes must not be used to determine serialized record sizes.

Records must be serialized and deserialized field by field according to this specification.

Do not write an entire record using:

```cpp
file.write(reinterpret_cast<const char*>(&record), sizeof(record));
```

Existing model classes and record structures do not need to be reorganized for serialization.

## 3. File Organization

### 3.1. Record Files

| File | Magic | Record size (bytes) |
|---|---|---:|
| `wallets.bin` | `WALT` | 29 |
| `sources.bin` | `SRCE` | 13 |
| `categories.bin` | `CATG` | 13 |
| `transactions.bin` | `TRAN` | 41 |
| `recurring.bin` | `RECR` | 72 |

### 3.2. String Pool Files

| Record file | Associated pool file |
|---|---|
| `wallets.bin` | `walletNames.bin` |
| `sources.bin` | `sourceNames.bin` |
| `categories.bin` | `categoryNames.bin` |
| `transactions.bin` | `transactionDescriptions.bin` |
| `recurring.bin` | `recurringDescriptions.bin` |

All data files use relative paths under the `data/` directory.

Generated binary data files must not be committed to Git.

**Pending coordination with TV3:** The two Description Pool files are proposed extensions to the existing file structure. Update `FILE_STRUCTURE.md` after confirmation.

## 4. Binary File Header

Every record file begins with a fixed-size 16-byte header.

| Field | Type | Byte offset | Size |
|---|---|---:|---:|
| Magic | `char[4]` | 0 | 4 |
| Version | `uint32_t` | 4 | 4 |
| Record size | `uint32_t` | 8 | 4 |
| Record count | `uint32_t` | 12 | 4 |
| **Total** | | | **16** |

### Header Rules

- Magic contains exactly four ASCII bytes without a null terminator.
- Version must be `1`.
- Record size must match the expected on-disk record layout.
- Record count includes all stored records, including soft-deleted Master Data records.
- An empty dataset has a valid header with record count equal to zero.

### Record Position

The byte position of a record is calculated using:

```text
recordPosition = headerSize + index * recordSize
```

The record index is zero-based and is independent of the object ID.

### Expected File Size

```text
expectedFileSize = 16 + recordCount * recordSize
```

Use sufficiently wide integer types when calculating file sizes and positions.

Version 1 requires an exact match between the expected file size and the actual file size.

## 5. Master Data Record Layouts

All offsets in the following tables are relative to the beginning of an individual record.

### 5.1. Wallet Record

**Magic:** `WALT`
**Record size:** 29 bytes

| Field | Type | Offset | Size |
|---|---|---:|---:|
| `id` | `int32_t` | 0 | 4 |
| `nameOffset` | `uint32_t` | 4 | 4 |
| `nameLength` | `uint32_t` | 8 | 4 |
| `initialBalance` | `int64_t` | 12 | 8 |
| `balance` | `int64_t` | 20 | 8 |
| `isDeleted` | `uint8_t` | 28 | 1 |
| **Total** | | | **29** |

Both initial and current balances must be preserved.

Wallet balance restoration must not replay existing transactions.

### 5.2. Income Source Record

**Magic:** `SRCE`
**Record size:** 13 bytes

| Field | Type | Offset | Size |
|---|---|---:|---:|
| `id` | `int32_t` | 0 | 4 |
| `nameOffset` | `uint32_t` | 4 | 4 |
| `nameLength` | `uint32_t` | 8 | 4 |
| `isDeleted` | `uint8_t` | 12 | 1 |
| **Total** | | | **13** |

### 5.3. Expense Category Record

**Magic:** `CATG`
**Record size:** 13 bytes

| Field | Type | Offset | Size |
|---|---|---:|---:|
| `id` | `int32_t` | 0 | 4 |
| `nameOffset` | `uint32_t` | 4 | 4 |
| `nameLength` | `uint32_t` | 8 | 4 |
| `isDeleted` | `uint8_t` | 12 | 1 |
| **Total** | | | **13** |

### 5.4. Master Data Validation

- IDs must be positive and unique within each dataset.
- Names must not be empty.
- Active names must be unique within their corresponding dataset.
- Deleted records must be preserved.
- Deletion flags must be either `0` or `1`.
- Wallet balances must satisfy the supported balance rules.
- Aggregate wallet balance calculations must not overflow.

Model validation methods that reject inactive objects must not be used directly to reject valid soft-deleted records during loading.

## 6. Transaction Record Layout

**Magic:** `TRAN`
**Record size:** 41 bytes

Income and Expense share the same on-disk record layout.

| Field | Type | Offset | Size |
|---|---|---:|---:|
| `id` | `int32_t` | 0 | 4 |
| `type` | `uint8_t` | 4 | 1 |
| `day` | `int32_t` | 5 | 4 |
| `month` | `int32_t` | 9 | 4 |
| `year` | `int32_t` | 13 | 4 |
| `amount` | `int64_t` | 17 | 8 |
| `walletId` | `int32_t` | 25 | 4 |
| `sourceOrCategoryId` | `int32_t` | 29 | 4 |
| `descriptionOffset` | `uint32_t` | 33 | 4 |
| `descriptionLength` | `uint32_t` | 37 | 4 |
| **Total** | | | **41** |

### Transaction Types

| Type | Restored model | Reference |
|---:|---|---|
| 0 | Income | Income Source ID |
| 1 | Expense | Expense Category ID |

### Validation and Restoration

- Transaction IDs must be positive and unique.
- Transaction types must be valid.
- Amounts must be greater than zero.
- Transaction dates must be valid.
- Referenced Wallet and Source/Category IDs must exist.
- Historical references to soft-deleted Master Data records must be supported.
- Description references must remain within pool boundaries.

When loading transactions, reconstruct the corresponding Income or Expense objects while preserving their original IDs.

Do not use the existing `addIncome()` or `addExpense()` operations for restoration because these operations update wallet balances and assign new transaction IDs.

Transaction loading must manage temporary allocated objects safely and release them if loading fails.

## 7. Recurring Transaction Record Layout

**Magic:** `RECR`
**Record size:** 72 bytes

| Field | Type | Offset | Size |
|---|---|---:|---:|
| `id` | `int32_t` | 0 | 4 |
| `type` | `uint8_t` | 4 | 1 |
| `amount` | `int64_t` | 5 | 8 |
| `walletId` | `int32_t` | 13 | 4 |
| `sourceOrCategoryId` | `int32_t` | 17 | 4 |
| `descriptionOffset` | `uint32_t` | 21 | 4 |
| `descriptionLength` | `uint32_t` | 25 | 4 |
| `startDate` | Date | 29 | 12 |
| `hasEndDate` | `uint8_t` | 41 | 1 |
| `endDate` | Date | 42 | 12 |
| `dayOfMonth` | `int32_t` | 54 | 4 |
| `hasLastGeneratedDate` | `uint8_t` | 58 | 1 |
| `lastGeneratedDate` | Date | 59 | 12 |
| `isActive` | `uint8_t` | 71 | 1 |
| **Total** | | | **72** |

### 7.1. Optional Dates

Optional dates always occupy 12 bytes to preserve the fixed record size.

| Flag | Serialized date |
|---|---|
| 0 | `(0, 0, 0)` |
| 1 | Actual date |

During loading:

- Reject flags other than `0` or `1`.
- When the flag is `0`, verify the zero placeholder and keep the optional date unset.
- Do not treat the zero placeholder as a valid application Date.
- When the flag is `1`, validate the date before restoring it.
- Restore optional fields into newly constructed objects.

### 7.2. Recurring Record Validation

- IDs must be positive.
- Recurring IDs must be unique.
- Transaction types and boolean flags must be valid.
- Amounts must be greater than zero.
- Day of month must be between 1 and 31.
- Start Date must be valid.
- End Date must be valid and must not precede Start Date when present.
- Last Generated Date must be valid when present.
- For an unchanged schedule, Last Generated Date must not precede Start Date or exceed End Date when present.
- For an unchanged schedule, Last Generated Date must match the monthly generation schedule, including shorter months.
- Referenced IDs and Description Pool boundaries must be validated.

If recurring schedule editing is supported, generation history must be preserved according to the agreed editing rules. Do not automatically reset Last Generated Date.

### 7.3. Generation State

Both Last Generated Date and its presence flag must be persisted.

Recurring generation must create actual transactions through `TransactionManager`.

- Use existing Income and Expense creation operations.
- Do not separately deposit or withdraw the recurring amount.
- Update generation state only after successful transaction creation.
- Prevent new generation through inactive or deleted Master Data references.

Restoring recurring state does not automatically generate transactions or modify wallet balances.

Cross-session duplicate prevention requires consistent Transaction and Recurring snapshots and will be further addressed in Sprint 4.

## 8. Name Pool and Description Pool

### 8.1. Pool Layout

A pool file contains consecutive string bytes.

- UTF-8 encoding.
- No file header.
- No separators.
- No null terminators.
- No padding.
- Zero-based byte offsets.
- Lengths measured in bytes, not characters.
- Maximum pool size: `UINT32_MAX` bytes.

### 8.2. Example

```text
walletNames.bin:

CashBankSavings
```

| Name | Offset | Length |
|---|---:|---:|
| Cash | 0 | 4 |
| Bank | 4 | 4 |
| Savings | 8 | 7 |

The record stores the offset and length required to retrieve its corresponding string.

### 8.3. Pool Reference Validation

Before reading a string, verify:

```cpp
if (offset > poolSize) {
    return false;
}

if (length > poolSize - offset) {
    return false;
}
```

Avoid unchecked addition of offset and length.

Verify stream status after seeking, reading, and writing. Validate string lengths before memory allocation and before conversion to `uint32_t`.

Master Data names must not be empty.

Transaction and Recurring descriptions may be empty.

### 8.4. Save Strategy

Sprint 3 uses a full rewrite strategy.

- Add operations modify RAM only.
- Rename operations modify RAM only.
- Delete operations modify RAM only.
- Saving rewrites the record file and its associated pool file.
- Offsets and lengths are calculated during serialization.
- Soft-deleted records and their names remain stored.
- Loading reads files without modifying them.

Models do not need to maintain persistent string offsets while operating in RAM.

String deduplication, append-only pools, and random-access updates are not required in Sprint 3.

## 9. ID Restoration

For datasets that retain all previously assigned IDs:

```text
nextId = maxStoredId + 1
```

Rules:

- An empty dataset starts with next ID equal to 1.
- Include soft-deleted records when determining the maximum ID.
- Reject duplicate or non-positive IDs.
- Do not derive next ID from record count.
- Do not reuse IDs belonging to soft-deleted records.
- Check integer limits before incrementing or assigning IDs.

For the supported 32-bit application ID type, `INT32_MAX` is reserved as the exhausted-ID state. The largest assignable ID is `INT32_MAX - 1`.

Add operations must fail safely when no further ID can be assigned.

****Pending coordination with TV3:** The two Description Pool files are proposed extensions reflected in `FILE_STRUCTURE.md`. Their implementation is subject to confirmation with TV3.
## 10. File Validation and Error Handling

### 10.1. File-Level Validation

Before loading records:

- Verify that the required files can be opened.
- Validate magic, version, and record size.
- Validate record count against supported storage capacity.
- Check the exact expected file size.
- Validate pool size and string references.
- Check stream state after every required I/O operation.
- Reject truncated files and unexpected trailing record bytes.

### 10.2. Missing and Empty Files

| Condition | Result |
|---|---|
| Both record and pool files missing | New dataset only if explicitly handled by the application coordinator |
| Only one required file missing | Failed load |
| Zero-byte record file | Failed load |
| Valid header with zero records | Valid empty dataset |
| Empty pool with no non-empty string references | Valid |
| Invalid header or corrupted record | Failed load |
| File access or I/O failure | Failed load |

A module load operation must not silently clear existing data when files are missing.

### 10.3. Safe Loading

Each Manager must follow this sequence:

1. Open and validate the required files.
2. Read records and strings into temporary storage.
3. Validate records, IDs, references, and business rules.
4. Calculate the restored next ID.
5. Replace existing module data only after successful validation.

If any step fails, release temporary resources and preserve the existing module state, including its next ID.

This provides all-or-nothing loading at the individual module level.

### 10.4. Validation Responsibilities

| Layer | Responsibility | Owner |
|---|---|---|
| Binary I/O | File headers, byte encoding, file sizes, stream state, pool boundaries | TV1 |
| Record validation | IDs, flags, amounts, dates, names, optional fields | TV2 / TV3 |
| Manager validation | Duplicate IDs, references, balance rules, next ID | TV2 / TV3 |

## 11. Shared Binary I/O Contracts

Shared Binary I/O utilities provide:

- Reading and writing fixed-width signed and unsigned integers.
- Reading and writing file headers.
- File size and header validation.
- Reading and writing strings through pool references.
- Safe offset and length validation.

Utilities must not own application datasets or contain module-specific business logic.

Managers remain responsible for serializing individual record fields, reconstructing model objects, and managing application state.

Operations that encounter invalid data or I/O failures must report failure. Failed read operations must not overwrite their output values.

Exact C++ utility interfaces will be defined during implementation.

## 12. Module Integration

### 12.1. Loading Order

```text
Master Data
    |
    v
Transactions
    |
    v
Recurring State
    |
    v
Application Ready
```

Master Data must be available before restoring Transaction and Recurring references.

Wallet balances are restored from Wallet records. Transaction loading must not replay financial operations.

### 12.2. Recurring Generation Dependency

```text
RecurringTransactionManager
    |
    v
TransactionManager
    |
    +-- WalletManager
    +-- IncomeSourceManager
    +-- ExpenseCategoryManager
```

RecurringTransactionManager is responsible for schedule evaluation and generation state.

TransactionManager is responsible for actual Income/Expense creation and balance updates.

The exact generation method signature must be agreed upon by TV1 and TV3 before integration.

### 12.3. Statistics

Statistics modules access restored data through existing read-only interfaces.

Loading and statistics calculations must not modify financial values outside their defined responsibilities.

## 13. Verification Requirements

- Verify exact on-disk record sizes.
- Verify little-endian integer encoding using known byte sequences.
- Test valid and invalid file headers.
- Test empty datasets and string pools.
- Test truncated files and unexpected trailing bytes.
- Test invalid offsets and lengths.
- Test save/load round-trips.
- Verify ID restoration and soft-deleted records.
- Verify wallet balances before and after restoration.
- Verify failed loads preserve existing module state.
- Run relevant module and integration regression tests.

Transaction and Recurring Persistence verification applies to the portions implemented during Sprint 3. Remaining work must be documented.

## 14. Current Limitations

The following features are outside the basic Sprint 3 persistence scope:

- Atomic saving across multiple files.
- Application-wide atomic loading.
- Checksums and comprehensive corruption detection.
- Detection of all mismatched record and pool file pairs.
- Random-access record updates.
- Name Pool compaction and string deduplication.
- Full cross-session recurring duplicate prevention.
- Complete Transaction and Recurring Persistence.

A successful individual module load does not guarantee consistency across all application data files.

## 15. References

- `CODING_RULES.md`
- `FILE_STRUCTURE.md`
- `WEEKLY_PLAN.md`
- `docs/plans/plan_for_sprint_3.md`
