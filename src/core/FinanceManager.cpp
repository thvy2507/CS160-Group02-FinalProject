
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

        if (!readInt("Choose an option: ", choice)) {
            return;
        }

        if (!handleChoice(choice)) {
            return;
        }
    }
}
