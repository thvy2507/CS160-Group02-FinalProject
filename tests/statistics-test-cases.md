# Future statistics test cases

These are hand-calculated expected results, not passing algorithm tests.
Money is VND. Opening balances before all listed transactions: Cash (1) 500000,
Bank (2) 1000000. Sources: Salary (1), Bonus (2). Categories: Food (1), Rent (2).

| Date | Type | Wallet | Source/category ID | Amount |
|---|---|---|---|---:|
| 2025-12-31 | INCOME | 1 | 2 | 100000 |
| 2026-10-01 | INCOME | 2 | 1 | 2000000 |
| 2026-10-03 | EXPENSE | 1 | 1 | 50000 |
| 2026-10-10 | EXPENSE | 2 | 2 | 500000 |
| 2026-10-31 | EXPENSE | 1 | 1 | 10000 |
| 2026-11-01 | INCOME | 1 | 2 | 200000 |

| Case | Input | Expected income / expense / net or grouped result |
|---|---|---|
| S1 boundaries | 2026-10-01 to 2026-10-31 | 2000000 / 560000 / 1440000 |
| S2 Bank | Same dates, wallet 2 | 2000000 / 500000 / 1500000 |
| S2 Cash | Same dates, wallet 1 | 0 / 60000 / -60000 |
| S3 one year | 2026 | 2200000 / 560000 / 1640000 |
| S3 multiple | 2025, 2026 | 2025: 100000 / 0 / 100000; 2026: 2200000 / 560000 / 1640000 |
| S3 duplicate | 2026, 2026 | One 2026 row, same totals |
| S4 sources | 2025, 2026 | Salary 2000000; Bonus 300000 |
| S5 categories | 2026 | Food 60000; Rent 500000 |
| No matches | 2027-01-01 to 2027-12-31 | 0 / 0 / 0, success |
| Single day | 2026-10-31 to 2026-10-31 | 0 / 10000 / -10000 |
| Income only | 2025-12-31 to 2025-12-31 | 100000 / 0 / 100000 |
| Invalid range | Start after end | false, output unchanged |
| Invalid date | Date(30, 2, 2026) | Date constructor throws std::invalid_argument before calling StatisticsManager |
| Missing wallet | ID 999 | false, output unchanged |
| Empty years | No selected years | false, output unchanged |
| Empty data | Empty transactions, year 2026 | Annual zero row; empty breakdowns |
| Dashboard | After all sample transactions | Cash 740000, Bank 2500000, total 3240000 |
| Empty wallets | Empty wallet list | No wallets available, total 0 VND |
| Formatter | 1234567; -500; 0 | 1,234,567 VND; -500 VND; 0 VND |

Future persistence check: load this same dataset through TV1's API and confirm
identical report/dashboard results. This check cannot run without that module.
