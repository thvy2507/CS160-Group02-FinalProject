#include "models/Wallet.h"

Wallet::Wallet()
    : id_(INVALID_ID),
      name_(""),
      initialBalance_(0),
      balance_(0),
      isDeleted_(false) {}

Wallet::Wallet(int id, const std::string& name, Money initialBalance)
    : id_(id),
      name_(name),
      initialBalance_(initialBalance),
      balance_(initialBalance),
      isDeleted_(false) {}

Wallet::Wallet(int id, const std::string& name, Money initialBalance, Money currentBalance, bool isDeleted)
    : id_(id),
      name_(name),
      initialBalance_(initialBalance),
      balance_(currentBalance),
      isDeleted_(isDeleted) {}

int Wallet::getId() const { return id_; }
const std::string& Wallet::getName() const { return name_; }
Money Wallet::getInitialBalance() const { return initialBalance_; }
Money Wallet::getBalance() const { return balance_; }
bool Wallet::isDeleted() const { return isDeleted_; }
bool Wallet::isValid() const { return (id_ > INVALID_ID) && (!isDeleted_) && (!name_.empty()); }

bool Wallet::setName(const std::string& name) {
    if (name.empty()) return false;
    name_ = name;
    return true;
}

void Wallet::setInitialBalance(Money initialBalance) { initialBalance_ = initialBalance; }
void Wallet::setBalance(Money balance) { balance_ = balance; }
void Wallet::setDeleted(bool deleted) { isDeleted_ = deleted; }

bool Wallet::canWithdraw(Money amount, bool allowNegative) const {
    if (amount <= 0 || isDeleted_) return false;
    if (!allowNegative && balance_ < amount) return false;
    return true;
}

bool Wallet::deposit(Money amount) {
    if (amount <= 0 || isDeleted_) return false;
    balance_ += amount;
    return true;
}

bool Wallet::withdraw(Money amount, bool allowNegative) {
    if (!canWithdraw(amount, allowNegative)) return false;
    balance_ -= amount;
    return true;
}

void Wallet::adjustBalance(Money delta) {
    if (isDeleted_) return;
    balance_ += delta;
}

WalletRecord Wallet::toRecord(uint32_t nameOffset, uint32_t nameLength) const {
    WalletRecord record{};
    record.id = static_cast<int32_t>(id_);
    record.nameOffset = nameOffset;
    record.nameLength = nameLength;
    record.initialBalance = initialBalance_;
    record.balance = balance_;
    record.isDeleted = static_cast<uint8_t>(isDeleted_ ? 1 : 0);
    return record;
}

Wallet Wallet::fromRecord(const WalletRecord& record, const std::string& resolvedName) {
    return Wallet(static_cast<int>(record.id), resolvedName, record.initialBalance, record.balance, record.isDeleted != 0);
}
