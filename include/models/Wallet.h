#ifndef MODELS_WALLET_H
#define MODELS_WALLET_H

#include <cstdint>
#include <string>
#include <type_traits>

// Kieu du lieu tien te chung (dung int64_t / long long cho VND de tranh sai so float).
// Neu ca nhom thong nhat dung double, chi can doi thanh: using Money = double;
using Money = int64_t;

// Ban ghi kich thuoc co dinh (Fixed-size Record) phuc vu luu file nhi phan (.bin)
// Chuoi ten duoc luu trong Name Pool (.pool) va tham chieu qua (nameOffset, nameLength).
struct WalletRecord {
    int32_t id;             // Object ID duy nhat (khac voi Record Index trong file)
    uint32_t nameOffset;    // Vi tri byte bat dau cua ten vi trong Name Pool
    uint32_t nameLength;    // Do dai ten vi (tinh bang byte) trong Name Pool
    Money initialBalance;   // So du ban dau khi tao vi
    Money balance;          // So du hien tai cua vi
    uint8_t isDeleted;      // Co xoa mem: 0 = Active, 1 = Deleted
};

static_assert(std::is_trivially_copyable_v<WalletRecord>,
              "WalletRecord must be trivially copyable for binary file I/O.");

class Wallet {
public:
    static constexpr int INVALID_ID = 0;

    // Default constructor (bat buoc de su dung voi DynamicArray<Wallet> cap phat bang new[])
    Wallet();

    // Constructor tao vi moi
    Wallet(int id, const std::string& name, Money initialBalance = 0);

    // Constructor khoi phuc day du tu du lieu doc len
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

    // Nghiep vu cap nhat so du (phuc vu TransactionManager cua TV3)
    bool canWithdraw(Money amount, bool allowNegative = false) const;
    bool deposit(Money amount);
    bool withdraw(Money amount, bool allowNegative = false);
    void adjustBalance(Money delta);

    // Chuyen doi giua Object tren RAM va Fixed-size Record tren Disk (phuc vu Binary I/O cua TV1)
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