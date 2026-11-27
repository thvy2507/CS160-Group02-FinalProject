/**
 * @file RecurringTransactionManager.h
 * @author TV3
 * @brief Interface for managing monthly recurring transactions
 * @note Sprint 3 - supports in-memory recurring transaction management
 * and monthly generation through TransactionManager.
 * Binary persistence is deferred to a later sprint.
 */

#ifndef RECURRING_TRANSACTION_MANAGER_H
#define RECURRING_TRANSACTION_MANAGER_H

#include "core/Date.h"
#include "core/DynamicArray.h"
#include "models/RecurringTransaction.h"

class WalletManager;
class IncomeSourceManager;
class ExpenseCategoryManager;
class TransactionManager;

class RecurringTransactionManager {
public:
    // Constructors & Destructor
    RecurringTransactionManager(WalletManager* walletManager,
                                IncomeSourceManager* incomeSourceManager,
                                ExpenseCategoryManager* expenseCategoryManager);
    ~RecurringTransactionManager();

    // CRUD Operations
    bool createRecurringTransaction(const RecurringTransaction& recurring);
    void viewAllRecurringTransactions() const;
    bool viewRecurringTransaction(int id) const;
    bool editRecurringTransaction(int id, const RecurringTransaction& updated);
    bool deleteRecurringTransaction(int id);

    // Search & Access
    const RecurringTransaction* getRecurringTransactionById(int id) const;
    int getRecurringTransactionCount() const;

    // Domain-specific Operations
    // Returns false if any due transaction cannot be generated.
    bool generateMonthlyTransactions(const Date& asOfDate,
                                TransactionManager& transactionManager);

private:
    // Private Helpers
    int findRecurringIndex(int id) const;

    // Private Members
    DynamicArray<RecurringTransaction> m_recurringTransactions;
    int m_nextId;

    // Non-owning Manager Pointers
    WalletManager* m_walletManager;
    IncomeSourceManager* m_incomeSourceManager;
    ExpenseCategoryManager* m_expenseCategoryManager;
};

#endif
