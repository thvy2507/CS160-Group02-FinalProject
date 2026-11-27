/**
 * @file WalletManager.cpp
 * @author TV2 (Khoi)
 * @brief Implements wallet management and balance operations.
 */

#include "managers/WalletManager.h"

#include <fstream>
#include <iostream>
#include <limits>

#include "core/BinaryIO.h"

namespace {

    bool writeWalletRecord(std::ostream& file, const WalletRecord& record) {
        return BinaryIO::writeInt32(file, record.id)
            && BinaryIO::writeUInt32(file, record.nameOffset)
            && BinaryIO::writeUInt32(file, record.nameLength)
            && BinaryIO::writeInt64(file, record.initialBalance)
            && BinaryIO::writeInt64(file, record.balance)
            && BinaryIO::writeUInt8(file, record.isDeleted);
    }

    bool readWalletRecord(std::istream& file, WalletRecord& record) {
        return BinaryIO::readInt32(file, record.id)
            && BinaryIO::readUInt32(file, record.nameOffset)
            && BinaryIO::readUInt32(file, record.nameLength)
            && BinaryIO::readInt64(file, record.initialBalance)
            && BinaryIO::readInt64(file, record.balance)
            && BinaryIO::readUInt8(file, record.isDeleted);
    }

    bool isValidWalletRecord(const WalletRecord& record,
                             const std::string& name,
                             const DynamicArray<Wallet>& wallets) {
        if (record.id <= Wallet::INVALID_ID ||
            record.id == std::numeric_limits<int32_t>::max() ||
            record.isDeleted > 1 ||
            record.initialBalance < 0 ||
            name.empty()) {
            return false;
        }

        for (int i = 0; i < wallets.size(); ++i) {
            const Wallet& wallet = wallets[i];

            if (wallet.getId() == record.id) {
                return false;
            }

            if (record.isDeleted == 0 && !wallet.isDeleted() && wallet.getName() == name) {
                return false;
            }
        }

        return true;
    }

} // namespace

WalletManager::WalletManager() : m_wallets(), m_nextId(1) {}

int WalletManager::addWallet(const std::string& name, Money initialBalance) {
    if (m_nextId == std::numeric_limits<int32_t>::max() ||
        name.empty() || initialBalance < 0 ||
        findIndexByName(name) != -1) {
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

    if (recordFilePath == namePoolPath) {
        return false;
    }

    std::ifstream recordFile(recordFilePath, std::ios::binary);
    std::ifstream namePool(namePoolPath, std::ios::binary);

    if (!recordFile || !namePool) {
        return false;
    }

    uint64_t recordFileSize, poolSize;
    BinaryIO::FileHeader header{};

    if (!BinaryIO::getFileSize(recordFile, recordFileSize) ||
        !BinaryIO::getFileSize(namePool, poolSize) ||
        !BinaryIO::readHeader(recordFile, header) ||
        !BinaryIO::validateHeader(
            header, BinaryIO::WALLET_MAGIC,
            BinaryIO::WALLET_RECORD_SIZE, recordFileSize,
            std::numeric_limits<int>::max() - 1) ||
        poolSize > std::numeric_limits<uint32_t>::max()) {
        return false;
    }

    DynamicArray<Wallet> tempWallets(header.recordCount == 0 ?
                                     4 : static_cast<int>(header.recordCount)
                                     );

    int maxId = 0;
    Money totalBalance = 0;
    for (uint32_t i = 0; i < header.recordCount; ++i) {
        WalletRecord record{};
        std::string name;

        if (!readWalletRecord(recordFile, record) ||
            !BinaryIO::readString(namePool, record.nameOffset, record.nameLength, poolSize, name) ||
            !isValidWalletRecord(record, name, tempWallets)) {
            return false;
        }

        if (record.isDeleted == 0) {
            if ((record.balance > 0 &&
                 totalBalance > std::numeric_limits<Money>::max() - record.balance) ||
                (record.balance < 0 && totalBalance < std::numeric_limits<Money>::min() - record.balance)) {
                return false;
            }

            totalBalance += record.balance;
        }

        tempWallets.add(Wallet::fromRecord(record, name));

        if (record.id > maxId) {
            maxId = record.id;
        }
    }

    m_wallets = tempWallets;
    m_nextId = maxId + 1;

    return true;
}

bool WalletManager::saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const {
    if (recordFilePath == namePoolPath) {
        return false;
    }

    std::ofstream recordFile(recordFilePath, std::ios::binary | std::ios::trunc);
    std::ofstream namePool(namePoolPath, std::ios::binary | std::ios::trunc);

    if (!recordFile || !namePool) {
        return false;
    }

    uint32_t recordCount = static_cast<uint32_t>(m_wallets.size());

    if (!BinaryIO::writeHeader(recordFile, BinaryIO::WALLET_MAGIC, BinaryIO::WALLET_RECORD_SIZE, recordCount)) {
        return false;
    }

    for (int i = 0; i < m_wallets.size(); ++i) {
        const Wallet& wallet = m_wallets[i];

        uint32_t offset, length;

        if (!BinaryIO::appendString(namePool, wallet.getName(), offset, length)) {
            return false;
        }

        WalletRecord record = wallet.toRecord(offset, length);

        if (!writeWalletRecord(recordFile, record)) {
            return false;
        }
    }

    recordFile.close();
    namePool.close();

    return static_cast<bool>(recordFile) && static_cast<bool>(namePool);
}
