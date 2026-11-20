/**
 * @file TransactionManager.h
 * @author TV3
 * @brief Interface for managing income and expense transactions
 * @note Sprint 2 supports adding, viewing and searching transactions.
 * Edit and delete operations are deferred to a later sprint.
  */

#ifndef TRANSACTION_MANAGER_H
#define TRANSACTION_MANAGER_H

#include "core/DynamicArray.h"
#include "models/Expense.h"
#include "models/Income.h"
#include "models/Transaction.h"

class WalletManager;
class IncomeSourceManager;
class ExpenseCategoryManager;

class TransactionManager {
public:
    // Constructors & Destructor
    TransactionManager(WalletManager* walletManager,
                       IncomeSourceManager* incomeSourceManager,
                       ExpenseCategoryManager* expenseCategoryManager);
    ~TransactionManager();

    // CRUD Operations
    bool addIncome(const Income& income);
    bool addExpense(const Expense& expense);
    void viewAllTransactions() const;
    bool viewTransaction(int id) const;
    bool editIncome(int id, const Income& updated);
    bool editExpense(int id, const Expense& updated);
    bool deleteTransaction(int id);

    // Search & Access
    const Transaction* getTransactionAt(int index) const;
    const Transaction* getTransactionById(int id) const;
    int getTransactionCount() const;

private:
    // Disable Copying
    TransactionManager(const TransactionManager&) = delete;
    TransactionManager& operator=(const TransactionManager&) = delete;

    // Private Helpers
    int findTransactionIndex(int id) const;

    // Private Members
    // Owns the allocated transactions
    DynamicArray<Transaction*> m_transactions;
    int m_nextId;
    WalletManager* m_walletManager;
    IncomeSourceManager* m_incomeSourceManager;
    ExpenseCategoryManager* m_expenseCategoryManager;
};

#endif
