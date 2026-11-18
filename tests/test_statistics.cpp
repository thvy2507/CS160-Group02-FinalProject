/**
 * @file test_statistics.cpp
 * @author TV4
 * @brief Tests date-range financial statistics.
 */

#include "statistics/StatisticsManager.h"

#include <iostream>

#include "core/Date.h"
#include "managers/ExpenseCategoryManager.h"
#include "managers/IncomeSourceManager.h"
#include "managers/TransactionManager.h"
#include "managers/WalletManager.h"

int main() {
    WalletManager wallets;
    IncomeSourceManager sources;
    ExpenseCategoryManager categories;
    TransactionManager transactions(&wallets, &sources, &categories);
    StatisticsManager statistics;

    StatisticsResult result;
    result.totalIncome = 100;
    result.totalExpense = 50;
    result.netBalance = 50;

    bool success = statistics.calculateSummary(
        transactions, Date(1, 11, 2026), Date(30, 11, 2026), result
    );

    if (!success || result.totalIncome != 0
        || result.totalExpense != 0 || result.netBalance != 0) {
        std::cout << "FAIL: empty transaction list\n";
        return 1;
    }

    std::cout << "PASS: empty transaction list\n";
    return 0;
}