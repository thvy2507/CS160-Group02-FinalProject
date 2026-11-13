/**
 * @file TransactionManager.h
 * @author TV3
 * @brief Interface for managing income and expense transactions
 * @note Week 1 - declarations only. Validation and balance updates go through WalletManager
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
    TransactionManager(WalletManager* walletManager,
                       IncomeSourceManager* incomeSourceManager,
                       ExpenseCategoryManager* expenseCategoryManager);
    ~TransactionManager();

    bool addExpense(const Expense& expense);
    bool addIncome(const Income& income);
    bool deleteTransaction(int id);
    bool editExpense(int id, const Expense& updated);
    bool editIncome(int id, const Income& updated);
    void viewAllTransactions() const;
    bool viewTransaction(int id) const;

    const Transaction* getTransactionAt(int index) const;
    const Transaction* getTransactionById(int id) const;
    int getTransactionCount() const;

private:
    TransactionManager(const TransactionManager&) = delete;
    TransactionManager& operator=(const TransactionManager&) = delete;

    int findTransactionIndex(int id) const;

    DynamicArray<Transaction*> m_transactions;
    int m_nextId;
    WalletManager* m_walletManager;
    IncomeSourceManager* m_incomeSourceManager;
    ExpenseCategoryManager* m_expenseCategoryManager;
};

#endif