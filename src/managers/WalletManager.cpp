/**
 * @file WalletManager.cpp
 * @author TV2 (Khoi)
 * @brief Implements wallet management and balance operations.
 */

#include "managers/WalletManager.h"

#include <iostream>
#include <limits>

WalletManager::WalletManager() : m_wallets(), m_nextId(1) {}

int WalletManager::addWallet(const std::string& name, Money initialBalance) {
    if (name.empty() || initialBalance < 0 || findIndexByName(name) != -1) {
        return Wallet::INVALID_ID;
    }

    if (getTotalBalance() > std::numeric_limits<Money>::max() - initialBalance) {
        return Wallet::INVALID_ID;
    }

    int assignedId = m_nextId++;
    Wallet newWallet(assignedId, name, initialBalance);
    m_wallets.add(newWallet);

    return assignedId;
}

void WalletManager::viewAllWallets() const {
    std::cout << "\n===== WALLET LIST =====\n";

    int activeCount = 0;

    for (int i = 0; i < m_wallets.getSize(); ++i) {
        if (m_wallets[i].isDeleted()) continue;
        ++activeCount;

        std::cout << "ID: " << m_wallets[i].getId()
                  << " | Name: " << m_wallets[i].getName()
                  << " | Balance: " << m_wallets[i].getBalance()
                  << " | Initial balance: " << m_wallets[i].getInitialBalance()
                  << "\n";
    }

    if (activeCount == 0) std::cout << "No active wallets found.\n";

    std::cout << "Total balance: " << getTotalBalance() << "\n";
}

bool WalletManager::editWalletName(int walletId, const std::string& newName) {
    if (newName.empty()) return false;

    int targetIdx = findIndexById(walletId);
    if (targetIdx == -1) return false;

    int duplicateIdx = findIndexByName(newName);
    if (duplicateIdx != -1 && duplicateIdx != targetIdx) return false;

    return m_wallets[targetIdx].setName(newName);
}

bool WalletManager::deleteWallet(int walletId) {
    int targetIdx = findIndexById(walletId);
    if (targetIdx == -1) return false;

    m_wallets[targetIdx].setDeleted(true);
    return true;
}

bool WalletManager::exists(int walletId) const { return findIndexById(walletId) != -1; }

int WalletManager::findIndexById(int walletId) const {
    if (walletId <= Wallet::INVALID_ID) return -1;

    for (int i = 0; i < m_wallets.getSize(); ++i) {
        if (!m_wallets[i].isDeleted() && m_wallets[i].getId() == walletId) return i;
    }

    return -1;
}

int WalletManager::findIndexByName(const std::string& name) const {
    if (name.empty()) return -1;

    for (int i = 0; i < m_wallets.getSize(); ++i) {
        if (!m_wallets[i].isDeleted() && m_wallets[i].getName() == name) return i;
    }

    return -1;
}

Wallet* WalletManager::getWalletById(int walletId) {
    int idx = findIndexById(walletId);

    return (idx != -1) ? &m_wallets[idx] : nullptr;
}

const Wallet* WalletManager::getWalletById(int walletId) const {
    int idx = findIndexById(walletId);

    return (idx != -1) ? &m_wallets[idx] : nullptr;
}

std::string WalletManager::getWalletName(int walletId) const {
    const Wallet* wallet = getWalletById(walletId);

    return (wallet != nullptr) ? wallet->getName() : "";
}

const DynamicArray<Wallet>& WalletManager::getAllWallets() const { return m_wallets; }

int WalletManager::getActiveWalletCount() const {
    int count = 0;
    for (int i = 0; i < m_wallets.getSize(); ++i) {
        if (!m_wallets[i].isDeleted()) ++count;
    }

    return count;
}

Money WalletManager::getWalletBalance(int walletId) const {
    const Wallet* wallet = getWalletById(walletId);

    return (wallet != nullptr) ? wallet->getBalance() : 0;
}

Money WalletManager::getTotalBalance() const {
    Money total = 0;
    for (int i = 0; i < m_wallets.getSize(); ++i) {
        if (!m_wallets[i].isDeleted()) total += m_wallets[i].getBalance();
    }

    return total;
}

bool WalletManager::deposit(int walletId, Money amount) {
    Wallet* wallet = getWalletById(walletId);

    if (!wallet || amount <= 0) {
        return false;
    }

    if (getTotalBalance() > std::numeric_limits<Money>::max() - amount) {
        return false;
    }

    return wallet->deposit(amount);
}

bool WalletManager::canWithdraw(int walletId, Money amount, bool allowNegative) const {
    const Wallet* wallet = getWalletById(walletId);
    if (wallet == nullptr) return false;

    return wallet->canWithdraw(amount, allowNegative);
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
