/**
 * @file Wallet.h
 * @author TV2 (Khoi)
 * @brief Defines the Wallet model and its binary storage record.
 */

#ifndef MODELS_WALLET_H
#define MODELS_WALLET_H

#include <cstdint>
#include <string>
#include <type_traits>

// Money is stored as an integer in VND.
using Money = long long;

// Fixed-size record for binary storage.
// Wallet names are stored separately in the Name Pool.
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

    // Getters
    int getId() const;
    const std::string& getName() const;
    Money getInitialBalance() const;
    Money getBalance() const;
    bool isDeleted() const;
    bool isValid() const;

    // Setters
    void setName(const std::string& name);
    void setInitialBalance(Money initialBalance);
    void setBalance(Money balance);
    void setDeleted(bool deleted);

    // Balance Operations
    bool canWithdraw(Money amount, bool allowNegative = false) const;
    bool deposit(Money amount);
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
