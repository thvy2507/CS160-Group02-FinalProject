
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

// Constructors
FinanceManager::FinanceManager()
    : m_transactionManager(&m_walletManager,
                           &m_incomeSourceManager,
                           &m_expenseCategoryManager) {}

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

bool FinanceManager::handleChoice(int choice) {
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

void FinanceManager::runIncomeMenu() {
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
                if (!addIncome()) {
                    return;
                }
                break;

            case 2:
                viewTransactions(TransactionType::INCOME);
                break;

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

void FinanceManager::runIncomeSourceMenu() {
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

            case 1: {
                std::string name;

                if (!readText("Income source name: ", name)) {
                    return;
                }

                int id = m_incomeSourceManager.addIncomeSource(name);

                if (id > 0) {
                    std::cout << "Income source added successfully. ID: " << id << "\n";
                } else {
                    std::cout << "Unable to add income source. Check the name.\n";
                }

                break;
            }

            case 2:
                m_incomeSourceManager.viewAllIncomeSources();
                break;

            case 3: {
                m_incomeSourceManager.viewAllIncomeSources();

                int id;
                std::string name;

                if (!readInt("Income source ID: ", id)) {
                    return;
                }

                if (!m_incomeSourceManager.exists(id)) {
                    std::cout << "Income source not found.\n";
                    break;
                }

                if (!readText("New name: ", name)) {
                    return;
                }

                if (m_incomeSourceManager.editIncomeSource(id, name)) {
                    std::cout << "Income source updated successfully.\n";
                } else {
                    std::cout << "Unable to update income source.\n";
                }

                break;
            }

            case 4: {
                m_incomeSourceManager.viewAllIncomeSources();

                int id;

                if (!readInt("Income source ID: ", id)) {
                    return;
                }

                if (m_incomeSourceManager.deleteIncomeSource(id)) {
                    std::cout << "Income source deleted successfully.\n";
                } else {
                    std::cout << "Income source not found.\n";
                }

                break;
            }

            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }
}

void FinanceManager::runExpenseMenu() {
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
                if (!addExpense()) {
                    return;
                }
                break;

            case 2:
                viewTransactions(TransactionType::EXPENSE);
                break;

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

void FinanceManager::runExpenseCategoryMenu() {
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

            case 1: {
                std::string name;

                if (!readText("Expense category name: ", name)) {
                    return;
                }

                int id = m_expenseCategoryManager.addExpenseCategory(name);

                if (id > 0) {
                    std::cout << "Expense category added successfully. ID: " << id << "\n";
                } else {
                    std::cout << "Unable to add expense category. Check the name.\n";
                }

                break;
            }

            case 2:
                m_expenseCategoryManager.viewAllExpenseCategories();
                break;

            case 3: {
                m_expenseCategoryManager.viewAllExpenseCategories();

                int id;
                std::string name;

                if (!readInt("Expense category ID: ", id)) {
                    return;
                }

                if (!m_expenseCategoryManager.exists(id)) {
                    std::cout << "Expense category not found.\n";
                    break;
                }

                if (!readText("New name: ", name)) {
                    return;
                }

                if (m_expenseCategoryManager.editExpenseCategory(id, name)) {
                    std::cout << "Expense category updated successfully.\n";
                } else {
                    std::cout << "Unable to update expense category.\n";
                }

                break;
            }

            case 4: {
                m_expenseCategoryManager.viewAllExpenseCategories();

                int id;

                if (!readInt("Expense category ID: ", id)) {
                    return;
                }

                if (m_expenseCategoryManager.deleteExpenseCategory(id)) {
                    std::cout << "Expense category deleted successfully.\n";
                } else {
                    std::cout << "Expense category not found.\n";
                }

                break;
            }

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

void FinanceManager::runWalletMenu() {
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

            case 1: {
                std::string name;
                long long initialBalance;

                if (!readText("Wallet name: ", name)) {
                    return;
                }

                if (!readMoney("Initial balance (VND): ", initialBalance)) {
                    return;
                }

                if (initialBalance < 0) {
                    std::cout << "Initial balance cannot be negative.\n";
                    break;
                }

                int id = m_walletManager.addWallet(name, initialBalance);

                if (id > 0) {
                    std::cout << "Wallet added successfully. ID: " << id << "\n";
                } else {
                    std::cout << "Unable to add wallet. Check the name.\n";
                }

                break;
            }

            case 2:
                m_walletManager.viewAllWallets();
                break;

            case 3: {
                m_walletManager.viewAllWallets();

                int id;
                std::string name;

                if (!readInt("Wallet ID: ", id)) {
                    return;
                }

                if (!m_walletManager.exists(id)) {
                    std::cout << "Wallet not found.\n";
                    break;
                }

                if (!readText("New wallet name: ", name)) {
                    return;
                }

                if (m_walletManager.editWalletName(id, name)) {
                    std::cout << "Wallet updated successfully.\n";
                } else {
                    std::cout << "Unable to update wallet.\n";
                }

                break;
            }

            case 4: {
                m_walletManager.viewAllWallets();

                int id;

                if (!readInt("Wallet ID: ", id)) {
                    return;
                }

                if (m_walletManager.deleteWallet(id)) {
                    std::cout << "Wallet deleted successfully.\n";
                } else {
                    std::cout << "Wallet not found.\n";
                }

                break;
            }

            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }
}

// Transaction Operations
bool FinanceManager::addIncome() {
    if (m_walletManager.getActiveWalletCount() == 0) {
        std::cout << "Please create a wallet first.\n";
        return true;
    }

    if (m_incomeSourceManager.getActiveCount() == 0) {
        std::cout << "Please create an income source first.\n";
        return true;
    }

    int walletId;
    int sourceId;
    long long amount;
    Date date;
    std::string description;

    m_walletManager.viewAllWallets();

    if (!readInt("Wallet ID: ", walletId)) {
        return false;
    }

    if (!m_walletManager.exists(walletId)) {
        std::cout << "Wallet not found.\n";
        return true;
    }

    m_incomeSourceManager.viewAllIncomeSources();

    if (!readInt("Income source ID: ", sourceId)) {
        return false;
    }

    if (!m_incomeSourceManager.exists(sourceId)) {
        std::cout << "Income source not found.\n";
        return true;
    }

    if (!readMoney("Amount (VND): ", amount)) {
        return false;
    }

    if (amount <= 0) {
        std::cout << "Amount must be greater than zero.\n";
        return true;
    }

    if (!readDate("Date (DD/MM/YYYY): ", date)) {
        return false;
    }

    if (!readText("Description: ", description, true)) {
        return false;
    }

    Income income(Transaction::INVALID_ID, date, amount,
                  description, walletId, sourceId);

    if (m_transactionManager.addIncome(income)) {
        std::cout << "Income added successfully.\n";
        std::cout << "Current wallet balance: " << m_walletManager.getWalletBalance(walletId) << " VND\n";
    } else {
        std::cout << "Unable to add income.\n";
    }

    return true;
}

bool FinanceManager::addExpense() {
    if (m_walletManager.getActiveWalletCount() == 0) {
        std::cout << "Please create a wallet first.\n";
        return true;
    }

    if (m_expenseCategoryManager.getActiveCount() == 0) {
        std::cout << "Please create an expense category first.\n";
        return true;
    }

    int walletId;
    int categoryId;
    long long amount;
    Date date;
    std::string description;

    m_walletManager.viewAllWallets();

    if (!readInt("Wallet ID: ", walletId)) {
        return false;
    }

    if (!m_walletManager.exists(walletId)) {
        std::cout << "Wallet not found.\n";
        return true;
    }

    m_expenseCategoryManager.viewAllExpenseCategories();

    if (!readInt("Expense category ID: ", categoryId)) {
        return false;
    }

    if (!m_expenseCategoryManager.exists(categoryId)) {
        std::cout << "Expense category not found.\n";
        return true;
    }

    if (!readMoney("Amount (VND): ", amount)) {
        return false;
    }

    if (amount <= 0) {
        std::cout << "Amount must be greater than zero.\n";
        return true;
    }

    if (!m_walletManager.canWithdraw(walletId, amount)) {
        std::cout << "Insufficient wallet balance.\n";
        return true;
    }

    if (!readDate("Date (DD/MM/YYYY): ", date)) {
        return false;
    }

    if (!readText("Description: ", description, true)) {
        return false;
    }

    Expense expense(Transaction::INVALID_ID, date, amount,
                    description, walletId, categoryId);

    if (m_transactionManager.addExpense(expense)) {
        std::cout << "Expense added successfully.\n";
        std::cout << "Current wallet balance: " << m_walletManager.getWalletBalance(walletId) << " VND\n";
    } else {
        std::cout << "Unable to add expense.\n";
    }

    return true;
}

void FinanceManager::viewTransactions(TransactionType type) const {
    if (type == TransactionType::INCOME) {
        std::cout << "\n===== INCOME LIST =====\n";
    } else {
        std::cout << "\n===== EXPENSE LIST =====\n";
    }

    bool found = false;

    for (int i = 0; i < m_transactionManager.getTransactionCount(); ++i) {
        const Transaction* transaction = m_transactionManager.getTransactionAt(i);

        if (transaction && transaction->getType() == type) {
            m_transactionManager.viewTransaction(transaction->getId());
            found = true;
        }
    }

    if (!found) {
        std::cout << "No transactions found.\n";
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
