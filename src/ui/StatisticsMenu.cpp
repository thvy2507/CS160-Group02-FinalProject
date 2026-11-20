/**
 * @file StatisticsMenu.cpp
 * @author TV4
 * @brief Handles date-range reports and statistics menu navigation.
 */

#include "ui/StatisticsMenu.h"

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "core/Date.h"
#include "statistics/StatisticsManager.h"

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

void StatisticsMenu::run(const TransactionManager& transactions) const {
    const int BACK = 0;
    const int DATE_SUMMARY = 1;

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
        } else {
            std::cout << "This report is not available yet.\n";
        }
    }
}

void StatisticsMenu::show() const {
    std::cout << "\n=== STATISTICS ===\n"
              << "1. Summary by date range\n"
              << "2. Summary by date and wallet [unavailable]\n"
              << "3. Annual overview [unavailable]\n"
              << "4. Income by source [unavailable]\n"
              << "5. Expenses by category [unavailable]\n"
              << "0. Back\nChoice: ";
}

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

bool StatisticsMenu::showDateSummary(
    const TransactionManager& transactions) const {
    Date start;
    Date end;

    if (!readDate("Start date (DD/MM/YYYY): ", start)) {
        return false;
    }

    if (!readDate("End date (DD/MM/YYYY): ", end)) {
        return false;
    }

    StatisticsManager statistics;
    StatisticsResult result;

    if (!statistics.calculateSummary(transactions, start, end, result)) {
        std::cout << "Unable to calculate summary. Check the date range "
                  << "and transaction data.\n";
        return true;
    }

    std::cout << "\n=== DATE-RANGE SUMMARY ===\n"
              << "Total Income: " << result.totalIncome << " VND\n"
              << "Total Expense: " << result.totalExpense << " VND\n"
              << "Net Balance: " << result.netBalance << " VND\n";

    return true;
}
