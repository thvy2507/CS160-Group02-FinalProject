cmake_minimum_required(VERSION 3.15)

project(PersonalFinanceManager LANGUAGES CXX)

add_executable(PersonalFinanceManager
    src/main.cpp

    # Core
    src/core/FinanceManager.cpp
    src/core/Date.cpp
    src/core/BinaryIO.cpp

    # Models
    src/models/Wallet.cpp
    src/models/IncomeSource.cpp
    src/models/ExpenseCategory.cpp
    src/models/Transaction.cpp
    src/models/Income.cpp
    src/models/Expense.cpp

    # Managers
    src/managers/WalletManager.cpp
    src/managers/IncomeSourceManager.cpp
    src/managers/ExpenseCategoryManager.cpp
    src/managers/TransactionManager.cpp

    # Statistics
    src/statistics/StatisticsManager.cpp
    src/statistics/NumberFormatter.cpp

    # UI
    src/ui/Dashboard.cpp
    src/ui/StatisticsMenu.cpp
)

target_compile_features(PersonalFinanceManager PRIVATE cxx_std_17)

set_target_properties(PersonalFinanceManager PROPERTIES
    CXX_EXTENSIONS OFF
)

target_include_directories(PersonalFinanceManager PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

if(MSVC)
    target_compile_options(PersonalFinanceManager PRIVATE /W4)
else()
    target_compile_options(PersonalFinanceManager PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()
