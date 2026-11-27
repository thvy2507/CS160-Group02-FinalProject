#include <iostream>
#include <cassert>
#include "managers/WalletManager.h"
#include "managers/ExpenseCategoryManager.h"
#include "managers/IncomeSourceManager.h"
#include "core/BinaryIO.h"

int main() {
    std::cout << "--- TEST SPRINT 3: MASTER DATA BINARY PERSISTENCE ---\n";

    // --- TEST WALLET ---
    {
        WalletManager wm;
        int w1 = wm.addWallet("Tien mat", 1000);
        int w2 = wm.addWallet("The", 500);
        wm.deleteWallet(w1);
        wm.deposit(w2, 200);

        assert(wm.saveToBinary("wallets.dat", "wallets.pool"));

        WalletManager wm_restore;
        assert(wm_restore.loadFromBinary("wallets.dat", "wallets.pool"));
        assert(wm_restore.getActiveWalletCount() == 1);
        assert(!wm_restore.exists(w1));
        assert(wm_restore.getWalletBalance(w2) == 700);
        assert(wm_restore.addWallet("Vi moi", 0) == 3); // nextId phai dc restore
    }
    std::cout << "-> Wallet persistence passed.\n";

    // --- TEST EXPENSE ---
    {
        ExpenseCategoryManager ecm;
        ecm.addExpenseCategory("An uong");
        assert(ecm.saveToBinary("expense.dat", "expense.pool"));

        ExpenseCategoryManager ecm_restore;
        assert(ecm_restore.loadFromBinary("expense.dat", "expense.pool"));
        assert(ecm_restore.getActiveCount() == 1);
        assert(ecm_restore.getExpenseCategoryName(1) == "An uong");
    }
    std::cout << "-> Expense Category persistence passed.\n";

    // --- TEST CORRUPTION (Fail-safe) ---
    {
        IncomeSourceManager ism;
        ism.addIncomeSource("Luong");
        ism.saveToBinary("income.dat", "income.pool");

        // Ghi de file dat bang size khong hop le
        char badData[3] = {1, 2, 3};
        BinaryIO::writeBuffer("income.dat", badData, 3);

        IncomeSourceManager ism2;
        ism2.addIncomeSource("Tien thuong"); // Giu lai neu load loi
        assert(!ism2.loadFromBinary("income.dat", "income.pool")); // Phai tra ve false vi file hong
        assert(ism2.getActiveCount() == 1); // Khong bi ghi de
    }
    std::cout << "-> Fail-safe loading passed.\n";

    std::cout << "ALL SPRINT 3 TESTS PASSED.\n";
    return 0;
}
