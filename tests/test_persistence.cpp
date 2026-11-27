/**
 * @file test_persistence.cpp
 * @author TV2 (Khoi), TV1 (Vy)
 * @brief Tests master data binary persistence and safe loading.
 */

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

#include "core/BinaryIO.h"
#include "managers/ExpenseCategoryManager.h"
#include "managers/IncomeSourceManager.h"
#include "managers/WalletManager.h"

// Test Constants
const std::string RECORD_FILE = "test_records.bin";
const std::string POOL_FILE = "test_names.bin";

// Test Helpers
void cleanFiles() {
    std::remove(RECORD_FILE.c_str());
    std::remove(POOL_FILE.c_str());
}

void expectFileSize(const std::string& path, uint64_t expectedSize) {
    std::ifstream file(path, std::ios::binary);
    uint64_t size;

    assert(BinaryIO::getFileSize(file, size));
    assert(size == expectedSize);
}

void overwriteUInt32(uint64_t offset, uint32_t value) {
    std::fstream file(RECORD_FILE, std::ios::binary | std::ios::in | std::ios::out);

    assert(file);
    file.seekp(static_cast<std::streamoff>(offset));
    assert(BinaryIO::writeUInt32(file, value));
}

void overwriteUInt8(uint64_t offset, uint8_t value) {
    std::fstream file(RECORD_FILE, std::ios::binary | std::ios::in | std::ios::out);

    assert(file);
    file.seekp(static_cast<std::streamoff>(offset));
    assert(BinaryIO::writeUInt8(file, value));
}

// Persistence Tests
void testWalletPersistence() {
    WalletManager manager;

    assert(manager.addWallet("Cash", 1000) == 1);
    assert(manager.addWallet("Bank", 500) == 2);
    assert(manager.deleteWallet(1));
    assert(manager.deposit(2, 200));

    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));

    expectFileSize(RECORD_FILE, 16 + 2 * 29);
    expectFileSize(POOL_FILE, 8);

    WalletManager restored;

    assert(restored.loadFromBinary(RECORD_FILE, POOL_FILE));
    assert(restored.getAllWallets().size() == 2);
    assert(restored.getActiveWalletCount() == 1);
    assert(restored.getAllWallets()[0].isDeleted());
    assert(!restored.exists(1));

    assert(restored.getWalletName(2) == "Bank");
    assert(restored.getWalletById(2)->getInitialBalance() == 500);
    assert(restored.getWalletBalance(2) == 700);
    assert(restored.addWallet("New Wallet", 0) == 3);

    cleanFiles();
}

void testIncomeSourcePersistence() {
    IncomeSourceManager manager;

    assert(manager.addIncomeSource("Salary") == 1);
    assert(manager.addIncomeSource("Bonus") == 2);
    assert(manager.deleteIncomeSource(1));

    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));

    expectFileSize(RECORD_FILE, 16 + 2 * 13);
    expectFileSize(POOL_FILE, 11);

    IncomeSourceManager restored;

    assert(restored.loadFromBinary(RECORD_FILE, POOL_FILE));
    assert(restored.getAllIncomeSources().size() == 2);
    assert(restored.getActiveCount() == 1);
    assert(restored.getAllIncomeSources()[0].isDeleted());
    assert(!restored.exists(1));

    assert(restored.getIncomeSourceName(2) == "Bonus");
    assert(restored.addIncomeSource("Freelance") == 3);
    assert(restored.addIncomeSource("Bonus") == 0);
    assert(restored.addIncomeSource("Salary") == 4);

    cleanFiles();
}

void testExpenseCategoryPersistence() {
    ExpenseCategoryManager manager;

    assert(manager.addExpenseCategory("Food") == 1);
    assert(manager.addExpenseCategory("Rent") == 2);
    assert(manager.deleteExpenseCategory(1));

    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));

    expectFileSize(RECORD_FILE, 16 + 2 * 13);
    expectFileSize(POOL_FILE, 8);

    ExpenseCategoryManager restored;

    assert(restored.loadFromBinary(RECORD_FILE, POOL_FILE));
    assert(restored.getAllExpenseCategories().size() == 2);
    assert(restored.getActiveCount() == 1);
    assert(restored.getAllExpenseCategories()[0].isDeleted());
    assert(!restored.exists(1));

    assert(restored.getExpenseCategoryName(2) == "Rent");
    assert(restored.addExpenseCategory("Transport") == 3);
    assert(restored.addExpenseCategory("Rent") == 0);
    assert(restored.addExpenseCategory("Food") == 4);

    cleanFiles();
}

void testEmptyDatasets() {
    WalletManager wallets;
    IncomeSourceManager sources;
    ExpenseCategoryManager categories;

    assert(wallets.saveToBinary(RECORD_FILE, POOL_FILE));
    expectFileSize(RECORD_FILE, 16);
    expectFileSize(POOL_FILE, 0);
    assert(wallets.addWallet("Cash", 0) == 1);
    assert(wallets.loadFromBinary(RECORD_FILE, POOL_FILE));
    assert(wallets.getAllWallets().empty());
    assert(wallets.addWallet("Bank", 0) == 1);

    assert(sources.saveToBinary(RECORD_FILE, POOL_FILE));
    expectFileSize(RECORD_FILE, 16);
    expectFileSize(POOL_FILE, 0);
    assert(sources.addIncomeSource("Salary") == 1);
    assert(sources.loadFromBinary(RECORD_FILE, POOL_FILE));
    assert(sources.getAllIncomeSources().empty());
    assert(sources.addIncomeSource("Bonus") == 1);

    assert(categories.saveToBinary(RECORD_FILE, POOL_FILE));
    expectFileSize(RECORD_FILE, 16);
    expectFileSize(POOL_FILE, 0);
    assert(categories.addExpenseCategory("Food") == 1);
    assert(categories.loadFromBinary(RECORD_FILE, POOL_FILE));
    assert(categories.getAllExpenseCategories().empty());
    assert(categories.addExpenseCategory("Rent") == 1);

    cleanFiles();
}

// Validation Tests
void testMissingFiles() {
    cleanFiles();

    WalletManager manager;
    assert(manager.addWallet("Cash", 1000) == 1);

    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));

    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));
    std::remove(POOL_FILE.c_str());

    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));
    assert(manager.getWalletBalance(1) == 1000);
    assert(manager.addWallet("Bank", 0) == 2);

    cleanFiles();
}

void testInvalidHeaders() {
    IncomeSourceManager manager;
    assert(manager.addIncomeSource("Salary") == 1);

    // Invalid magic.
    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));
    overwriteUInt8(0, 0);
    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));

    // Unsupported version.
    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));
    overwriteUInt32(4, 999);
    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));

    // Incorrect record size.
    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));
    overwriteUInt32(8, 999);
    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));

    // Incorrect record count.
    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));
    overwriteUInt32(12, 999);
    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));

    assert(manager.getIncomeSourceName(1) == "Salary");
    assert(manager.addIncomeSource("Bonus") == 2);

    cleanFiles();
}

void testInvalidFileSizes() {
    IncomeSourceManager manager;
    assert(manager.addIncomeSource("Salary") == 1);

    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));

    // Unexpected trailing byte.
    {
        std::ofstream file(RECORD_FILE, std::ios::binary | std::ios::app);

        assert(BinaryIO::writeUInt8(file, 0));
    }

    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));

    // Truncated header.
    {
        std::ofstream file(RECORD_FILE, std::ios::binary | std::ios::trunc);

        assert(BinaryIO::writeUInt8(file, 0));
    }

    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));
    assert(manager.getActiveCount() == 1);

    cleanFiles();
}

void testInvalidPoolReferences() {
    ExpenseCategoryManager manager;
    assert(manager.addExpenseCategory("Food") == 1);

    // Invalid offset.
    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));
    overwriteUInt32(16 + 4, 999);
    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));

    // Invalid length.
    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));
    overwriteUInt32(16 + 8, 999);
    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));

    // Empty name.
    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));
    overwriteUInt32(16 + 8, 0);
    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));

    assert(manager.getExpenseCategoryName(1) == "Food");
    assert(manager.addExpenseCategory("Rent") == 2);

    cleanFiles();
}

void testInvalidDeletionFlag() {
    WalletManager manager;
    assert(manager.addWallet("Cash", 1000) == 1);

    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));
    overwriteUInt8(16 + 28, 2);

    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));
    assert(manager.getWalletBalance(1) == 1000);

    cleanFiles();
}

void testInvalidIds() {
    IncomeSourceManager manager;

    assert(manager.addIncomeSource("Salary") == 1);
    assert(manager.addIncomeSource("Bonus") == 2);
    assert(manager.deleteIncomeSource(1));

    // Non-positive ID.
    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));
    overwriteUInt32(16, 0);
    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));

    // Duplicate ID, including a deleted record.
    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));
    overwriteUInt32(16 + 13, 1);
    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));

    assert(manager.getAllIncomeSources().size() == 2);
    assert(manager.getActiveCount() == 1);
    assert(manager.addIncomeSource("Freelance") == 3);

    cleanFiles();
}

void testDuplicateActiveNames() {
    ExpenseCategoryManager manager;

    assert(manager.addExpenseCategory("Food") == 1);
    assert(manager.addExpenseCategory("Rent") == 2);
    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));

    // Make the second record reference the first name.
    overwriteUInt32(16 + 13 + 4, 0);

    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));
    assert(manager.getExpenseCategoryName(2) == "Rent");
    assert(manager.addExpenseCategory("Transport") == 3);

    cleanFiles();
}

void testExhaustedIds() {
    IncomeSourceManager manager;

    assert(manager.addIncomeSource("Salary") == 1);
    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));

    const uint32_t maxId = std::numeric_limits<int32_t>::max();

    // Restore the largest assignable ID.
    overwriteUInt32(16, maxId - 1);

    assert(manager.loadFromBinary(RECORD_FILE, POOL_FILE));
    assert(manager.exists(static_cast<int>(maxId - 1)));
    assert(manager.addIncomeSource("Bonus") == 0);

    // The reserved exhausted-ID value is not a valid record ID.
    overwriteUInt32(16, maxId);

    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));
    assert(manager.exists(static_cast<int>(maxId - 1)));

    cleanFiles();
}

void testWalletBalanceOverflow() {
    WalletManager manager;

    assert(manager.addWallet("Cash", 1000) == 1);
    assert(manager.addWallet("Bank", 500) == 2);
    assert(manager.saveToBinary(RECORD_FILE, POOL_FILE));

    // Set the first current balance to the maximum value.
    {
        std::fstream file(RECORD_FILE, std::ios::binary | std::ios::in | std::ios::out);

        file.seekp(16 + 20);

        assert(BinaryIO::writeInt64(file, std::numeric_limits<int64_t>::max()
        ));
    }

    assert(!manager.loadFromBinary(RECORD_FILE, POOL_FILE));
    assert(manager.getWalletBalance(1) == 1000);
    assert(manager.getWalletBalance(2) == 500);
    assert(manager.getTotalBalance() == 1500);
    assert(manager.addWallet("New Wallet", 0) == 3);

    cleanFiles();
}

int main() {
    std::cout << "===== MASTER DATA PERSISTENCE TESTS =====\n";

    cleanFiles();

    testWalletPersistence();
    std::cout << "PASS: Wallet persistence\n";

    testIncomeSourcePersistence();
    std::cout << "PASS: Income source persistence\n";

    testExpenseCategoryPersistence();
    std::cout << "PASS: Expense category persistence\n";

    testEmptyDatasets();
    std::cout << "PASS: Empty datasets\n";

    testMissingFiles();
    std::cout << "PASS: Reject missing files\n";

    testInvalidHeaders();
    std::cout << "PASS: Reject invalid headers\n";

    testInvalidFileSizes();
    std::cout << "PASS: Reject invalid file sizes\n";

    testInvalidPoolReferences();
    std::cout << "PASS: Reject invalid pool references\n";

    testInvalidDeletionFlag();
    std::cout << "PASS: Reject invalid deletion flag\n";

    testInvalidIds();
    std::cout << "PASS: Reject invalid and duplicate IDs\n";

    testDuplicateActiveNames();
    std::cout << "PASS: Reject duplicate active names\n";

    testExhaustedIds();
    std::cout << "PASS: Handle exhausted IDs\n";

    testWalletBalanceOverflow();
    std::cout << "PASS: Reject aggregate balance overflow\n";

    std::cout << "\nAll 13 persistence test groups passed!\n";

    return 0;
}
