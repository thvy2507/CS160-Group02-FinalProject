/**
 * @file test_wallet_statistics.cpp
 * @author TV4
 * @brief Tests wallet-based statistics and error handling.
 */

#include <iostream>
#include <limits>

#include "core/Date.h"
#include "managers/ExpenseCategoryManager.h"
#include "managers/IncomeSourceManager.h"
#include "managers/TransactionManager.h"
#include "managers/WalletManager.h"
#include "models/Expense.h"
#include "models/Income.h"
#include "statistics/StatisticsManager.h"

namespace {

struct TestData {
    WalletManager wallets;
    IncomeSourceManager sources;
    ExpenseCategoryManager categories;
    TransactionManager transactions{&wallets, &sources, &categories};
};

bool check(bool passed, const char* name) {
    std::cout << (passed ? "PASS: " : "FAIL: ") << name << '\n';
    return passed;
}

bool checkSummary(const TestData& data, int walletId,
                  const Date& start, const Date& end, const char* name,
                  StatisticsResult expected, bool expectedSuccess = true) {
    StatisticsManager statistics;
    StatisticsResult result{900, 100, 800};
    if (!expectedSuccess) {
        expected = result;
    }

    const bool success = statistics.calculateWalletSummary(
        data.transactions, start, end, walletId, data.wallets.getAllWallets(), result);
    return check(success == expectedSuccess && result.totalIncome == expected.totalIncome
                 && result.totalExpense == expected.totalExpense
                 && result.netBalance == expected.netBalance, name);
}

bool testWalletSummaries() {
    TestData data;
    const int cash = data.wallets.addWallet("Cash", 1000);
    const int bank = data.wallets.addWallet("Bank", 1000);
    const int deleted = data.wallets.addWallet("Deleted");
    const int salary = data.sources.addIncomeSource("Salary");
    const int food = data.categories.addExpenseCategory("Food");
    const Date start(1, 11, 2026);
    const Date end(30, 11, 2026);

    const bool ready = cash > 0 && bank > 0 && deleted > 0 && salary > 0 && food > 0
        && data.wallets.deleteWallet(deleted)
        && data.transactions.addIncome(Income(0, Date(31, 10, 2026), 40, "October", cash, salary))
        && data.transactions.addIncome(Income(0, start, 100, "Cash income", cash, salary))
        && data.transactions.addIncome(Income(0, start, 400, "Bank income", bank, salary))
        && data.transactions.addExpense(Expense(0, end, 30, "Cash expense", cash, food))
        && data.transactions.addExpense(Expense(0, end, 80, "Bank expense", bank, food))
        && data.transactions.addExpense(Expense(0, Date(1, 12, 2026), 20, "December", cash, food));
    if (!ready) {
        return check(false, "wallet test data setup");
    }

    const int countBefore = data.transactions.getTransactionCount();
    const Money cashBefore = data.wallets.getWalletBalance(cash);
    const Money bankBefore = data.wallets.getWalletBalance(bank);

    return checkSummary(data, cash, start, end, "cash excludes other wallet and dates", {100, 30, 70})
        && checkSummary(data, bank, start, end, "bank has independent totals", {400, 80, 320})
        && checkSummary(data, cash, start, start, "start day is included", {100, 0, 100})
        && checkSummary(data, cash, end, end, "end day is included", {0, 30, -30})
        && checkSummary(data, cash, Date(1, 1, 2027), Date(31, 1, 2027), "no matching transactions", {})
        && checkSummary(data, Wallet::INVALID_ID, start, end, "invalid wallet ID preserves output", {}, false)
        && checkSummary(data, 9999, start, end, "unknown wallet ID preserves output", {}, false)
        && checkSummary(data, deleted, start, end, "deleted wallet preserves output", {}, false)
        && checkSummary(data, cash, end, start, "reversed date range preserves output", {}, false)
        && check(data.transactions.getTransactionCount() == countBefore
                 && data.wallets.getWalletBalance(cash) == cashBefore
                 && data.wallets.getWalletBalance(bank) == bankBefore,
                 "transaction count and wallet balances are preserved");
}

bool testOverflow() {
    TestData data;
    const int wallet = data.wallets.addWallet("Overflow test");
    const int source = data.sources.addIncomeSource("Salary");
    const int category = data.categories.addExpenseCategory("Food");
    const Money limit = std::numeric_limits<Money>::max();
    const Date first(1, 11, 2026);
    const Date second(2, 11, 2026);
    const Date third(3, 11, 2026);

    const bool ready = wallet > 0 && source > 0 && category > 0
        && data.transactions.addIncome(Income(0, first, limit, "Maximum income", wallet, source))
        && data.transactions.addExpense(Expense(0, second, limit, "Maximum expense", wallet, category))
        && data.transactions.addIncome(Income(0, third, 1, "Extra income", wallet, source))
        && data.transactions.addExpense(Expense(0, third, 1, "Extra expense", wallet, category));
    if (!ready) {
        return check(false, "overflow test data setup");
    }

    return checkSummary(data, wallet, first, second, "exact limits are accepted", {limit, limit, 0})
        && checkSummary(data, wallet, first, third, "income overflow preserves output", {}, false)
        && checkSummary(data, wallet, second, third, "expense overflow preserves output", {}, false);
}

} // namespace

int main() {
    const bool summaryPassed = testWalletSummaries();
    const bool overflowPassed = testOverflow();
    return summaryPassed && overflowPassed ? 0 : 1;
}
