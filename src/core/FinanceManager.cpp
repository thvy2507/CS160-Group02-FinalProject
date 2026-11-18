
/**
 * @file FinanceManager.cpp
 * @author TV1 (Vy)
 * @brief Implements the main console menu and navigation skeleton.
 */

#include "core/FinanceManager.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "ui/StatisticsMenu.h"

// Input Helpers
bool FinanceManager::readInt(const std::string& prompt, int& value) const {
    std::string line;

    while (true) {
        std::cout << prompt;

        if (!std::getline(std::cin, line)) {
            return false;
        }

        std::istringstream input(line);
        char extra;

        if ((input >> value) && !(input >> extra)) {
            return true;
        }

        std::cout << "Invalid input. Please enter an integer.\n";
    }
}

bool FinanceManager::readMoney(const std::string& prompt, long long& amount) const {
    std::string line;

    while (true) {
        std::cout << prompt;

        if (!std::getline(std::cin, line)) {
            return false;
        }

        std::istringstream input(line);
        char extra;

        if ((input >> amount) && !(input >> extra)) {
            return true;
        }

        std::cout << "Invalid amount. Please enter an integer.\n";
    }
}

bool FinanceManager::readText(const std::string& prompt, std::string& value, bool allowEmpty) const {
    while (true) {
        std::cout << prompt;

        if (!std::getline(std::cin, value)) {
            return false;
        }

        if (value.find_first_not_of(" \t\r\n") != std::string::npos) {
            return true;
        }

        if (allowEmpty) {
            value.clear();
            return true;
        }

        std::cout << "Input cannot be empty.\n";
    }
}

bool FinanceManager::readDate(const std::string& prompt, Date& date) const {
    std::string line;

    while (true) {
        std::cout << prompt;

        if (!std::getline(std::cin, line)) {
            return false;
        }

        std::istringstream input(line);

        int day, month, year;
        char separator1, separator2, extra;

        if ((input >> day >> separator1 >> month >> separator2 >> year)
            && separator1 == '/' && separator2 == '/' && !(input >> extra)) {
            try {
                date = Date(day, month, year);
                return true;
            } catch (const std::invalid_argument&) {
                std::cout << "Invalid date.\n";
                continue;
            }
        }

        std::cout << "Invalid format. Please use DD/MM/YYYY.\n";
    }
}

// Menu & Navigation
void FinanceManager::displayMenu() const {
    std::cout << "\n===== PERSONAL FINANCE MANAGER =====\n";
    std::cout << "1. Income Management\n";
    std::cout << "2. Expense Management\n";
    std::cout << "3. Reports and Statistics\n";
    std::cout << "4. Wallet Management\n";
    std::cout << "5. Recurring Transactions\n";
    std::cout << "0. Exit\n";
}

bool FinanceManager::handleChoice(int choice) const {
    switch (choice) {
        case 0:
            std::cout << "Goodbye!\n";
            return false;

        case 1:
            runIncomeMenu();
            return true;

        case 2:
            runExpenseMenu();
            return true;

        case 3:
            runStatisticsMenu();
            return true;

        case 4:
            runWalletMenu();
            return true;

        case 5:
            std::cout << "This module is not implemented yet.\n";
            return true;

        default:
            std::cout << "Invalid option. Please try again.\n";
            return true;
    }
}

void FinanceManager::runIncomeMenu() const {
    while (true) {
        std::cout << "\n===== INCOME MANAGEMENT =====\n";
        std::cout << "1. Add Income\n";
        std::cout << "2. View Income\n";
        std::cout << "3. Edit Income\n";
        std::cout << "4. Delete Income\n";
        std::cout << "5. Manage Income Sources\n";
        std::cout << "0. Back\n";

        int choice;

        if (!readInt("Choose an option: ", choice)) {
            return;
        }

        switch (choice) {
            case 0:
                return;

            case 1:
            case 2:
            case 3:
            case 4:
                std::cout << "This operation is not implemented yet.\n";
                break;

            case 5:
                runIncomeSourceMenu();
                break;

            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }
}

void FinanceManager::runIncomeSourceMenu() const {
    while (true) {
        std::cout << "\n===== INCOME SOURCE MANAGEMENT =====\n";
        std::cout << "1. Add Income Source\n";
        std::cout << "2. View Income Sources\n";
        std::cout << "3. Edit Income Source\n";
        std::cout << "4. Delete Income Source\n";
        std::cout << "0. Back\n";

        int choice;

        if (!readInt("Choose an option: ", choice)) {
            return;
        }

        switch (choice) {
            case 0:
                return;

            case 1:
            case 2:
            case 3:
            case 4:
                std::cout << "This operation is not implemented yet.\n";
                break;

            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }
}

void FinanceManager::runExpenseMenu() const {
    while (true) {
        std::cout << "\n===== EXPENSE MANAGEMENT =====\n";
        std::cout << "1. Add Expense\n";
        std::cout << "2. View Expenses\n";
        std::cout << "3. Edit Expense\n";
        std::cout << "4. Delete Expense\n";
        std::cout << "5. Manage Expense Categories\n";
        std::cout << "0. Back\n";

        int choice;

        if (!readInt("Choose an option: ", choice)) {
            return;
        }

        switch (choice) {
            case 0:
                return;

            case 1:
            case 2:
            case 3:
            case 4:
                std::cout << "This operation is not implemented yet.\n";
                break;

            case 5:
                runExpenseCategoryMenu();
                break;

            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }
}

void FinanceManager::runExpenseCategoryMenu() const {
    while (true) {
        std::cout << "\n===== EXPENSE CATEGORY MANAGEMENT =====\n";
        std::cout << "1. Add Expense Category\n";
        std::cout << "2. View Expense Categories\n";
        std::cout << "3. Edit Expense Category\n";
        std::cout << "4. Delete Expense Category\n";
        std::cout << "0. Back\n";

        int choice;

        if (!readInt("Choose an option: ", choice)) {
            return;
        }

        switch (choice) {
            case 0:
                return;

            case 1:
            case 2:
            case 3:
            case 4:
                std::cout << "This operation is not implemented yet.\n";
                break;

            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }
}

void FinanceManager::runStatisticsMenu() const {
    StatisticsMenu menu;

    while (true) {
        menu.show();

        int choice = menu.readChoice();

        if (choice == 0) {
            return;
        }

        std::cout << "This report is not implemented yet.\n";
    }
}

void FinanceManager::runWalletMenu() const {
    while (true) {
        std::cout << "\n===== WALLET MANAGEMENT =====\n";
        std::cout << "1. Add Wallet\n";
        std::cout << "2. View Wallets\n";
        std::cout << "3. Edit Wallet\n";
        std::cout << "4. Delete Wallet\n";
        std::cout << "0. Back\n";

        int choice;

        if (!readInt("Choose an option: ", choice)) {
            return;
        }

        switch (choice) {
            case 0:
                return;

            case 1:
            case 2:
            case 3:
            case 4:
                std::cout << "This operation is not implemented yet.\n";
                break;

            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }
}

void FinanceManager::run() {
    while (true) {
        displayMenu();

        int choice;

        if (!readInt("Choose an option: ", choice)) {
            return;
        }

        if (!handleChoice(choice)) {
            return;
        }
    }
}
