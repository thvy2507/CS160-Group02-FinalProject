/**
 * @file Wallet.cpp
 * @author TV2 (Khoi)
 * @brief Implements wallet operations and balance management.
 */

#include "models/Wallet.h"

Wallet::Wallet()
    : m_id(INVALID_ID),
      m_name(""),
      m_initialBalance(0),
      m_balance(0),
      m_isDeleted(false) {}

Wallet::Wallet(int id, const std::string& name, Money initialBalance)
    : m_id(id),
      m_name(name),
      m_initialBalance(initialBalance),
      m_balance(initialBalance),
      m_isDeleted(false) {}

Wallet::Wallet(int id, const std::string& name, Money initialBalance, Money currentBalance, bool isDeleted)
    : m_id(id),
      m_name(name),
      m_initialBalance(initialBalance),
      m_balance(currentBalance),
      m_isDeleted(isDeleted) {}

bool Wallet::isValid() const { return (m_id > INVALID_ID) && (!m_isDeleted) && (!m_name.empty()); }


int Wallet::getId() const { return m_id; }

const std::string& Wallet::getName() const { return m_name; }

Money Wallet::getInitialBalance() const { return m_initialBalance; }

Money Wallet::getBalance() const { return m_balance; }

bool Wallet::isDeleted() const { return m_isDeleted; }

bool Wallet::setName(const std::string& name) {
    if (name.empty()) return false;

    m_name = name;
    return true;
}

void Wallet::setInitialBalance(Money initialBalance) { m_initialBalance = initialBalance; }

void Wallet::setBalance(Money balance) { m_balance = balance; }

void Wallet::setDeleted(bool deleted) { m_isDeleted = deleted; }

bool Wallet::deposit(Money amount) {
    if (amount <= 0 || m_isDeleted) return false;

    m_balance += amount;
    return true;
}

bool Wallet::canWithdraw(Money amount, bool allowNegative) const {
    if (amount <= 0 || m_isDeleted) return false;
    if (!allowNegative && m_balance < amount) return false;

    return true;
}

bool Wallet::withdraw(Money amount, bool allowNegative) {
    if (!canWithdraw(amount, allowNegative)) return false;

    m_balance -= amount;
    return true;
}

void Wallet::adjustBalance(Money delta) {
    if (m_isDeleted) return;

    m_balance += delta;
}

WalletRecord Wallet::toRecord(uint32_t nameOffset, uint32_t nameLength) const {
    WalletRecord record{};
    record.id = static_cast<int32_t>(m_id);
    record.nameOffset = nameOffset;
    record.nameLength = nameLength;
    record.initialBalance = m_initialBalance;
    record.balance = m_balance;
    record.isDeleted = static_cast<uint8_t>(m_isDeleted ? 1 : 0);

    return record;
}

Wallet Wallet::fromRecord(const WalletRecord& record, const std::string& resolvedName) {
    return Wallet(static_cast<int>(record.id), resolvedName, record.initialBalance, record.balance, record.isDeleted != 0);
}
