/**
 * @file RecurringTransaction.cpp
 * @author TV3
 * @brief Implements the monthly recurring transaction model
 * @note Week 3 - stores the schedule only; RecurringTransactionManager validates and generates
 */

#include "models/RecurringTransaction.h"

namespace {

// Used when a wallet / source / category is not chosen yet
const int NO_REFERENCE_ID = 0;

}  // namespace

// Constructors
RecurringTransaction::RecurringTransaction()
    : m_id(INVALID_ID),
      m_type(TransactionType::EXPENSE),
      m_amount(0),
      m_description(),
      m_walletId(NO_REFERENCE_ID),
      m_sourceOrCategoryId(NO_REFERENCE_ID),
      m_startDate(),
      m_hasEndDate(false),
      m_endDate(),
      m_dayOfMonth(MIN_DAY_OF_MONTH),
      m_hasLastGeneratedDate(false),
      m_lastGeneratedDate(),
      m_isActive(true) {}

RecurringTransaction::RecurringTransaction(int id, TransactionType type,
                                           long long amount,
                                           const std::string& description,
                                           int walletId, int sourceOrCategoryId,
                                           const Date& startDate, int dayOfMonth)
    : m_id(id),
      m_type(type),
      m_amount(amount),
      m_description(description),
      m_walletId(walletId),
      m_sourceOrCategoryId(sourceOrCategoryId),
      m_startDate(startDate),
      m_hasEndDate(false),
      m_endDate(startDate),
      m_dayOfMonth(dayOfMonth),
      m_hasLastGeneratedDate(false),
      m_lastGeneratedDate(startDate),
      m_isActive(true) {}

// Getters
long long RecurringTransaction::getAmount() const {
    return m_amount;
}

int RecurringTransaction::getDayOfMonth() const {
    return m_dayOfMonth;
}

std::string RecurringTransaction::getDescription() const {
    return m_description;
}

Date RecurringTransaction::getEndDate() const {
    return m_endDate;
}

int RecurringTransaction::getId() const {
    return m_id;
}

Date RecurringTransaction::getLastGeneratedDate() const {
    return m_lastGeneratedDate;
}

int RecurringTransaction::getSourceOrCategoryId() const {
    return m_sourceOrCategoryId;
}

Date RecurringTransaction::getStartDate() const {
    return m_startDate;
}

TransactionType RecurringTransaction::getType() const {
    return m_type;
}

int RecurringTransaction::getWalletId() const {
    return m_walletId;
}

bool RecurringTransaction::hasEndDate() const {
    return m_hasEndDate;
}

bool RecurringTransaction::hasLastGeneratedDate() const {
    return m_hasLastGeneratedDate;
}

bool RecurringTransaction::isActive() const {
    return m_isActive;
}

// Setters
void RecurringTransaction::setActive(bool isActive) {
    m_isActive = isActive;
}

void RecurringTransaction::setAmount(long long amount) {
    m_amount = amount;
}

void RecurringTransaction::setDayOfMonth(int dayOfMonth) {
    m_dayOfMonth = dayOfMonth;
}

void RecurringTransaction::setDescription(const std::string& description) {
    m_description = description;
}

// Setting an end date also turns the "has end date" flag on
void RecurringTransaction::setEndDate(const Date& endDate) {
    m_endDate = endDate;
    m_hasEndDate = true;
}

void RecurringTransaction::setId(int id) {
    m_id = id;
}

// Remembers the last generated date so the same month is never generated twice
void RecurringTransaction::setLastGeneratedDate(const Date& date) {
    m_lastGeneratedDate = date;
    m_hasLastGeneratedDate = true;
}

void RecurringTransaction::setSourceOrCategoryId(int sourceOrCategoryId) {
    m_sourceOrCategoryId = sourceOrCategoryId;
}

void RecurringTransaction::setStartDate(const Date& startDate) {
    m_startDate = startDate;
}

void RecurringTransaction::setWalletId(int walletId) {
    m_walletId = walletId;
}

// Date Operations
// Removes the end date, so the schedule runs without a limit
void RecurringTransaction::clearEndDate() {
    m_hasEndDate = false;
    m_endDate = m_startDate;
}
