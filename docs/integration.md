# Integration and acceptance
Add the following Statistics and Dashboard source files to the root CMakeLists.txt during integration:
```cmake
src/statistics/StatisticsManager.cpp
src/statistics/NumberFormatter.cpp
src/ui/Dashboard.cpp
src/ui/StatisticsMenu.cpp
```

Do not add tests/test_statistics.cpp to the main application (it has a separate main). verification/CMakeLists.txt is an isolated check, not a replacement for the team root CMakeLists.txt. Keep that folder outside the repo unless TV1 agrees to include it. No shared model, FinanceManager or root build files are supplied or modified by this package.

## Acceptance status

- Four header/source pairs: complete as Week 1 skeletons.
- Report contracts and layout/navigation proposal: documented.
- Sample data and expected results: prepared for all five reports.
- Naming, comments, English documentation, const inputs, no STL containers: applied against supplied coding rules.
- Standalone C++17 compile/link and menu smoke check: run in Linux.
- Actual shared model compatibility: pending TV1/TV2/TV3 confirmation.
- Team CLI integration and Windows/macOS build: pending on team machines.
- Full financial algorithms and formatting: intentionally deferred.

ROLES.md mentions a Sunday synchronization while WEEKLY_PLAN.md and CODING_RULES.md specify a Friday checkpoint. Confirm the actual meeting day with TV1; use the weekly plan's Friday demo as the current planning assumption.

## Sprint 2 - Statistics integration

### Statistics Menu

TV1 can open the statistics menu using:

```cpp
#include "ui/StatisticsMenu.h"

// Pass the application's existing TransactionManager instance.
StatisticsMenu menu;
menu.run(transactionManager);
```

`transactionManager` represents the existing manager used to add and store
the application's transactions. Use that same instance for reporting.

- Option 1 requests the start and end dates in `DD/MM/YYYY` format.
- Both date boundaries are included.
- A successful report displays Income, Expense, and Net Balance in VND.
- Invalid date input prompts again.
- Reversed ranges or calculation failures display an error.
- Reports 2-5 display an unavailable message.
- After a report, the statistics menu appears again.
- Option 0 returns to the main menu.
- End-of-input returns control to the caller.

### Dashboard navigation proposal

The following rules require confirmation from TV1:

- Display the Dashboard after application data is ready and before the main menu.
- Refresh displayed balances when returning to the main menu.
- Read current wallet balances from TV2's manager.
- Statistics Menu option 0 returns to the main menu.
- Main Menu option 0 exits the application.
- Full Dashboard rendering remains scheduled for a later sprint.
### Build and verification status

- Master data, transaction, statistics, and UI modules are linked
  through the root CMakeLists.txt.
- Full application build passed on macOS.
- All 8 statistics test cases passed.
- Date input format is consistent with Core (DD/MM/YYYY).
- Transaction ID assignment has been verified.
- FinanceManager integration is pending.
- Standalone Statistics Menu interaction tests passed.
