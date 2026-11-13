/**
 * @file WalletManager.h
 * @author TV2 (Khoi)
 * @brief Manages wallets and their balances.
 */

#ifndef MANAGERS_WALLET_MANAGER_H
#define MANAGERS_WALLET_MANAGER_H

#include <string>
#include "core/DynamicArray.h"
#include "models/Wallet.h"

class WalletManager {
public:
    // Constructors & Destructor
    WalletManager();
    ~WalletManager() = default;

    // CRUD Operations
    int addWallet(const std::string& name, Money initialBalance = 0);
    bool editWalletName(int walletId, const std::string& newName);
    bool deleteWallet(int walletId);
    void viewAllWallets() const;

    // Search & Access
    bool exists(int walletId) const;
    int findIndexById(int walletId) const;
    int findIndexByName(const std::string& name) const;

    Wallet* getWalletById(int walletId);
    const Wallet* getWalletById(int walletId) const;
    std::string getWalletName(int walletId) const;

    const DynamicArray<Wallet>& getAllWallets() const;
    int getActiveWalletCount() const;

    Money getWalletBalance(int walletId) const;
    Money getTotalBalance() const;

    bool canWithdraw(int walletId, Money amount, bool allowNegative = false) const;
    bool deposit(int walletId, Money amount);
    bool withdraw(int walletId, Money amount, bool allowNegative = false);
    bool adjustBalance(int walletId, Money delta);

    // Binary Persistence
    bool loadFromBinary(const std::string& recordFilePath, const std::string& namePoolPath);
    bool saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const;

private:
    // Private Members
    DynamicArray<Wallet> m_wallets;
    int m_nextId;
};

#endif // MANAGERS_WALLET_MANAGER_H
