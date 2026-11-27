/**
 * @file RecurringTransactionManager.cpp
 * @author TV3
 * @brief Manages monthly recurring transactions and generates the due ones
 * @note Week 3 - in-memory only; binary persistence is planned for a later week
 */

#include "managers/RecurringTransactionManager.h"

#include <iostream>

#include "managers/ExpenseCategoryManager.h"
#include "managers/IncomeSourceManager.h"
#include "managers/TransactionManager.h"
#include "managers/WalletManager.h"
#include "models/Expense.h"
#include "models/Income.h"

namespace {

const int NOT_FOUND = -1;
const int FIRST_RECURRING_ID = 1;
const int FIRST_MONTH = 1;
const int FEBRUARY = 2;
const int MONTHS_PER_YEAR = 12;
const int DAYS_IN_MONTH[MONTHS_PER_YEAR] = {31, 28, 31, 30, 31, 30,
                                            31, 31, 30, 31, 30, 31};

// ---------- Date helpers ----------

// Returns how many days a month has (February depends on the leap year)
int getDaysInMonth(int month, int year) {
    int days = DAYS_IN_MONTH[month - 1];
    const Date firstDay(1, month, year);
    if (month == FEBRUARY && firstDay.isLeapYear()) {
        ++days;
    }
    return days;
}

// Builds the date of one occurrence.
// If the chosen day is too big for the month (e.g. 31 in April), use the last day
Date makeOccurrenceDate(int dayOfMonth, int month, int year) {
    const int lastDay = getDaysInMonth(month, year);
    const int day = (dayOfMonth < lastDay) ? dayOfMonth : lastDay;
    return Date(day, month, year);
}

void moveToNextMonth(int& month, int& year) {
    ++month;
    if (month > MONTHS_PER_YEAR) {
        month = FIRST_MONTH;
        ++year;
    }
}

// ---------- Validation ----------

// Checks the amount, day of month and dates (no data is modified)
bool hasValidScheduleFields(const RecurringTransaction& recurring) {
    if (recurring.getAmount() <= 0) {
        return false;
    }
    if (recurring.getDayOfMonth() < MIN_DAY_OF_MONTH ||
        recurring.getDayOfMonth() > MAX_DAY_OF_MONTH) {
        return false;
    }
    if (!recurring.getStartDate().isValid()) {
        return false;
    }
    if (recurring.hasEndDate()) {
        if (!recurring.getEndDate().isValid() ||
            recurring.getEndDate() < recurring.getStartDate()) {
            return false;
        }
    }
    if (recurring.hasLastGeneratedDate() &&
        !recurring.getLastGeneratedDate().isValid()) {
        return false;
    }
    return true;
}

// Checks that the wallet and the income source / expense category exist
bool hasValidReferences(const RecurringTransaction& recurring,
                        const WalletManager* walletManager,
                        const IncomeSourceManager* incomeSourceManager,
                        const ExpenseCategoryManager* expenseCategoryManager) {
    if (walletManager == nullptr || !walletManager->exists(recurring.getWalletId())) {
        return false;
    }

    const int referenceId = recurring.getSourceOrCategoryId();
    if (recurring.getType() == TransactionType::INCOME) {
        return incomeSourceManager != nullptr && incomeSourceManager->exists(referenceId);
    }
    return expenseCategoryManager != nullptr && expenseCategoryManager->exists(referenceId);
}

// ---------- Copy and print ----------

// Makes a clean copy with the given ID.
// The last generated date is NOT copied here; the caller decides what to keep
RecurringTransaction copyWithId(const RecurringTransaction& source, int id) {
    RecurringTransaction copy(id, source.getType(), source.getAmount(),
                              source.getDescription(), source.getWalletId(),
                              source.getSourceOrCategoryId(), source.getStartDate(),
                              source.getDayOfMonth());
    if (source.hasEndDate()) {
        copy.setEndDate(source.getEndDate());
    }
    copy.setActive(source.isActive());
    return copy;
}

void printDate(const Date& date) {
    std::cout << date.getDay() << "/" << date.getMonth() << "/" << date.getYear();
}

void printRecurring(const RecurringTransaction& recurring) {
    const char* typeName =
        (recurring.getType() == TransactionType::INCOME) ? "Income" : "Expense";

    std::cout << "ID: " << recurring.getId()
              << " | " << typeName
              << " | Day " << recurring.getDayOfMonth() << " of each month"
              << " | Wallet: " << recurring.getWalletId()
              << " | " << recurring.getAmount() << " VND"
              << " | " << recurring.getDescription()
              << " | Start: ";
    printDate(recurring.getStartDate());

    std::cout << " | End: ";
    if (recurring.hasEndDate()) {
        printDate(recurring.getEndDate());
    } else {
        std::cout << "none";
    }

    std::cout << " | Last generated: ";
    if (recurring.hasLastGeneratedDate()) {
        printDate(recurring.getLastGeneratedDate());
    } else {
        std::cout << "never";
    }

    std::cout << " | " << (recurring.isActive() ? "Active" : "Inactive") << "\n";
}

// ---------- Generation helpers ----------

// A date is generated only if it is not before the start date
// and it is after the last generated date (this prevents duplicates)
bool shouldGenerate(const RecurringTransaction& recurring, const Date& occurrence) {
    if (occurrence < recurring.getStartDate()) {
        return false;
    }
    if (recurring.hasLastGeneratedDate() &&
        occurrence <= recurring.getLastGeneratedDate()) {
        return false;
    }
    return true;
}

// Creates one real Income or Expense through the TransactionManager.
// The TransactionManager checks the IDs and updates the wallet balance
bool addGeneratedTransaction(const RecurringTransaction& recurring,
                             const Date& date,
                             TransactionManager& transactionManager) {
    if (recurring.getType() == TransactionType::INCOME) {
        const Income income(Transaction::INVALID_ID, date, recurring.getAmount(),
                            recurring.getDescription(), recurring.getWalletId(),
                            recurring.getSourceOrCategoryId());
        return transactionManager.addIncome(income);
    }

    const Expense expense(Transaction::INVALID_ID, date, recurring.getAmount(),
                          recurring.getDescription(), recurring.getWalletId(),
                          recurring.getSourceOrCategoryId());
    return transactionManager.addExpense(expense);
}

// Goes month by month and generates every due transaction of ONE recurring item.
// Returns how many transactions were created
int generateForOne(RecurringTransaction& recurring, const Date& today,
                   TransactionManager& transactionManager) {
    if (!recurring.isActive()) {
        return 0;
    }

    // Start at the start month, or at the month after the last generated one
    int month = recurring.getStartDate().getMonth();
    int year = recurring.getStartDate().getYear();
    if (recurring.hasLastGeneratedDate()) {
        month = recurring.getLastGeneratedDate().getMonth();
        year = recurring.getLastGeneratedDate().getYear();
        moveToNextMonth(month, year);
    }

    int generatedCount = 0;
    while (true) {
        const Date occurrence = makeOccurrenceDate(recurring.getDayOfMonth(), month, year);

        // Stop when the date is in the future or after the end date
        if (occurrence > today) {
            break;
        }
        if (recurring.hasEndDate() && occurrence > recurring.getEndDate()) {
            break;
        }

        if (shouldGenerate(recurring, occurrence)) {
            // On failure (e.g. not enough money) stop and keep the last good date,
            // so the failed month can be tried again next time
            if (!addGeneratedTransaction(recurring, occurrence, transactionManager)) {
                break;
            }
            recurring.setLastGeneratedDate(occurrence);
            ++generatedCount;
        }
        moveToNextMonth(month, year);
    }
    return generatedCount;
}

}  // namespace

// Constructors & Destructor
RecurringTransactionManager::RecurringTransactionManager(
    WalletManager* walletManager, IncomeSourceManager* incomeSourceManager,
    ExpenseCategoryManager* expenseCategoryManager)
    : m_recurringTransactions(),
      m_nextId(FIRST_RECURRING_ID),
      m_walletManager(walletManager),
      m_incomeSourceManager(incomeSourceManager),
      m_expenseCategoryManager(expenseCategoryManager) {}

RecurringTransactionManager::~RecurringTransactionManager() {}

// CRUD Operations
bool RecurringTransactionManager::createRecurringTransaction(
    const RecurringTransaction& recurring) {
    // Validate first, so a failed call changes nothing
    if (!hasValidScheduleFields(recurring) ||
        !hasValidReferences(recurring, m_walletManager, m_incomeSourceManager,
                            m_expenseCategoryManager)) {
        return false;
    }

    RecurringTransaction record = copyWithId(recurring, m_nextId);
    if (recurring.hasLastGeneratedDate()) {
        record.setLastGeneratedDate(recurring.getLastGeneratedDate());
    }

    m_recurringTransactions.add(record);
    ++m_nextId;
    return true;
}

bool RecurringTransactionManager::deleteRecurringTransaction(int id) {
    const int index = findRecurringIndex(id);
    if (index == NOT_FOUND) {
        return false;
    }

    m_recurringTransactions.remove(index);
    return true;
}

bool RecurringTransactionManager::editRecurringTransaction(
    int id, const RecurringTransaction& updated) {
    const int index = findRecurringIndex(id);
    if (index == NOT_FOUND) {
        return false;
    }
    if (!hasValidScheduleFields(updated) ||
        !hasValidReferences(updated, m_walletManager, m_incomeSourceManager,
                            m_expenseCategoryManager)) {
        return false;
    }

    // Keep the old last generated date, so editing never causes duplicates
    RecurringTransaction record = copyWithId(updated, id);
    const RecurringTransaction& existing = m_recurringTransactions[index];
    if (existing.hasLastGeneratedDate()) {
        record.setLastGeneratedDate(existing.getLastGeneratedDate());
    }

    m_recurringTransactions[index] = record;
    return true;
}

void RecurringTransactionManager::viewAllRecurringTransactions() const {
    if (m_recurringTransactions.empty()) {
        std::cout << "No recurring transactions.\n";
        return;
    }

    for (int i = 0; i < m_recurringTransactions.size(); ++i) {
        printRecurring(m_recurringTransactions[i]);
    }
}

bool RecurringTransactionManager::viewRecurringTransaction(int id) const {
    const RecurringTransaction* recurring = getRecurringTransactionById(id);
    if (recurring == nullptr) {
        return false;
    }

    printRecurring(*recurring);
    return true;
}

// Search & Access
const RecurringTransaction* RecurringTransactionManager::getRecurringTransactionById(
    int id) const {
    const int index = findRecurringIndex(id);
    if (index == NOT_FOUND) {
        return nullptr;
    }
    return &m_recurringTransactions[index];
}

int RecurringTransactionManager::getRecurringTransactionCount() const {
    return m_recurringTransactions.size();
}

// Domain-specific Operations
int RecurringTransactionManager::generateDueTransactions(
    const Date& today, TransactionManager& transactionManager) {
    int totalGenerated = 0;
    for (int i = 0; i < m_recurringTransactions.size(); ++i) {
        totalGenerated += generateForOne(m_recurringTransactions[i], today,
                                         transactionManager);
    }
    return totalGenerated;
}

// Private Helpers
int RecurringTransactionManager::findRecurringIndex(int id) const {
    for (int i = 0; i < m_recurringTransactions.size(); ++i) {
        if (m_recurringTransactions[i].getId() == id) {
            return i;
        }
    }
    return NOT_FOUND;
}