/**
 * @file test_statistics.cpp
 * @author TV4
 * @brief Tests date-range summaries and read-only transaction access.
 */

#include "statistics/StatisticsManager.h"
#include <stdexcept>

#include <iostream>

#include "core/Date.h"
#include "managers/ExpenseCategoryManager.h"
#include "managers/IncomeSourceManager.h"
#include "managers/TransactionManager.h"
#include "managers/WalletManager.h"
#include "models/Expense.h"
#include "models/Income.h"

bool checkSummary(
    const StatisticsManager& statistics,
    const TransactionManager& transactions,
    const Date& start, const Date& end,
    long long income, long long expense,
    const char* name) {
    StatisticsResult result;
    bool success = statistics.calculateSummary(
        transactions, start, end, result
    );

    if (!success || result.totalIncome != income
        || result.totalExpense != expense
        || result.netBalance != income - expense) {
        std::cout << "FAIL: " << name << '\n';
        return false;
    }

    std::cout << "PASS: " << name << '\n';
    return true;
}

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

    Date invalidRanges[][2] = {
        {Date(30, 11, 2026), Date(1, 11, 2026)}
    };

    for (const auto& range : invalidRanges) {
        result.totalIncome = 100;
        result.totalExpense = 50;
        result.netBalance = 50;

        success = statistics.calculateSummary(
            transactions, range[0], range[1], result
        );

        if (success || result.totalIncome != 100
            || result.totalExpense != 50 || result.netBalance != 50) {
            std::cout << "FAIL: invalid range preserves output\n";
            return 1;
        }
    }
    std::cout << "PASS: invalid ranges preserve output\n";

    bool invalidDateRejected = false;

    try {
        Date invalidDate(31, 11, 2026);
        (void)invalidDate;
    } catch (const std::invalid_argument&) {
        invalidDateRejected = true;
    }

    if (!invalidDateRejected) {
        std::cout << "FAIL: invalid date construction\n";
        return 1;
    }

    std::cout << "PASS: invalid date construction rejected\n";

    int walletId = wallets.addWallet("Cash", 1000000);
    int sourceId = sources.addIncomeSource("Salary");
    int categoryId = categories.addExpenseCategory("Food");

    if (walletId <= 0 || sourceId <= 0 || categoryId <= 0) {
        std::cout << "FAIL: test data setup\n";
        return 1;
    }

    bool added =
        transactions.addIncome(
            Income(0, Date(1, 11, 2026), 2000000,
                   "Salary", walletId, sourceId))
        && transactions.addIncome(
            Income(0, Date(20, 11, 2026), 300000,
                   "Extra income", walletId, sourceId))
        && transactions.addExpense(
            Expense(0, Date(10, 11, 2026), 200000,
                    "Food", walletId, categoryId))
        && transactions.addExpense(
            Expense(0, Date(30, 11, 2026), 500000,
                    "Food", walletId, categoryId))
        && transactions.addIncome(
            Income(0, Date(31, 10, 2026), 100000,
                   "Previous month", walletId, sourceId))
        && transactions.addExpense(
            Expense(0, Date(1, 12, 2026), 50000,
                    "Next month", walletId, categoryId));

    if (!added) {
        std::cout << "FAIL: add sample transactions\n";
        return 1;
    }

    int countBefore = transactions.getTransactionCount();
    long long balanceBefore = wallets.getWalletBalance(walletId);

    if (!checkSummary(
            statistics, transactions,
            Date(1, 11, 2026), Date(30, 11, 2026),
            2300000, 700000, "month totals exclude outside dates")) {
        return 1;
    }

    if (!checkSummary(
            statistics, transactions,
            Date(1, 11, 2026), Date(1, 11, 2026),
            2000000, 0, "start boundary and income only")) {
        return 1;
    }

    if (!checkSummary(
            statistics, transactions,
            Date(30, 11, 2026), Date(30, 11, 2026),
            0, 500000, "end boundary and negative net balance")) {
        return 1;
    }

    if (!checkSummary(
            statistics, transactions,
            Date(1, 1, 2027), Date(31, 1, 2027),
            0, 0, "no matching transactions")) {
        return 1;
    }

    if (transactions.getTransactionCount() != countBefore
        || wallets.getWalletBalance(walletId) != balanceBefore) {
        std::cout << "FAIL: statistics changed count or wallet balance\n";
        return 1;
    }
    std::cout << "PASS: transaction count and wallet balance preserved\n";

    return 0;
}