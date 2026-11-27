/**
 * @file FinanceManager.h
 * @author TV1 (Vy)
 * @brief Declares the main application controller.
 */

#pragma once

#include <string>

#include "core/Date.h"
#include "managers/ExpenseCategoryManager.h"
#include "managers/IncomeSourceManager.h"
#include "managers/RecurringTransactionManager.h"
#include "managers/TransactionManager.h"
#include "managers/WalletManager.h"

class FinanceManager {
public:
    // Constructors
    FinanceManager();

    // Application Control
    void run();

private:
    // Input Helpers
    bool readInt(const std::string& prompt, int& value) const;
    bool readMoney(const std::string& prompt, long long& amount) const;
    bool readText(const std::string& prompt, std::string& value, bool allowEmpty = false) const;
    bool readDate(const std::string& prompt, Date& date) const;
    bool readRecurringReferences(TransactionType type, int& walletId, int& sourceOrCategoryId) const;
    bool readRecurringDetails(TransactionType type, RecurringTransaction& recurring) const;

    // Menu & Navigation
    void displayMenu() const;
    bool handleChoice(int choice);

    void runIncomeMenu();
    void runIncomeSourceMenu();

    void runExpenseMenu();
    void runExpenseCategoryMenu();

    void runStatisticsMenu() const;

    void runWalletMenu();
    void runRecurringMenu();

    // Transaction Operations
    bool addIncome();
    bool addExpense();
    void viewTransactions(TransactionType type) const;

    // Recurring Transaction Operations
    bool createRecurringTransaction();

    // Private Members
    WalletManager m_walletManager;
    IncomeSourceManager m_incomeSourceManager;
    ExpenseCategoryManager m_expenseCategoryManager;
    TransactionManager m_transactionManager;
    RecurringTransactionManager m_recurringTransactionManager;
};
