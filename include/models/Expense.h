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
    // Constructors & Destructor
    Expense();
    Expense(int id, const Date& date, long long amount,
            const std::string& description, int walletId, int expenseCategoryId);
    ~Expense() override;

    // Getters
    int getExpenseCategoryId() const;
    TransactionType getType() const override;

    // Setters
    void setExpenseCategoryId(int expenseCategoryId);

private:
    // Private Members
    int m_expenseCategoryId;
};

#endif
