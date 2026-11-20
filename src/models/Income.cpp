/**
 * @file Income.cpp
 * @author TV3
 * @brief Implements the Income transaction linked to an income source
 * @note Week 2 - constructors, getters, setters and getType()
 */

#include "models/Income.h"

// Constructors & Destructor
Income::Income()
    : Transaction(),
      m_incomeSourceId(INVALID_ID) {}

Income::Income(int id, const Date& date, long long amount,
               const std::string& description, int walletId, int incomeSourceId)
    : Transaction(id, date, amount, description, walletId),
      m_incomeSourceId(incomeSourceId) {}

Income::~Income() {}

// Getters
int Income::getIncomeSourceId() const {
    return m_incomeSourceId;
}

TransactionType Income::getType() const {
    return TransactionType::INCOME;
}

// Setters
void Income::setIncomeSourceId(int incomeSourceId) {
    m_incomeSourceId = incomeSourceId;
}