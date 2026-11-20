/**
 * @file test_master_data.cpp
 * @author TV2 (Khoi)
 * @brief Tests master data management operations in memory.
 */

#include <cassert>
#include <iostream>

#include "managers/ExpenseCategoryManager.h"
#include "managers/IncomeSourceManager.h"
#include "managers/WalletManager.h"

void testWalletOperations() {
    std::cout << "--- Testing WalletManager ---\n";
    WalletManager wm;

    int w1 = wm.addWallet("Tien mat", 500000);
    assert(w1 == 1);
    int w2 = wm.addWallet("The tin dung", 0);
    assert(w2 == 2);

    int wInvalid = wm.addWallet("Tien mat", 100000);
    assert(wInvalid == Wallet::INVALID_ID);
    wInvalid = wm.addWallet("Vi ao", -50);
    assert(wInvalid == Wallet::INVALID_ID);

    assert(wm.editWalletName(w1, "Tien mat (VND)"));
    assert(wm.getWalletName(w1) == "Tien mat (VND)");
    assert(!wm.editWalletName(w1, "The tin dung"));

    assert(wm.deposit(w1, 100000));
    assert(wm.getWalletBalance(w1) == 600000);

    assert(!wm.deposit(w1, -50000));

    assert(wm.withdraw(w1, 200000));
    assert(wm.getWalletBalance(w1) == 400000);

    assert(!wm.withdraw(w1, 900000));
    assert(wm.withdraw(w1, 900000, true));
    assert(wm.getWalletBalance(w1) == -500000);

    assert(wm.getTotalBalance() == -500000);

    assert(wm.deleteWallet(w1));
    assert(!wm.exists(w1)); 
    assert(wm.getActiveWalletCount() == 1);
    
    int w3 = wm.addWallet("Vi moi", 0);
    assert(w3 == 3);

    std::cout << "-> WalletManager Passed.\n\n";
}

void testCategoryOperations() {
    std::cout << "--- Testing ExpenseCategoryManager ---\n";
    ExpenseCategoryManager ecm;

    int c1 = ecm.addExpenseCategory("An uong");
    int c2 = ecm.addExpenseCategory("Di lai");
    
    assert(c1 == 1 && c2 == 2);
    assert(ecm.getActiveCount() == 2);
    
    assert(ecm.deleteExpenseCategory(c1));
    assert(!ecm.exists(c1));
    assert(ecm.getActiveCount() == 1);

    int c3 = ecm.addExpenseCategory("An uong");
    assert(c3 == 3);

    std::cout << "-> ExpenseCategoryManager Passed.\n\n";
}

int main() {
    std::cout << "=== MASTER DATA TESTS ===\n";
    testWalletOperations();
    testCategoryOperations();
    std::cout << "All master data tests passed!\n";
    return 0;
}
