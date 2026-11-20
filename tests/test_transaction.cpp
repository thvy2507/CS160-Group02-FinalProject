/**
 * @file test_transaction.cpp
 * @author TV1 (Vy)
 * @brief Tests transaction operations and wallet balance updates.
 */

#include <cassert>
#include <iostream>
#include <limits>

#include "core/Date.h"
#include "managers/ExpenseCategoryManager.h"
#include "managers/IncomeSourceManager.h"
#include "managers/TransactionManager.h"
#include "managers/WalletManager.h"
#include "models/Expense.h"
#include "models/Income.h"

struct TestContext {
    WalletManager wallets;
    IncomeSourceManager sources;
    ExpenseCategoryManager categories;
    TransactionManager transactions;

    int walletId;
    int sourceId;
    int categoryId;
    Date date;

    TestContext()
        : transactions(&wallets, &sources, &categories),
          date(20, 11, 2026) {
        walletId = wallets.addWallet("Cash", 100000);
        sourceId = sources.addIncomeSource("Salary");
        categoryId = categories.addExpenseCategory("Food");

        assert(walletId > 0);
        assert(sourceId > 0);
        assert(categoryId > 0);
    }
};

void testAddIncome(TestContext& context) {
    Income income(0, context.date, 50000, "Salary", context.walletId, context.sourceId);

    assert(context.transactions.addIncome(income));
    assert(context.transactions.getTransactionCount() == 1);
    assert(context.wallets.getWalletBalance(context.walletId) == 150000);

    const Transaction* record = context.transactions.getTransactionAt(0);

    assert(record != nullptr);
    assert(record->getId() == 1);
    assert(record->getType() == TransactionType::INCOME);
    assert(record->getAmount() == 50000);
    assert(record->getDate() == context.date);
    assert(record->getWalletId() == context.walletId);
    assert(record->getDescription() == "Salary");

    std::cout << "PASS: Add income\n";
}

void testAddExpense(TestContext& context) {
    Expense expense(0, context.date, 30000, "Lunch", context.walletId, context.categoryId);

    assert(context.transactions.addExpense(expense));
    assert(context.transactions.getTransactionCount() == 2);
    assert(context.wallets.getWalletBalance(context.walletId) == 120000);

    const Transaction* record = context.transactions.getTransactionAt(1);

    assert(record != nullptr);
    assert(record->getId() == 2);
    assert(record->getType() == TransactionType::EXPENSE);
    assert(record->getAmount() == 30000);
    assert(record->getDate() == context.date);
    assert(record->getWalletId() == context.walletId);
    assert(record->getDescription() == "Lunch");

    std::cout << "PASS: Add expense\n";
}

void testSearchTransactions(const TestContext& context) {
    assert(context.transactions.getTransactionById(1) != nullptr);
    assert(context.transactions.getTransactionById(2) != nullptr);
    assert(context.transactions.getTransactionById(0) == nullptr);
    assert(context.transactions.getTransactionById(999) == nullptr);

    assert(context.transactions.getTransactionAt(-1) == nullptr);
    assert(context.transactions.getTransactionAt(2) == nullptr);

    assert(context.transactions.getTransactionById(1)
           == context.transactions.getTransactionAt(0));

    std::cout << "PASS: Search transactions\n";
}

void checkRejectedTransaction(const TestContext& context, bool success) {
    assert(!success);
    assert(context.transactions.getTransactionCount() == 2);
    assert(context.wallets.getWalletBalance(context.walletId) == 120000);
}

void testInvalidAmounts(TestContext& context) {
    Income zeroIncome(0, context.date, 0, "", context.walletId, context.sourceId);
    Income negativeIncome(0, context.date, -1000, "", context.walletId, context.sourceId);

    Expense zeroExpense(0, context.date, 0, "", context.walletId, context.categoryId);
    Expense negativeExpense(0, context.date, -1000, "", context.walletId, context.categoryId);

    checkRejectedTransaction(context, context.transactions.addIncome(zeroIncome));
    checkRejectedTransaction(context, context.transactions.addIncome(negativeIncome));
    checkRejectedTransaction(context, context.transactions.addExpense(zeroExpense));
    checkRejectedTransaction(context, context.transactions.addExpense(negativeExpense));

    std::cout << "PASS: Reject invalid amounts\n";
}

void testInvalidReferences(TestContext& context) {
    const int INVALID_ID = 999;

    Income invalidWalletIncome(0, context.date, 1000, "", INVALID_ID, context.sourceId);

    Expense invalidWalletExpense(0, context.date, 1000, "", INVALID_ID, context.categoryId);

    Income invalidSource(0, context.date, 1000, "", context.walletId, INVALID_ID);

    Expense invalidCategory(0, context.date, 1000, "", context.walletId, INVALID_ID);

    checkRejectedTransaction(context, context.transactions.addIncome(invalidWalletIncome));
    checkRejectedTransaction(context, context.transactions.addExpense(invalidWalletExpense));
    checkRejectedTransaction(context, context.transactions.addIncome(invalidSource));
    checkRejectedTransaction(context, context.transactions.addExpense(invalidCategory));

    std::cout << "PASS: Reject invalid references\n";
}

void testInsufficientBalance(TestContext& context) {
    Expense expense(0, context.date, 120001, "", context.walletId, context.categoryId);

    checkRejectedTransaction(context, context.transactions.addExpense(expense));

    std::cout << "PASS: Reject insufficient balance\n";
}

void testDeletedReferences(TestContext& context) {
    int deletedWalletId = context.wallets.addWallet("Old wallet");
    int deletedSourceId = context.sources.addIncomeSource("Old source");
    int deletedCategoryId = context.categories.addExpenseCategory("Old category");

    assert(deletedWalletId > 0);
    assert(deletedSourceId > 0);
    assert(deletedCategoryId > 0);

    assert(context.wallets.deleteWallet(deletedWalletId));
    assert(context.sources.deleteIncomeSource(deletedSourceId));
    assert(context.categories.deleteExpenseCategory(deletedCategoryId));

    Income invalidWalletIncome(0, context.date, 1000, "", deletedWalletId, context.sourceId);

    Expense invalidWalletExpense(0, context.date, 1000, "", deletedWalletId, context.categoryId);

    Income invalidSource(0, context.date, 1000, "", context.walletId, deletedSourceId);

    Expense invalidCategory(0, context.date, 1000, "", context.walletId, deletedCategoryId);

    checkRejectedTransaction(context, context.transactions.addIncome(invalidWalletIncome));
    checkRejectedTransaction(context, context.transactions.addExpense(invalidWalletExpense));
    checkRejectedTransaction(context, context.transactions.addIncome(invalidSource));
    checkRejectedTransaction(context, context.transactions.addExpense(invalidCategory));

    std::cout << "PASS: Reject deleted references\n";
}

void testSequentialIds(TestContext& context) {
    Income income(0, context.date, 10000, "Bonus", context.walletId, context.sourceId);

    assert(context.transactions.addIncome(income));
    assert(context.transactions.getTransactionCount() == 3);
    assert(context.wallets.getWalletBalance(context.walletId) == 130000);

    const Transaction* record = context.transactions.getTransactionAt(2);

    assert(record != nullptr);
    assert(record->getId() == 3);

    std::cout << "PASS: Sequential IDs after rejected transactions\n";
}

void testViewTransactions(const TestContext& context) {
    assert(context.transactions.viewTransaction(1));
    assert(!context.transactions.viewTransaction(999));

    context.transactions.viewAllTransactions();

    assert(context.transactions.getTransactionCount() == 3);
    assert(context.wallets.getWalletBalance(context.walletId) == 130000);

    std::cout << "PASS: View transactions\n";
}

void testBalanceOverflow() {
    const Money MAX_BALANCE = std::numeric_limits<Money>::max();

    Wallet wallet(1, "Limit", MAX_BALANCE);

    assert(!wallet.deposit(1));
    assert(wallet.getBalance() == MAX_BALANCE);

    WalletManager wallets;
    IncomeSourceManager sources;
    ExpenseCategoryManager categories;
    TransactionManager transactions(&wallets, &sources, &categories);

    int cashId = wallets.addWallet("Cash", MAX_BALANCE - 1);
    int bankId = wallets.addWallet("Bank", 1);
    int sourceId = sources.addIncomeSource("Salary");

    assert(cashId > 0);
    assert(bankId > 0);
    assert(sourceId > 0);
    assert(wallets.getTotalBalance() == MAX_BALANCE);

    assert(wallets.addWallet("Overflow", 1) == Wallet::INVALID_ID);
    assert(wallets.getActiveWalletCount() == 2);

    Income income(0, Date(20, 11, 2026), 1, "Overflow income", cashId, sourceId);

    assert(!transactions.addIncome(income));
    assert(transactions.getTransactionCount() == 0);
    assert(wallets.getWalletBalance(cashId) == MAX_BALANCE - 1);
    assert(wallets.getTotalBalance() == MAX_BALANCE);

    std::cout << "PASS: Prevent wallet balance overflow\n";
}

int main() {
    std::cout << "===== TRANSACTION TESTS =====\n\n";

    TestContext context;

    testAddIncome(context);
    testAddExpense(context);
    testSearchTransactions(context);
    testInvalidAmounts(context);
    testInvalidReferences(context);
    testInsufficientBalance(context);
    testDeletedReferences(context);
    testSequentialIds(context);
    testViewTransactions(context);
    testBalanceOverflow();

    std::cout << "\nAll transaction tests passed!\n";

    return 0;
}
