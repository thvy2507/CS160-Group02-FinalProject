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
    static constexpr int INVALID_ID = 0;

    Wallet();
    Wallet(int id, const std::string& name, Money initialBalance = 0);
    Wallet(int id, const std::string& name, Money initialBalance, Money currentBalance, bool isDeleted = false);

    int getId() const;
    const std::string& getName() const;
    Money getInitialBalance() const;
    Money getBalance() const;
    bool isDeleted() const;
    bool isValid() const;

    bool setName(const std::string& name);
    void setInitialBalance(Money initialBalance);
    void setBalance(Money balance);
    void setDeleted(bool deleted);

    bool canWithdraw(Money amount, bool allowNegative = false) const;
    bool deposit(Money amount);
    bool withdraw(Money amount, bool allowNegative = false);
    void adjustBalance(Money delta);

    WalletRecord toRecord(uint32_t nameOffset, uint32_t nameLength) const;
    static Wallet fromRecord(const WalletRecord& record, const std::string& resolvedName);

private:
    int id_;
    std::string name_;
    Money initialBalance_;
    Money balance_;
    bool isDeleted_;
};

#endif // MODELS_WALLET_H
