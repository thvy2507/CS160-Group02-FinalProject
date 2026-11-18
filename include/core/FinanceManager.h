
/**
 * @file FinanceManager.h
 * @author TV1 (Vy)
 * @brief Declares the main application controller.
 */

#pragma once

#include <string>

#include "core/Date.h"

class FinanceManager {
public:
    void run();

private:
    // Input Helpers
    bool readInt(const std::string& prompt, int& value) const;
    bool readMoney(const std::string& prompt, long long& amount) const;
    bool readText(const std::string& prompt, std::string& value, bool allowEmpty = false) const;
    bool readDate(const std::string& prompt, Date& date) const;

    // Menu & Navigation
    void displayMenu() const;
    bool handleChoice(int choice) const;

    void runIncomeMenu() const;
    void runIncomeSourceMenu() const;

    void runExpenseMenu() const;
    void runExpenseCategoryMenu() const;

    void runStatisticsMenu() const;

    void runWalletMenu() const;
};
