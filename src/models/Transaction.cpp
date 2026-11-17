/**
 * @file Transaction.cpp
 * @author TV3
 * @brief Implements the base Transaction model
 * @note Week 2 - constructors, getters and setters
 */

#include "models/Transaction.h"

// Constructors & Destructor
Transaction::Transaction()
    : m_id(INVALID_ID),
      m_date(),
      m_amount(0),
      m_description(),
      m_walletId(INVALID_ID) {}

Transaction::Transaction(int id, const Date& date, long long amount,
                         const std::string& description, int walletId)
    : m_id(id),
      m_date(date),
      m_amount(amount),
      m_description(description),
      m_walletId(walletId) {}

Transaction::~Transaction() {}

// Getters
long long Transaction::getAmount() const {
    return m_amount;
}

Date Transaction::getDate() const {
    return m_date;
}

std::string Transaction::getDescription() const {
    return m_description;
}

int Transaction::getId() const {
    return m_id;
}

int Transaction::getWalletId() const {
    return m_walletId;
}

// Setters
void Transaction::setAmount(long long amount) {
    m_amount = amount;
}

void Transaction::setDate(const Date& date) {
    m_date = date;
}

void Transaction::setDescription(const std::string& description) {
    m_description = description;
}

void Transaction::setId(int id) {
    m_id = id;
}

void Transaction::setWalletId(int walletId) {
    m_walletId = walletId;
}