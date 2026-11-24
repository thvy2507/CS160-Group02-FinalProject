# Weekly Report — Week 07

### General Information

- **Group ID**: Group02
- **Project Name**: Personal Finance Manager
- **Course:** CS160-26A01
- **Date Range:** 14/11/2026 – 20/11/2026
- **GitHub Repository:** https://github.com/thvy2507/CS160-Group02-FinalProject

---

## Team Members

| Member | Name               | Responsibility                   |
| ------ | ------------------ | -------------------------------- |
| TV1    | Trần Hồng Vy       | Team Leader / Core & Integration |
| TV2    | Lê Thiện Khôi      | Master Data & Wallets            |
| TV3    | Dương Thị Nhã Uyên | Transactions & Recurring         |
| TV4    | Trần Nhật Tân      | Statistics & Dashboard           |

---

## Tasks Completed This Week

> This reporting period focused on **Sprint 2 — Core Management**. The team implemented in-memory master data management, income and expense transactions, automatic wallet balance updates, and date-range statistics. All completed modules were integrated through `FinanceManager`, tested, and merged into `main`.

### 1. Core Development & Integration — TV1

- Implemented reusable input validation helpers for integers, monetary values, text, and dates.
- Completed CLI menu navigation for the main financial management modules.
- Integrated Wallet, Income Source, and Expense Category CRUD operations into `FinanceManager`.
- Integrated income and expense creation and viewing through `TransactionManager`.
- Connected date-range financial summaries to the application's existing transaction data.
- Updated `CMakeLists.txt` to compile and link all required Master Data and Transaction implementations.
- Restored the shared `DynamicArray<T>` implementation and resolved compatibility issues between modules.
- Added transaction tests covering validation, ID assignment, wallet balance updates, and rejected operations.
- Added overflow protection for wallet creation, income deposits, and total active wallet balances.
- Implemented `tests/test_cli.cpp` with 33 automated integration checks.
- Reviewed and standardized coding conventions across the integrated modules.
- Configured branch protection for `main` to enforce the team's Pull Request workflow.
- Conducted manual CLI testing and full regression testing on macOS.
- Completed and merged [PR #17 — Sprint 2 Core Integration](https://github.com/thvy2507/CS160-Group02-FinalProject/pull/17).

### 2. Master Data & Wallet Management — TV2

- Implemented the `Wallet`, `IncomeSource`, and `ExpenseCategory` models.
- Implemented the corresponding manager classes using in-memory data storage.
- Completed Add, View, Edit, and Delete operations for all three Master Data groups.
- Implemented stable ID assignment, ID-based lookup, duplicate-name validation, and soft deletion.
- Implemented wallet balance operations, including deposits, withdrawals, balance adjustments, and total balance calculations.
- Provided lookup and balance management interfaces required by `TransactionManager`.
- Prepared Master Data tests for Wallet and Expense Category management.
- Integrated the module through [PR #15 — Master Data Implementation](https://github.com/thvy2507/CS160-Group02-FinalProject/pull/15).
- Compatibility corrections and coding convention improvements were subsequently completed by TV1 through [PR #16 — Master Data Compatibility Fix](https://github.com/thvy2507/CS160-Group02-FinalProject/pull/16).

### 3. Income & Expense Transactions — TV3

- Implemented the `Transaction` base class and the derived `Income` and `Expense` classes.
- Implemented transaction constructors, getters, setters, virtual destructors, and transaction type identification.
- Implemented in-memory income and expense creation through `TransactionManager`.
- Implemented transaction ID assignment, transaction lookup, and transaction viewing.
- Managed dynamically allocated transactions using `DynamicArray<Transaction*>`, including memory cleanup in the manager destructor.
- Validated transaction amounts, dates, wallet IDs, income source IDs, and expense category IDs.
- Connected transaction processing to wallet balance operations.
- Implemented rollback when a wallet balance operation fails, preventing rejected transactions from remaining in the transaction list.
- Integrated the module with the shared Core and Master Data implementations.
- Completed module integration through [PR #13 — Transaction Implementation](https://github.com/thvy2507/CS160-Group02-FinalProject/pull/13), including additional tests and coding convention corrections by TV1.

### 4. Date-Range Statistics & Navigation — TV4

- Implemented date-range financial summaries in `StatisticsManager`.
- Calculated total income, total expenses, and net balance using actual transaction records.
- Used read-only access to `TransactionManager` without modifying transaction data or wallet balances.
- Implemented inclusive start-date and end-date filtering.
- Handled empty transaction lists, unmatched date ranges, invalid ranges, and arithmetic overflow during summary calculations.
- Implemented interactive date-range reporting through `StatisticsMenu`.
- Prepared eight statistics test cases covering financial totals, date boundaries, invalid inputs, and data preservation.
- Documented Statistics integration requirements and proposed Dashboard navigation rules.
- Integrated the module through [PR #14 — Sprint 2 Date-Range Statistics](https://github.com/thvy2507/CS160-Group02-FinalProject/pull/14), with date input standardization and build integration completed by TV1.

### 5. Shared Architecture & Integration

- Retained the shared `DynamicArray<T>` and `Date` utilities implemented during Sprint 1.
- Used signed 64-bit integer monetary values representing whole VND amounts.
- Maintained stable object IDs that are independent of array indices.
- Standardized date input to `DD/MM/YYYY` across financial transactions and statistics.
- Centralized wallet balance updates within the responsible models and managers to prevent duplicate updates.
- Connected all implemented modules to a shared application state managed by `FinanceManager`.
- Standardized method organization according to logical responsibilities and operation order.
- Preserved shared feature branches and synchronized module implementations with the latest `main`.
- Reviewed and integrated the completed Sprint 2 changes through PRs #13, #14, #15, #16, and #17.

### 6. Testing & Verification

The integrated application was successfully compiled and tested on macOS.

| Test Suite              | Result     |
| ----------------------- | ---------- |
| CMake Application Build | PASS       |
| Core Tests              | PASS       |
| Master Data Tests       | PASS       |
| Transaction Tests       | 10/10 PASS |
| Statistics Tests        | 8/8 PASS   |
| CLI Integration Tests   | 33/33 PASS |

Manual CLI testing also verified the following operations:

- 18 Master Data CRUD and validation checks.
- Income and expense creation and viewing.
- Automatic wallet balance updates.
- Rejection of expenses exceeding the available balance.
- Correct separation of income and expense lists.
- Date-range statistics and menu navigation.
- Recovery from invalid user input.

### 7. Sprint 2 Demo Checkpoint

The following end-to-end workflow was successfully verified:

1. Created a wallet named `Cash` with an initial balance of 1,000,000 VND.
2. Created an income source named `Salary`.
3. Created an expense category named `Food`.
4. Added an income transaction of 500,000 VND.
5. Added an expense transaction of 200,000 VND.
6. Rejected an expense exceeding the available wallet balance.
7. Generated a financial summary for November 2026.
8. Verified that statistics did not modify the wallet balance.

| Financial Item       | Expected Result | Actual Result |
| -------------------- | --------------: | ------------: |
| Initial Balance      |   1,000,000 VND | 1,000,000 VND |
| Total Income         |     500,000 VND |   500,000 VND |
| Total Expense        |     200,000 VND |   200,000 VND |
| Net Balance          |     300,000 VND |   300,000 VND |
| Final Wallet Balance |   1,300,000 VND | 1,300,000 VND |

**Implementation Status:** The primary Sprint 2 functionality has been implemented, integrated, tested, and merged into `main`. All five regression test suites passed on macOS before the final Core PR merge. Application data is currently stored in memory. Transaction editing and deletion, recurring transaction generation, binary persistence, full Dashboard rendering, and additional statistics reports remain scheduled for later sprints. Final merged-branch verification on Windows and dedicated memory sanitizer testing have not yet been confirmed.

---

## AI Usage Declaration

The team used AI tools during Sprint 2 to support implementation, testing, debugging, and documentation.

- **TV1 — Trần Hồng Vy:** ChatGPT (OpenAI) assisted with CLI implementation, module integration, code review, regression testing, Git troubleshooting, and documentation.
- **TV2 — Lê Thiện Khôi:** Gemini (Google) assisted with Master Data CRUD implementation, ID management, wallet balance validation, and test development.
- **TV3 — Dương Thị Nhã Uyên:** Claude (Anthropic) assisted with transaction implementation, code explanation, validation, test preparation, and Git troubleshooting.
- **TV4 — Trần Nhật Tân:** ChatGPT (OpenAI) assisted with date-range statistics, statistics tests, menu implementation, and Git/GitHub guidance.

Team members reviewed and applied AI-assisted suggestions in their own repositories. Detailed AI usage, prompt logs, and supporting evidence are provided in the separate AI Usage Declaration.

---

## Tasks Planned for Next Week

The team plans to begin **Sprint 3 — Persistence & Recurring Transactions**, following `WEEKLY_PLAN.md`.

**TV1 — Trần Hồng Vy (Core & Integration)**

- Design and implement shared binary file I/O utilities.
- Standardize fixed-size record formats and Name Pool offset and length conventions.
- Coordinate initial save/load operations across all modules.
- Refactor duplicated input validation logic where appropriate.
- Continue reviewing shared interfaces, coding conventions, and integration workflows.
- Prepare persistence and integration tests.

**TV2 — Lê Thiện Khôi (Master Data & Wallets)**

- Implement basic binary persistence for Wallet, Income Source, and Expense Category records.
- Implement the corresponding Name Pool storage mechanisms.
- Preserve stable IDs, soft-delete flags, names, and wallet balances across application sessions.
- Test saving and loading Master Data using sample datasets.
- Extend balance validation and overflow protection for additional wallet operations.
- Maintain consistency with the shared data format.

**TV3 — Dương Thị Nhã Uyên (Transactions & Recurring)**

- Implement monthly recurring transaction generation.
- Handle start dates, optional end dates, and active status.
- Prevent duplicate recurring transactions using generation tracking.
- Integrate recurring transactions with existing wallet balance operations.
- Begin implementing binary persistence for transactions and recurring transactions.
- Prepare test cases for recurring schedules, invalid inputs, and duplicate prevention.

**TV4 — Trần Nhật Tân (Statistics & Dashboard)**

- Implement wallet-based financial summaries within a selected date range.
- Validate wallet IDs and filter transactions by wallet.
- Prepare test cases involving multiple wallets and date ranges.
- Maintain read-only access to transaction and wallet data.
- Continue coordinating Statistics and Dashboard integration with TV1.

---

## Issues

### Issue 1: Cross-Module Compatibility and Integration

Differences in shared data structures, module interfaces, CMake dependencies, and coding conventions caused integration difficulties.

**How Addressed:**

- Restored the shared `DynamicArray<T>` implementation.
- Aligned module interfaces, coding conventions, and date input formats.
- Updated CMake to include all required source files.
- Verified module compatibility through compilation and integration testing.

### Issue 2: Git Workflow and Branch Management

An early Pull Request merge introduced changes into `main` before the integration review was completed.

**How Addressed:**

- Configured branch protection rules for `main`.
- Reinforced the Pull Request review and merge workflow.
- Synchronized feature branches and reviewed subsequent changes before merging.

### Issue 3: Data Integrity and Runtime Verification

Invalid transactions, insufficient wallet balances, and potential integer overflow required additional validation and testing.

**How Addressed:**

- Strengthened transaction validation and rollback mechanisms.
- Added wallet balance overflow protection.
- Expanded transaction and CLI integration tests.
- Successfully executed all five regression test suites on macOS.
