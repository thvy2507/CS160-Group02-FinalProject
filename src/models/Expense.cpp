/**
 * @file Expense.cpp
 * @author TV3
 * @brief Implements the Expense transaction linked to an expense category
 * @note Week 2 - constructors, getters, setters and getType()
 */

#include "models/Expense.h"

// Constructors & Destructor
Expense::Expense()
    : Transaction(),
      m_expenseCategoryId(INVALID_ID) {}

Expense::Expense(int id, const Date& date, long long amount,
                 const std::string& description, int walletId, int expenseCategoryId)
    : Transaction(id, date, amount, description, walletId),
      m_expenseCategoryId(expenseCategoryId) {}

Expense::~Expense() {}

// Getters
int Expense::getExpenseCategoryId() const {
    return m_expenseCategoryId;
}

TransactionType Expense::getType() const {
    return TransactionType::EXPENSE;
}

// Setters
void Expense::setExpenseCategoryId(int expenseCategoryId) {
    m_expenseCategoryId = expenseCategoryId;
}