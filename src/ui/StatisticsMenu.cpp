/**
 * @file StatisticsMenu.cpp
 * @author TV4
 * @brief Handles date-range and wallet reports in the statistics menu.
 */

#include "ui/StatisticsMenu.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "core/Date.h"
#include "managers/WalletManager.h"
#include "statistics/StatisticsManager.h"

namespace {

// Output Helpers
void printTotals(const StatisticsResult& result) {
    std::cout << "Total Income: " << result.totalIncome << " VND\n"
              << "Total Expense: " << result.totalExpense << " VND\n"
              << "Net Balance: " << result.netBalance << " VND\n";
}

} // namespace

// Menu & Navigation
void StatisticsMenu::show() const {
    std::cout << "\n=== STATISTICS ===\n"
              << "1. Summary by date range\n"
              << "2. Summary by date and wallet\n"
              << "3. Annual overview [unavailable]\n"
              << "4. Income by source [unavailable]\n"
              << "5. Expenses by category [unavailable]\n"
              << "0. Back\nChoice: ";
}

int StatisticsMenu::readChoice() const {
    const int BACK = 0;
    const int LAST_REPORT = 5;
    std::string line;

    while (std::getline(std::cin, line)) {
        std::istringstream input(line);
        int choice = BACK;
        char extra;

        if ((input >> choice) && !(input >> extra)
            && choice >= BACK && choice <= LAST_REPORT) {
            return choice;
        }

        std::cout << "Enter an integer from 0 to 5: ";
    }

    return BACK;
}

void StatisticsMenu::run(const TransactionManager& transactions,
                         const WalletManager& wallets) const {
    const int BACK = 0;
    const int DATE_SUMMARY = 1;
    const int WALLET_SUMMARY = 2;

    while (true) {
        show();
        int choice = readChoice();

        if (choice == BACK) {
            return;
        }

        if (choice == DATE_SUMMARY) {
            if (!showDateSummary(transactions)) {
                return;
            }
        } else if (choice == WALLET_SUMMARY) {
            if (!showWalletSummary(transactions, wallets)) {
                return;
            }
        } else {
            std::cout << "This report is not available yet.\n";
        }
    }
}

// Private Helpers
bool StatisticsMenu::readDate(const char* prompt, Date& date) const {
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
            && separator1 == '/' && separator2 == '/'
            && !(input >> extra)) {
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

bool StatisticsMenu::readDateRange(Date& start, Date& end) const {
    return readDate("Start date (DD/MM/YYYY): ", start)
        && readDate("End date (DD/MM/YYYY): ", end);
}

bool StatisticsMenu::showDateSummary(
    const TransactionManager& transactions) const {
    Date start;
    Date end;
    if (!readDateRange(start, end)) {
        return false;
    }

    StatisticsManager statistics;
    StatisticsResult result;
    if (!statistics.calculateSummary(transactions, start, end, result)) {
        std::cout << "Unable to calculate summary. Check the date range "
                  << "and transaction data.\n";
        return true;
    }

    std::cout << "\n=== DATE-RANGE SUMMARY ===\n";
    printTotals(result);
    return true;
}

bool StatisticsMenu::showWalletSummary(
    const TransactionManager& transactions, const WalletManager& wallets) const {
    if (wallets.getActiveWalletCount() == 0) {
        std::cout << "No active wallets found.\n";
        return true;
    }

    wallets.viewAllWallets();
    const int BACK = 0;
    int walletId = BACK;
    std::string line;

    while (true) {
        std::cout << "Wallet ID (0 to go back): ";
        if (!std::getline(std::cin, line)) {
            return false;
        }

        std::istringstream input(line);
        char extra;
        if ((input >> walletId) && !(input >> extra)) {
            if (walletId == BACK) {
                return true;
            }
            if (wallets.exists(walletId)) {
                break;
            }
        }
        std::cout << "Invalid wallet ID. Choose an active wallet.\n";
    }

    Date start;
    Date end;
    if (!readDateRange(start, end)) {
        return false;
    }

    StatisticsManager statistics;
    StatisticsResult result;
    if (!statistics.calculateWalletSummary(
            transactions, start, end, walletId, wallets.getAllWallets(), result)) {
        std::cout << "Unable to calculate wallet summary. Check the date range "
                  << "and transaction data.\n";
        return true;
    }

    std::cout << "\n=== WALLET SUMMARY ===\n"
              << "Wallet: " << wallets.getWalletName(walletId)
              << " (ID: " << walletId << ")\n";
    printTotals(result);
    return true;
}
