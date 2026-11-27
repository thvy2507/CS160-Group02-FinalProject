/**
 * @file test_recurring.cpp
 * @author TV3
 * @brief Tests recurring transaction creation, validation and monthly generation
 */

#include <cassert>
#include <iostream>

#include "core/Date.h"
#include "managers/ExpenseCategoryManager.h"
#include "managers/IncomeSourceManager.h"
#include "managers/RecurringTransactionManager.h"
#include "managers/TransactionManager.h"
#include "managers/WalletManager.h"
#include "models/Expense.h"
#include "models/Income.h"
#include "models/RecurringTransaction.h"

// Each recurring test creates its own context, so the tests do not affect each other
struct RecurringTestContext {
    WalletManager wallets;
    IncomeSourceManager sources;
    ExpenseCategoryManager categories;
    TransactionManager transactions;
    RecurringTransactionManager recurring;

    int walletId;
    int sourceId;
    int categoryId;

    RecurringTestContext()
        : transactions(&wallets, &sources, &categories),
          recurring(&wallets, &sources, &categories) {
        walletId = wallets.addWallet("Cash", 100000);
        sourceId = sources.addIncomeSource("Salary");
        categoryId = categories.addExpenseCategory("Rent");

        assert(walletId > 0);
        assert(sourceId > 0);
        assert(categoryId > 0);
    }
};

// Income of 1000 VND on the given day of every month
RecurringTransaction makeIncomeRecurring(const RecurringTestContext& context,
                                         const Date& startDate, int dayOfMonth) {
    return RecurringTransaction(0, TransactionType::INCOME, 1000, "Salary",
                                context.walletId, context.sourceId, startDate,
                                dayOfMonth);
}

RecurringTransaction makeExpenseRecurring(const RecurringTestContext& context,
                                          long long amount, const Date& startDate,
                                          int dayOfMonth) {
    return RecurringTransaction(0, TransactionType::EXPENSE, amount, "Rent",
                                context.walletId, context.categoryId, startDate,
                                dayOfMonth);
}

void checkRejectedRecurring(const RecurringTestContext& context, bool success) {
    assert(!success);
    assert(context.recurring.getRecurringTransactionCount() == 0);
}

void testCreateRecurring() {
    RecurringTestContext context;

    RecurringTransaction salary = makeIncomeRecurring(context, Date(5, 1, 2026), 5);
    assert(context.recurring.createRecurringTransaction(salary));
    assert(context.recurring.getRecurringTransactionCount() == 1);

    const RecurringTransaction* record = context.recurring.getRecurringTransactionById(1);

    assert(record != nullptr);
    assert(record->getId() == 1);
    assert(record->getType() == TransactionType::INCOME);
    assert(record->getAmount() == 1000);
    assert(record->getDayOfMonth() == 5);
    assert(record->getStartDate() == Date(5, 1, 2026));
    assert(record->getWalletId() == context.walletId);
    assert(record->getSourceOrCategoryId() == context.sourceId);
    assert(!record->hasEndDate());
    assert(!record->hasLastGeneratedDate());
    assert(record->isActive());

    RecurringTransaction rent = makeExpenseRecurring(context, 500, Date(1, 1, 2026), 1);
    assert(context.recurring.createRecurringTransaction(rent));
    assert(context.recurring.getRecurringTransactionById(2) != nullptr);

    std::cout << "PASS: Create recurring transaction\n";
}

void testRejectInvalidRecurring() {
    RecurringTestContext context;
    const int INVALID_ID = 999;
    const Date start(31, 1, 2026);

    checkRejectedRecurring(context, context.recurring.createRecurringTransaction(
        makeIncomeRecurring(context, start, 0)));
    checkRejectedRecurring(context, context.recurring.createRecurringTransaction(
        makeIncomeRecurring(context, start, 32)));
    checkRejectedRecurring(context, context.recurring.createRecurringTransaction(
        makeExpenseRecurring(context, 0, start, 5)));
    checkRejectedRecurring(context, context.recurring.createRecurringTransaction(
        makeExpenseRecurring(context, -1000, start, 5)));

    RecurringTransaction invalidWallet(0, TransactionType::INCOME, 1000, "", INVALID_ID,
                                       context.sourceId, start, 5);
    RecurringTransaction invalidSource(0, TransactionType::INCOME, 1000, "",
                                       context.walletId, INVALID_ID, start, 5);
    RecurringTransaction invalidCategory(0, TransactionType::EXPENSE, 1000, "",
                                         context.walletId, INVALID_ID, start, 5);
    checkRejectedRecurring(context, context.recurring.createRecurringTransaction(invalidWallet));
    checkRejectedRecurring(context, context.recurring.createRecurringTransaction(invalidSource));
    checkRejectedRecurring(context, context.recurring.createRecurringTransaction(invalidCategory));

    // The end date is before the start date
    RecurringTransaction badEndDate = makeIncomeRecurring(context, start, 31);
    badEndDate.setEndDate(Date(1, 1, 2026));
    checkRejectedRecurring(context, context.recurring.createRecurringTransaction(badEndDate));

    std::cout << "PASS: Reject invalid recurring transactions\n";
}

void testGenerateLastDayOfMonth() {
    RecurringTestContext context;

    // Day 31, so shorter months (February, April) must use their last day
    assert(context.recurring.createRecurringTransaction(
        makeIncomeRecurring(context, Date(31, 1, 2026), 31)));

    int created = context.recurring.generateDueTransactions(Date(30, 4, 2026),
                                                            context.transactions);

    assert(created == 4);
    assert(context.transactions.getTransactionCount() == 4);
    assert(context.transactions.getTransactionAt(0)->getDate() == Date(31, 1, 2026));
    assert(context.transactions.getTransactionAt(1)->getDate() == Date(28, 2, 2026));
    assert(context.transactions.getTransactionAt(2)->getDate() == Date(31, 3, 2026));
    assert(context.transactions.getTransactionAt(3)->getDate() == Date(30, 4, 2026));
    assert(context.wallets.getWalletBalance(context.walletId) == 104000);

    const RecurringTransaction* record = context.recurring.getRecurringTransactionById(1);
    assert(record->hasLastGeneratedDate());
    assert(record->getLastGeneratedDate() == Date(30, 4, 2026));

    std::cout << "PASS: Use the last day of short months\n";
}

void testGenerateLeapYear() {
    RecurringTestContext context;

    assert(context.recurring.createRecurringTransaction(
        makeIncomeRecurring(context, Date(31, 1, 2028), 31)));

    int created = context.recurring.generateDueTransactions(Date(29, 2, 2028),
                                                            context.transactions);

    assert(created == 2);
    assert(context.transactions.getTransactionAt(1)->getDate() == Date(29, 2, 2028));

    std::cout << "PASS: Use February 29 in a leap year\n";
}

void testPreventDuplicateGeneration() {
    RecurringTestContext context;

    assert(context.recurring.createRecurringTransaction(
        makeIncomeRecurring(context, Date(5, 1, 2026), 5)));

    assert(context.recurring.generateDueTransactions(Date(5, 3, 2026),
                                                     context.transactions) == 3);

    // Running again with the same date must not create anything
    assert(context.recurring.generateDueTransactions(Date(5, 3, 2026),
                                                     context.transactions) == 0);
    assert(context.transactions.getTransactionCount() == 3);
    assert(context.wallets.getWalletBalance(context.walletId) == 103000);

    // A later date only creates the new month
    assert(context.recurring.generateDueTransactions(Date(5, 4, 2026),
                                                     context.transactions) == 1);
    assert(context.transactions.getTransactionCount() == 4);

    std::cout << "PASS: Prevent duplicate generation\n";
}

void testStartDate() {
    RecurringTestContext context;

    // Start on 20/01 with day 5: the occurrence on 05/01 is before the start date
    assert(context.recurring.createRecurringTransaction(
        makeIncomeRecurring(context, Date(20, 1, 2026), 5)));

    // Today is before the start date, so nothing is due
    assert(context.recurring.generateDueTransactions(Date(1, 1, 2026),
                                                     context.transactions) == 0);

    assert(context.recurring.generateDueTransactions(Date(31, 3, 2026),
                                                     context.transactions) == 2);
    assert(context.transactions.getTransactionAt(0)->getDate() == Date(5, 2, 2026));
    assert(context.transactions.getTransactionAt(1)->getDate() == Date(5, 3, 2026));

    std::cout << "PASS: Respect start date\n";
}

void testEndDate() {
    RecurringTestContext context;

    // The end date itself is included, later months are not
    RecurringTransaction limited = makeIncomeRecurring(context, Date(5, 1, 2026), 5);
    limited.setEndDate(Date(5, 3, 2026));
    assert(context.recurring.createRecurringTransaction(limited));

    assert(context.recurring.generateDueTransactions(Date(31, 12, 2026),
                                                     context.transactions) == 3);
    assert(context.transactions.getTransactionCount() == 3);

    const RecurringTransaction* record = context.recurring.getRecurringTransactionById(1);
    assert(record->getLastGeneratedDate() == Date(5, 3, 2026));

    std::cout << "PASS: Respect end date\n";
}

void testInactiveRecurring() {
    RecurringTestContext context;

    RecurringTransaction paused = makeIncomeRecurring(context, Date(5, 1, 2026), 5);
    paused.setActive(false);
    assert(context.recurring.createRecurringTransaction(paused));

    assert(context.recurring.generateDueTransactions(Date(31, 12, 2026),
                                                     context.transactions) == 0);
    assert(context.transactions.getTransactionCount() == 0);

    std::cout << "PASS: Skip inactive recurring transactions\n";
}

void testGenerationRetryAfterFailure() {
    RecurringTestContext context;

    // Wallet has 100000: January works, February fails (only 40000 left)
    assert(context.recurring.createRecurringTransaction(
        makeExpenseRecurring(context, 60000, Date(1, 1, 2026), 1)));

    assert(context.recurring.generateDueTransactions(Date(31, 3, 2026),
                                                     context.transactions) == 1);
    assert(context.wallets.getWalletBalance(context.walletId) == 40000);
    assert(context.recurring.getRecurringTransactionById(1)->getLastGeneratedDate()
           == Date(1, 1, 2026));

    // After adding money, the failed month is generated again
    assert(context.wallets.deposit(context.walletId, 100000));
    assert(context.recurring.generateDueTransactions(Date(31, 3, 2026),
                                                     context.transactions) == 2);
    assert(context.transactions.getTransactionCount() == 3);
    assert(context.wallets.getWalletBalance(context.walletId) == 20000);

    std::cout << "PASS: Retry generation after a failed month\n";
}

void testEditRecurring() {
    RecurringTestContext context;

    assert(context.recurring.createRecurringTransaction(
        makeIncomeRecurring(context, Date(5, 1, 2026), 5)));
    assert(context.recurring.generateDueTransactions(Date(5, 2, 2026),
                                                     context.transactions) == 2);

    RecurringTransaction raise(0, TransactionType::INCOME, 2000, "Raise",
                               context.walletId, context.sourceId, Date(5, 1, 2026), 5);
    assert(context.recurring.editRecurringTransaction(1, raise));

    const RecurringTransaction* record = context.recurring.getRecurringTransactionById(1);
    assert(record->getId() == 1);
    assert(record->getAmount() == 2000);
    assert(record->getDescription() == "Raise");

    // Editing keeps the last generated date, so old months are not generated again
    assert(record->hasLastGeneratedDate());
    assert(record->getLastGeneratedDate() == Date(5, 2, 2026));
    assert(context.recurring.generateDueTransactions(Date(5, 2, 2026),
                                                     context.transactions) == 0);

    // Invalid edits change nothing
    RecurringTransaction invalid(0, TransactionType::INCOME, 0, "", context.walletId,
                                 context.sourceId, Date(5, 1, 2026), 5);
    assert(!context.recurring.editRecurringTransaction(1, invalid));
    assert(!context.recurring.editRecurringTransaction(999, raise));
    assert(context.recurring.getRecurringTransactionById(1)->getAmount() == 2000);

    std::cout << "PASS: Edit recurring transaction\n";
}

void testViewAndDeleteRecurring() {
    RecurringTestContext context;

    assert(context.recurring.createRecurringTransaction(
        makeIncomeRecurring(context, Date(5, 1, 2026), 5)));

    assert(context.recurring.viewRecurringTransaction(1));
    assert(!context.recurring.viewRecurringTransaction(999));
    context.recurring.viewAllRecurringTransactions();

    assert(context.recurring.deleteRecurringTransaction(1));
    assert(context.recurring.getRecurringTransactionCount() == 0);
    assert(context.recurring.getRecurringTransactionById(1) == nullptr);
    assert(!context.recurring.deleteRecurringTransaction(1));

    std::cout << "PASS: View and delete recurring transaction\n";
}

int main() {
    std::cout << "===== RECURRING TRANSACTION TESTS =====\n\n";

    testCreateRecurring();
    testRejectInvalidRecurring();
    testGenerateLastDayOfMonth();
    testGenerateLeapYear();
    testPreventDuplicateGeneration();
    testStartDate();
    testEndDate();
    testInactiveRecurring();
    testGenerationRetryAfterFailure();
    testEditRecurring();
    testViewAndDeleteRecurring();

    std::cout << "\nAll recurring transaction tests passed!\n";

    return 0;
}
