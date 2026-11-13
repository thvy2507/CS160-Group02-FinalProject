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
