/**
 * @file Wallet.h
 * @author TV2 (Khoi)
 * @brief Declares the wallet model and binary record structure.
 */

#ifndef MODELS_WALLET_H
#define MODELS_WALLET_H

#include <cstdint>
#include <string>
#include <type_traits>

using Money = int64_t;

struct WalletRecord {
    int32_t id;
    uint32_t nameOffset;
    uint32_t nameLength;
    Money initialBalance;
    Money balance;
    uint8_t isDeleted;
};

static_assert(std::is_trivially_copyable_v<WalletRecord>,
              "WalletRecord must be trivially copyable for binary file I/O.");

class Wallet {
public:
    // Constants
    static constexpr int INVALID_ID = 0;

    // Constructors
    Wallet();
    Wallet(int id, const std::string& name, Money initialBalance = 0);
    Wallet(int id, const std::string& name, Money initialBalance, Money currentBalance, bool isDeleted = false);

    // Validation
    bool isValid() const;

    // Getters
    int getId() const;
    const std::string& getName() const;
    Money getInitialBalance() const;
    Money getBalance() const;
    bool isDeleted() const;

    // Setters
    bool setName(const std::string& name);
    void setInitialBalance(Money initialBalance);
    void setBalance(Money balance);
    void setDeleted(bool deleted);

    // Balance Operations
    bool deposit(Money amount);
    bool canWithdraw(Money amount, bool allowNegative = false) const;
    bool withdraw(Money amount, bool allowNegative = false);
    void adjustBalance(Money delta);

    // Binary Record Conversion
    WalletRecord toRecord(uint32_t nameOffset, uint32_t nameLength) const;
    static Wallet fromRecord(const WalletRecord& record, const std::string& resolvedName);

private:
    // Private Members
    int m_id;
    std::string m_name;
    Money m_initialBalance;
    Money m_balance;
    bool m_isDeleted;
};

#endif // MODELS_WALLET_H
