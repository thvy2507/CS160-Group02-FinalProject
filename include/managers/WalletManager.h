#ifndef MANAGERS_WALLET_MANAGER_H
#define MANAGERS_WALLET_MANAGER_H

#include <string>
#include "core/DynamicArray.h"
#include "models/Wallet.h"

class WalletManager {
public:
    WalletManager();
    ~WalletManager() = default;

    // --- Thao tac CRUD chinh ---
    // Tao vi moi, tra ve Object ID cua vi vua tao (tra ve Wallet::INVALID_ID neu that bai/trung ten)
    int addWallet(const std::string& name, Money initialBalance = 0);

    // Chinh sua ten vi theo ID
    bool editWalletName(int walletId, const std::string& newName);

    // Xoa mem vi theo ID (chi xoa khi khong bi rang buoc hoac theo quy tac)
    bool deleteWallet(int walletId);

    // Hien thi danh sach vi ra CLI
    void viewAllWallets() const;

    // --- Truy van & Tim kiem (Phuc vu TV3 Transaction & TV4 Statistics) ---
    bool exists(int walletId) const;
    int findIndexById(int walletId) const;
    int findIndexByName(const std::string& name) const;

    Wallet* getWalletById(int walletId);
    const Wallet* getWalletById(int walletId) const;
    std::string getWalletName(int walletId) const;

    const DynamicArray<Wallet>& getAllWallets() const;
    int getActiveWalletCount() const;

    // --- Quan ly So du Vi (Phuc vu TV3 Transaction & TV4 Dashboard) ---
    Money getWalletBalance(int walletId) const;
    Money getTotalBalance() const;

    bool canWithdraw(int walletId, Money amount, bool allowNegative = false) const;
    bool deposit(int walletId, Money amount);
    bool withdraw(int walletId, Money amount, bool allowNegative = false);
    bool adjustBalance(int walletId, Money delta);

    // --- Binary Persistence Hooks (Implement chi tiet o tuan sau) ---
    bool loadFromBinary(const std::string& recordFilePath, const std::string& namePoolPath);
    bool saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const;

private:
    DynamicArray<Wallet> wallets_;
    int nextId_;
};

#endif // MANAGERS_WALLET_MANAGER_H