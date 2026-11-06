
/**
 * @file FinanceManager.cpp
 * @author TV1 (Vy)
 * @brief Implements the main console menu and navigation skeleton.
 */

#include "core/FinanceManager.h"

#include <iostream>
#include <limits>

void FinanceManager::displayMenu() const {
    std::cout << "\n===== PERSONAL FINANCE MANAGER =====\n";
    std::cout << "1. Income Management\n";
    std::cout << "2. Expense Management\n";
    std::cout << "3. Reports and Statistics\n";
    std::cout << "4. Wallet Management\n";
    std::cout << "5. Recurring Transactions\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

bool FinanceManager::handleChoice(int choice) const {
    switch (choice) {
        case 0:
            std::cout << "Goodbye!\n";
            return false;

        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            std::cout << "This module is not implemented yet.\n";
            return true;

        default:
            std::cout << "Invalid option. Please try again.\n";
            return true;
    }
}

void FinanceManager::run() {
    while (true) {
        displayMenu();

        int choice;

        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                return;
            }

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (!handleChoice(choice)) {
            return;
        }
    }
}
