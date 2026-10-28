# **Directory & File Structure**

> **Golden Rule:** Each main class should have one pair of `.h` + `.cpp` files, placed in the appropriate directory according to its functionality.

## Complete Directory Tree

```text
CS160-Group02-FinalProject/
│
├── CMakeLists.txt                             ← TV1 build configuration (macOS/Windows)
├── README.md
├── ROLES.md
├── WEEKLY_PLAN.md
├── FILE_STRUCTURE.md
├── CODING_RULES.md
├── .gitignore
│
├── docs/                                      ← TV1 maintains
│   ├── plans/
│   │   ├── plan_for_week_1.md
│   │   ├── plan_for_week_2.md
│   │   ├── plan_for_week_3.md
│   │   ├── plan_for_week_4.md
│   │   └── plan_for_week_5.md
│
├── data/
│   ├── .gitkeep
│   ├── wallets.bin                            ← TV2: Fixed-size wallet records
│   ├── walletNames.bin                        ← TV2: Wallet Name Pool
│   ├── sources.bin                            ← TV2: Income source records
│   ├── sourceNames.bin                        ← TV2: Income Source Name Pool
│   ├── categories.bin                         ← TV2: Expense category records
│   ├── categoryNames.bin                      ← TV2: Expense Category Name Pool
│   ├── transactions.bin                       ← TV3: Transaction records
│   └── recurring.bin                          ← TV3: Recurring transaction records
│
├── tests/                                     ← Each member tests their own module
│   ├── test_core.cpp                          ← TV1
│   ├── test_master_data.cpp                   ← TV2
│   ├── test_transactions.cpp                  ← TV3
│   └── test_statistics.cpp                    ← TV4
│
├── include/                                   ← All .h header files
│   │
│   ├── core/                                  ← TV1: Core & Integration
│   │   ├── DynamicArray.h                     ← Custom Dynamic Array (template)
│   │   ├── Date.h                             ← Date handling and validation
│   │   ├── FinanceManager.h                   ← Main application controller
│   │   └── PersistenceManager.h               ← Binary save/load management
│   │
│   ├── models/                                ← Data models
│   │   ├── Wallet.h                           ← TV2: Wallet information
│   │   ├── IncomeSource.h                     ← TV2: Income source
│   │   ├── ExpenseCategory.h                  ← TV2: Expense category
│   │   ├── Transaction.h                      ← TV3: Base transaction model
│   │   ├── Income.h                           ← TV3: Income transaction
│   │   ├── Expense.h                          ← TV3: Expense transaction
│   │   └── RecurringTransaction.h             ← TV3: Recurring transaction
│   │
│   ├── managers/                              ← Data management
│   │   ├── WalletManager.h                    ← TV2: Wallet and balance management
│   │   ├── IncomeSourceManager.h              ← TV2: Income source management
│   │   ├── ExpenseCategoryManager.h           ← TV2: Expense category management
│   │   ├── TransactionManager.h               ← TV3: Income/expense management
│   │   └── RecurringTransactionManager.h      ← TV3: Recurring transaction management
│   │
│   ├── statistics/                            ← TV4: Statistics & Reporting
│   │   ├── StatisticsManager.h                ← Financial statistics and reports
│   │   └── NumberFormatter.h                  ← Currency and number formatting
│   │
│   └── ui/                                    ← Console user interface
│       ├── MasterDataMenu.h                   ← TV2: Master data menus
│       ├── TransactionMenu.h                  ← TV3: Transaction menus
│       ├── StatisticsMenu.h                   ← TV4: Statistics menus
│       └── Dashboard.h                        ← TV4: Financial overview
│
└── src/                                       ← All .cpp source files
    ├── main.cpp                               ← TV1: Application entry point
    │
    ├── core/                                  ← TV1
    │   ├── Date.cpp
    │   ├── FinanceManager.cpp
    │   └── PersistenceManager.cpp
    │
    ├── models/
    │   ├── Wallet.cpp                         ← TV2
    │   ├── IncomeSource.cpp                   ← TV2
    │   ├── ExpenseCategory.cpp                ← TV2
    │   ├── Transaction.cpp                    ← TV3
    │   ├── Income.cpp                         ← TV3
    │   ├── Expense.cpp                        ← TV3
    │   └── RecurringTransaction.cpp           ← TV3
    │
    ├── managers/
    │   ├── WalletManager.cpp                  ← TV2
    │   ├── IncomeSourceManager.cpp            ← TV2
    │   ├── ExpenseCategoryManager.cpp         ← TV2
    │   ├── TransactionManager.cpp             ← TV3
    │   └── RecurringTransactionManager.cpp    ← TV3
    │
    ├── statistics/                            ← TV4
    │   ├── StatisticsManager.cpp
    │   └── NumberFormatter.cpp
    │
    └── ui/
        ├── MasterDataMenu.cpp                 ← TV2
        ├── TransactionMenu.cpp                ← TV3
        ├── StatisticsMenu.cpp                 ← TV4
        └── Dashboard.cpp                      ← TV4
```

---

## File Rules

### Header files `.h`

```cpp
// Required at the top of each .h file
#pragma once

// Include order:
// 1. Standard library
#include <string>

// 2. Project headers (relative to include/)
#include "core/DynamicArray.h"
#include "models/Wallet.h"
```

### Source files `.cpp`

```cpp
// Required header comment at the top of each .cpp file
/**
 * @file WalletManager.cpp
 * @author TV2
 * @brief Manages wallets and their balances
 */

// Include its own header first
#include "managers/WalletManager.h"

// Followed by other required headers
#include "models/Wallet.h"
```

### Main file

`main.cpp` should only initialize and run the application.

```
#include "core/FinanceManager.h"

int main() {
    FinanceManager app;
    app.run();
    return 0;
}
```

The `FinanceManager` coordinates the application's modules. Business logic must remain in the corresponding managers.

## Binary Data Storage

**Rules:**

- Use fixed-size records to support direct access with `seekg()` and `seekp()`.
- Store variable-length names in separate Name Pool files.
- Reference names using their offset and length.
- Handle renaming and deletion without corrupting existing records.

**Responsibilities:**

- **TV1:** Designs the binary file format and implements the overall save/load system.
- **TV2:** Provides serialization support for wallets, income sources, and expense categories.
- **TV3:** Provides serialization support for transactions and recurring transactions.
- **TV4:** Verifies statistics and dashboard results after loading saved data.

### File Paths

Always use relative paths from the agreed working directory.

```cpp
// CORRECT
std::string filePath = "data/wallets.bin";

// INCORRECT — hardcoded absolute path, will not run on others' machines
std::string filePath = "/Users/name/project/data/wallets.bin";
```

---

## Naming Conventions Quick Guide

| Type                  | Convention               | Example                                |
| --------------------- | ------------------------ | -------------------------------------- |
| Class / Struct        | `PascalCase`             | `WalletManager`, `Transaction`         |
| Method                | `camelCase`              | `addIncome()`, `loadData()`            |
| Variable              | `camelCase`              | `walletBalance`, `totalIncome`         |
| Private member        | `m_` prefix              | `m_wallets`, `m_balance`               |
| Constant / enum value | `UPPER_SNAKE_CASE`       | `MAX_CAPACITY`, `INCOME`               |
| File                  | Class name               | `WalletManager.h`, `WalletManager.cpp` |
| Branch                | `feat/`, `fix/`, `docs/` | `feat/wallet-management`               |

→ Full details: see [`CODING_RULES.md`](CODING_RULES.md)

