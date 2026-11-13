/**
 * @file RecurringTransactionManager.h
 * @author TV3
 * @brief Interface for managing monthly recurring transactions
 * @note Week 1 - declarations only. Generation is planned for a later week
 */

#ifndef RECURRING_TRANSACTION_MANAGER_H
#define RECURRING_TRANSACTION_MANAGER_H

#include "core/DynamicArray.h"
#include "models/RecurringTransaction.h"

class WalletManager;
class IncomeSourceManager;
class ExpenseCategoryManager;

class RecurringTransactionManager {
public:
    RecurringTransactionManager(WalletManager* walletManager,
                                IncomeSourceManager* incomeSourceManager,
                                ExpenseCategoryManager* expenseCategoryManager);
    ~RecurringTransactionManager();

    bool createRecurringTransaction(const RecurringTransaction& recurring);
    bool deleteRecurringTransaction(int id);
    bool editRecurringTransaction(int id, const RecurringTransaction& updated);
    void viewAllRecurringTransactions() const;
    bool viewRecurringTransaction(int id) const;

    const RecurringTransaction* getRecurringTransactionById(int id) const;
    int getRecurringTransactionCount() const;

private:
    int findRecurringIndex(int id) const;

    DynamicArray<RecurringTransaction> m_recurringTransactions;
    int m_nextId;
    WalletManager* m_walletManager;
    IncomeSourceManager* m_incomeSourceManager;
    ExpenseCategoryManager* m_expenseCategoryManager;
};

#endif