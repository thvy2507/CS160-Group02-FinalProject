/**
 * @file Expense.h
 * @author TV3
 * @brief Expense transaction linked to an expense category
 * @note Week 1 - interface only
 */

#ifndef EXPENSE_H
#define EXPENSE_H

#include "models/Transaction.h"

class Expense : public Transaction {
public:
    Expense();
    Expense(int id, const Date& date, double amount,
            const std::string& description, int walletId, int expenseCategoryId);
    ~Expense() override;

    int getExpenseCategoryId() const;
    TransactionType getType() const override;
    void setExpenseCategoryId(int expenseCategoryId);

private:
    int m_expenseCategoryId;
};

#endif