#include "managers/WalletManager.h"
#include "core/BinaryIO.h"
#include <iostream>
#include <vector>

WalletManager::WalletManager() : wallets_(), nextId_(1) {}

int WalletManager::addWallet(const std::string& name, Money initialBalance) {
    if (name.empty() || initialBalance < 0 || findIndexByName(name) != -1) return Wallet::INVALID_ID;
    int assignedId = nextId_++;
    wallets_.pushBack(Wallet(assignedId, name, initialBalance));
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
    for (int i = 0; i < wallets_.getSize(); ++i) {
        if (!wallets_[i].isDeleted()) {
            std::cout << "ID: " << wallets_[i].getId() << " | Name: " << wallets_[i].getName() << "\n";
        }
    }
}
bool WalletManager::exists(int walletId) const { return findIndexById(walletId) != -1; }
int WalletManager::findIndexById(int walletId) const {
    for (int i = 0; i < wallets_.getSize(); ++i) {
        if (!wallets_[i].isDeleted() && wallets_[i].getId() == walletId) return i;
    }
    return -1;
}
int WalletManager::findIndexByName(const std::string& name) const {
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
    const Wallet* w = getWalletById(walletId);
    return w ? w->getName() : "";
}
const DynamicArray<Wallet>& WalletManager::getAllWallets() const { return wallets_; }
int WalletManager::getActiveWalletCount() const {
    int c = 0;
    for (int i = 0; i < wallets_.getSize(); ++i) if (!wallets_[i].isDeleted()) c++;
    return c;
}
Money WalletManager::getWalletBalance(int walletId) const {
    const Wallet* w = getWalletById(walletId);
    return w ? w->getBalance() : 0;
}
Money WalletManager::getTotalBalance() const {
    Money t = 0;
    for (int i = 0; i < wallets_.getSize(); ++i) if (!wallets_[i].isDeleted()) t += wallets_[i].getBalance();
    return t;
}
bool WalletManager::canWithdraw(int walletId, Money amount, bool allowNegative) const {
    const Wallet* w = getWalletById(walletId);
    return w ? w->canWithdraw(amount, allowNegative) : false;
}
bool WalletManager::deposit(int walletId, Money amount) {
    Wallet* w = getWalletById(walletId);
    return w ? w->deposit(amount) : false;
}
bool WalletManager::withdraw(int walletId, Money amount, bool allowNegative) {
    Wallet* w = getWalletById(walletId);
    return w ? w->withdraw(amount, allowNegative) : false;
}
bool WalletManager::adjustBalance(int walletId, Money delta) {
    Wallet* w = getWalletById(walletId);
    if (!w) return false;
    w->adjustBalance(delta);
    return true;
}

bool WalletManager::saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const {
    std::string namePool = "";
    std::vector<WalletRecord> records;
    for (int i = 0; i < wallets_.getSize(); ++i) {
        const Wallet& w = wallets_[i];
        uint32_t offset = static_cast<uint32_t>(namePool.size());
        uint32_t len = static_cast<uint32_t>(w.getName().size());
        namePool += w.getName();
        records.push_back(w.toRecord(offset, len));
    }
    
    if (!records.empty()) {
        if (!BinaryIO::writeBuffer(recordFilePath, records.data(), records.size() * sizeof(WalletRecord))) return false;
    } else {
        BinaryIO::writeBuffer(recordFilePath, "", 0);
    }
    
    if (!namePool.empty()) {
        if (!BinaryIO::writeBuffer(namePoolPath, namePool.data(), namePool.size())) return false;
    } else {
        BinaryIO::writeBuffer(namePoolPath, "", 0);
    }
    return true;
}

bool WalletManager::loadFromBinary(const std::string& recordFilePath, const std::string& namePoolPath) {
    if (!BinaryIO::fileExists(recordFilePath) || !BinaryIO::fileExists(namePoolPath)) return true; // file moi tao, ko loi
    
    size_t recordSize = BinaryIO::getFileSize(recordFilePath);
    size_t poolSize = BinaryIO::getFileSize(namePoolPath);
    
    if (recordSize % sizeof(WalletRecord) != 0) return false; // File bi hong
    
    std::vector<WalletRecord> records(recordSize / sizeof(WalletRecord));
    std::string pool(poolSize, '\0');
    
    if (recordSize > 0 && !BinaryIO::readBuffer(recordFilePath, records.data(), recordSize)) return false;
    if (poolSize > 0 && !BinaryIO::readBuffer(namePoolPath, pool.data(), poolSize)) return false;

    DynamicArray<Wallet> tempWallets;
    int maxId = 0;
    
    for (const auto& rec : records) {
        if (rec.nameOffset + rec.nameLength > poolSize) return false; // Name pool vuot bien
        std::string name = pool.substr(rec.nameOffset, rec.nameLength);
        tempWallets.pushBack(Wallet::fromRecord(rec, name));
        if (rec.id > maxId) maxId = rec.id;
    }
    
    wallets_ = tempWallets;
    nextId_ = maxId + 1;
    return true;
}
