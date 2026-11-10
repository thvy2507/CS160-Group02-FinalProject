/**
 * @file test_statistics.cpp
 * @author TV4
 * @brief Week 1 statistics and dashboard interface skeleton.
 * @note Proposed interfaces; shared model APIs require team review.
 */
#include "statistics/StatisticsManager.h"
#include "statistics/NumberFormatter.h"
#include "ui/Dashboard.h"
#include "ui/StatisticsMenu.h"

#include <iostream>
#include <string>

int main() {
    std::string output = "unchanged";
    if (NumberFormatter::formatMoney(1234.0, output) || output != "unchanged") {
        return 1;
    }
    Dashboard dashboard;
    StatisticsMenu menu;
    dashboard.showPreview();
    menu.show();
    const int choice = menu.readChoice();
    if (choice != 0) {
        std::cout << "Report not implemented in Week 1.\n";
    }
    return 0;
}
