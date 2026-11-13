/**
 * @file StatisticsMenu.cpp
 * @author TV4
 * @brief Week 1 statistics and dashboard interface skeleton.
 * @note Proposed interfaces; shared model APIs require team review.
 */
#include "ui/StatisticsMenu.h"

#include <iostream>
#include <sstream>
#include <string>

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

void StatisticsMenu::show() const {
    std::cout << "=== STATISTICS ===\n"
              << "1. Summary by date range\n"
              << "2. Summary by date and wallet\n"
              << "3. Annual overview\n"
              << "4. Income by source\n"
              << "5. Expenses by category\n"
              << "0. Back\nChoice: ";
}
