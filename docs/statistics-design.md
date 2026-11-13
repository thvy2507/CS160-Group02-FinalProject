# Week 1 statistics and dashboard design

Status: proposed contract, not a confirmed shared model API.

## Reports

| Report | Inputs | Successful output |
|---|---|---|
| Date summary | Transactions, inclusive start/end dates | Total income, expense, income minus expense |
| Wallet summary | Same filters plus wallet ID and wallets | Same totals for one existing wallet |
| Annual overview | Transactions, selected years | One summary per distinct year, ascending |
| Income by source | Transactions, selected years | One total per source ID across selected years |
| Expenses by category | Transactions, selected years | One total per category ID across selected years |

All collections use the shared DynamicArray<T>. Results contain IDs; the UI resolves names through TV2's managers. Money uses long long (whole VND), as agreed by the team. Display whole VND with commas, a leading minus for negative values, and suffix `VND`.

Success returns true and replaces output. No matching transactions is successful with zero totals (empty breakdowns; annual rows still include selected years).
Invalid dates, reversed ranges, empty year selections, and invalid wallet IDs return false without changing output. Duplicate years are counted once. Reports use actual transactions only, never recurring templates.
Date boundaries are inclusive. Filter transactions before grouping. Net change is not current wallet balance. Group by stable IDs, never names; unresolved names should display `Unknown (ID ...)` rather than dropping historical amounts.

Skeleton methods currently return false for every request. Callers must check this status before using output; default zero fields do not represent a report.
An explicit error/status enum can be agreed later if failure reasons are needed.

## Shared data contract to confirm

| Owner | Required access | Proposed meaning |
|---|---|---|
| TV1 | Date comparison, year, validity | Calendar dates, inclusive comparison |
| TV1 | DynamicArray<T> size, indexing, append, clear | Read inputs, write result collections |
| TV2 | Wallet ID, name, current balance; lookup by ID | Authoritative current balances |
| TV2 | Source/category lookup by ID | Display labels for grouped results |
| TV3 | Transaction date, amount, type, wallet ID, source/category ID | Positive amount; type determines income/expense |
| TV3 | TransactionManager::getTransactionCount() and getTransactionAt() | Read transactions without copying abstract Transaction objects |

StatisticsManager receives a const TransactionManager& and accesses transactions through its read-only methods.
Forward declarations deliberately avoid inventing shared implementations.
Confirm that DynamicArray is a one-parameter class template and model names are global types. If the team uses aliases/namespaces or separate Income/Expense collections, adapt these declarations and signatures before merging.
TV4 never mutates transactions or wallet balances and never reads binary files directly. FinanceManager owns data lifetime and passes const references. Output collections belong to the caller; do not cache references across mutations.

## Dashboard and navigation

Proposed startup order: TV1 loads data, TV3 processes due recurring transactions and coordinates balance updates with TV2, then Dashboard shows current balances above the main menu. On return to main menu, refresh balances. Confirm with TV1.

| Display | Example after the sample transactions |
|---|---|
| Total balance | 3,240,000 VND |
| Cash | 740,000 VND |
| Bank | 2,500,000 VND |

Show `No wallets available` and total 0 VND for an empty wallet list. Preserve full names (wrap long names); align numeric columns. Negative balances remain visible.
Dashboard sums TV2's current balances; it does not add transactions to them again.

StatisticsMenu displays choices 1–5 and 0 (Back). FinanceManager dispatches the choice, supplies report filters/data, and returns to the statistics menu after a report. Back returns to main menu; it does not exit the application. In Week 1, only menu input and a labeled layout preview are available. Failed reports show `Report unavailable or invalid input`; never display output as if successful.

## Planned algorithms

Date/wallet summaries: one scan, filter dates and optional wallet, accumulate by type. Annual report: scan for each unique selected year. Breakdowns: scan selected years, find/create a result row for the grouping ID and add the amount. Start with linear lookup using DynamicArray; optimize only if necessary. Formatting and full report rendering are deferred. TV4 does not own persistence or balance updates.
