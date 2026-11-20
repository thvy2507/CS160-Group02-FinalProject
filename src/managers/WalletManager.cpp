#include "managers/WalletManager.h"
#include <iomanip>
#include <iostream>

WalletManager::WalletManager() : wallets_(), nextId_(1) {}

int WalletManager::addWallet(const std::string& name, Money initialBalance) {
    if (name.empty() || initialBalance < 0 || findIndexByName(name) != -1) {
        return Wallet::INVALID_ID;
    }
    int assignedId = nextId_++;
    Wallet newWallet(assignedId, name, initialBalance);
    wallets_.add(newWallet);
    return assignedId;
}

bool WalletManager::editWalletName(int walletId, const std::string& newName) {
    if (newName.empty()) return false;
    int targetIdx = findIndexById(walletId);
    if (targetIdx == -1) return false;
    int duplicateIdx = findIndexByName(newName);
    if (duplicateIdx != -1 && duplicateIdx != targetIdx) return false;
    return wallets_[targetIdx].setName(newName);
}

bool WalletManager::deleteWallet(int walletId) {
    int targetIdx = findIndexById(walletId);
    if (targetIdx == -1) return false;
    wallets_[targetIdx].setDeleted(true);
    return true;
}

void WalletManager::viewAllWallets() const {
    std::cout << "\n=== DANH SACH VI ===\n";
    int activeCount = 0;
    for (int i = 0; i < wallets_.getSize(); ++i) {
        if (wallets_[i].isDeleted()) continue;
        ++activeCount;
        std::cout << "ID: " << wallets_[i].getId()
                  << " | Ten vi: " << wallets_[i].getName()
                  << " | So du: " << wallets_[i].getBalance()
                  << " | Ban dau: " << wallets_[i].getInitialBalance() << "\n";
    }
    if (activeCount == 0) std::cout << "(Chua co vi nao dang hoat dong)\n";
    std::cout << "Tong so du tat ca vi: " << getTotalBalance() << "\n";
}

bool WalletManager::exists(int walletId) const { return findIndexById(walletId) != -1; }

int WalletManager::findIndexById(int walletId) const {
    if (walletId <= Wallet::INVALID_ID) return -1;
    for (int i = 0; i < wallets_.getSize(); ++i) {
        if (!wallets_[i].isDeleted() && wallets_[i].getId() == walletId) return i;
    }
    return -1;
}

int WalletManager::findIndexByName(const std::string& name) const {
    if (name.empty()) return -1;
    for (int i = 0; i < wallets_.getSize(); ++i) {
        if (!wallets_[i].isDeleted() && wallets_[i].getName() == name) return i;
    }
    return -1;
}

Wallet* WalletManager::getWalletById(int walletId) {
    int idx = findIndexById(walletId);
    return (idx != -1) ? &wallets_[idx] : nullptr;
}

const Wallet* WalletManager::getWalletById(int walletId) const {
    int idx = findIndexById(walletId);
    return (idx != -1) ? &wallets_[idx] : nullptr;
}

std::string WalletManager::getWalletName(int walletId) const {
    const Wallet* wallet = getWalletById(walletId);
    return (wallet != nullptr) ? wallet->getName() : "";
}

const DynamicArray<Wallet>& WalletManager::getAllWallets() const { return wallets_; }

int WalletManager::getActiveWalletCount() const {
    int count = 0;
    for (int i = 0; i < wallets_.getSize(); ++i) {
        if (!wallets_[i].isDeleted()) ++count;
    }
    return count;
}

Money WalletManager::getWalletBalance(int walletId) const {
    const Wallet* wallet = getWalletById(walletId);
    return (wallet != nullptr) ? wallet->getBalance() : 0;
}

Money WalletManager::getTotalBalance() const {
    Money total = 0;
    for (int i = 0; i < wallets_.getSize(); ++i) {
        if (!wallets_[i].isDeleted()) total += wallets_[i].getBalance();
    }
    return total;
}

bool WalletManager::canWithdraw(int walletId, Money amount, bool allowNegative) const {
    const Wallet* wallet = getWalletById(walletId);
    if (wallet == nullptr) return false;
    return wallet->canWithdraw(amount, allowNegative);
}

bool WalletManager::deposit(int walletId, Money amount) {
    Wallet* wallet = getWalletById(walletId);
    if (wallet == nullptr) return false;
    return wallet->deposit(amount);
}

bool WalletManager::withdraw(int walletId, Money amount, bool allowNegative) {
    Wallet* wallet = getWalletById(walletId);
    if (wallet == nullptr) return false;
    return wallet->withdraw(amount, allowNegative);
}

bool WalletManager::adjustBalance(int walletId, Money delta) {
    Wallet* wallet = getWalletById(walletId);
    if (wallet == nullptr) return false;
    wallet->adjustBalance(delta);
    return true;
}

bool WalletManager::loadFromBinary(const std::string& recordFilePath, const std::string& namePoolPath) {
    (void)recordFilePath; (void)namePoolPath; return false;
}
bool WalletManager::saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const {
    (void)recordFilePath; (void)namePoolPath; return false;
}
