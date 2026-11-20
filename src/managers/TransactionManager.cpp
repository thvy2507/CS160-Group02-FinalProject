/**
 * @file TransactionManager.cpp
 * @author TV3
 * @brief Manages income and expense transactions in memory
 * @note Week 2 - add, view and search transactions; wallet balance updates go through WalletManager
 */

#include "managers/TransactionManager.h"

#include <iostream>

#include "managers/ExpenseCategoryManager.h"
#include "managers/IncomeSourceManager.h"
#include "managers/WalletManager.h"

namespace {

const int NOT_FOUND = -1;
const int FIRST_TRANSACTION_ID = 1;

// Checks the fields shared by Income and Expense (no data is modified)
bool hasValidCommonFields(const Transaction& transaction,
                          const WalletManager* walletManager) {
    if (walletManager == nullptr) {
        return false;
    }
    if (transaction.getAmount() <= 0) {
        return false;
    }
    if (!transaction.getDate().isValid()) {
        return false;
    }
    return walletManager->exists(transaction.getWalletId());
}

void printTransaction(const Transaction& transaction) {
    const Date date = transaction.getDate();
    const char* typeName =
        (transaction.getType() == TransactionType::INCOME) ? "Income" : "Expense";

    std::cout << "ID: " << transaction.getId()
              << " | " << typeName
              << " | " << date.getDay() << "/" << date.getMonth() << "/" << date.getYear()
              << " | Wallet: " << transaction.getWalletId()
              << " | " << transaction.getAmount() << " VND"
              << " | " << transaction.getDescription()
              << "\n";
}

}  // namespace

// Constructors & Destructor
TransactionManager::TransactionManager(WalletManager* walletManager,
                                       IncomeSourceManager* incomeSourceManager,
                                       ExpenseCategoryManager* expenseCategoryManager)
    : m_transactions(),
      m_nextId(FIRST_TRANSACTION_ID),
      m_walletManager(walletManager),
      m_incomeSourceManager(incomeSourceManager),
      m_expenseCategoryManager(expenseCategoryManager) {}

TransactionManager::~TransactionManager() {
    for (int i = 0; i < m_transactions.size(); ++i) {
        delete m_transactions[i];
    }
}

// CRUD Operations
bool TransactionManager::addIncome(const Income& income) {
    if (!hasValidCommonFields(income, m_walletManager)) {
        return false;
    }
    if (m_incomeSourceManager == nullptr ||
        !m_incomeSourceManager->exists(income.getIncomeSourceId())) {
        return false;
    }

    Income* record = new Income(m_nextId, income.getDate(), income.getAmount(),
                                income.getDescription(), income.getWalletId(),
                                income.getIncomeSourceId());
    m_transactions.add(record);

    if (!m_walletManager->deposit(income.getWalletId(), income.getAmount())) {
        m_transactions.remove(m_transactions.size() - 1);
        delete record;
        return false;
    }

    ++m_nextId;
    return true;
}

bool TransactionManager::addExpense(const Expense& expense) {
    if (!hasValidCommonFields(expense, m_walletManager)) {
        return false;
    }
    if (m_expenseCategoryManager == nullptr ||
        !m_expenseCategoryManager->exists(expense.getExpenseCategoryId())) {
        return false;
    }

    Expense* record = new Expense(m_nextId, expense.getDate(), expense.getAmount(),
                                  expense.getDescription(), expense.getWalletId(),
                                  expense.getExpenseCategoryId());
    m_transactions.add(record);

    // withdraw() rejects insufficient funds, so the balance is unchanged on failure
    if (!m_walletManager->withdraw(expense.getWalletId(), expense.getAmount())) {
        m_transactions.remove(m_transactions.size() - 1);
        delete record;
        return false;
    }

    ++m_nextId;
    return true;
}

void TransactionManager::viewAllTransactions() const {
    if (m_transactions.empty()) {
        std::cout << "No transactions.\n";
        return;
    }

    for (int i = 0; i < m_transactions.size(); ++i) {
        printTransaction(*m_transactions[i]);
    }
}

bool TransactionManager::viewTransaction(int id) const {
    const Transaction* transaction = getTransactionById(id);
    if (transaction == nullptr) {
        return false;
    }

    printTransaction(*transaction);
    return true;
}

// Search & Access
const Transaction* TransactionManager::getTransactionAt(int index) const {
    if (index < 0 || index >= m_transactions.size()) {
        return nullptr;
    }
    return m_transactions[index];
}

const Transaction* TransactionManager::getTransactionById(int id) const {
    int index = findTransactionIndex(id);
    if (index == NOT_FOUND) {
        return nullptr;
    }
    return m_transactions[index];
}

int TransactionManager::getTransactionCount() const {
    return m_transactions.size();
}

// Private Helpers
int TransactionManager::findTransactionIndex(int id) const {
    for (int i = 0; i < m_transactions.size(); ++i) {
        if (m_transactions[i]->getId() == id) {
            return i;
        }
    }
    return NOT_FOUND;
}
