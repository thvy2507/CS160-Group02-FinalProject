/**
 * @file StatisticsManager.cpp
 * @author TV4
 * @brief Provides financial reports and statistics.
 * @note Sprint 2 implements date-range summaries.
 * Other reports are deferred to later sprints.
 */

#include "statistics/StatisticsManager.h"

#include <limits>

#include "core/Date.h"
#include "managers/TransactionManager.h"
#include "models/Transaction.h"

// Statistics Operations
bool StatisticsManager::calculateSummary(
    const TransactionManager& transactions,
    const Date& start, const Date& end,
    StatisticsResult& result) const {
    if (!start.isValid() || !end.isValid() || start > end) {
        return false;
    }

    StatisticsResult summary;
    const long long MAX_AMOUNT = std::numeric_limits<long long>::max();

    for (int i = 0; i < transactions.getTransactionCount(); ++i) {
        const Transaction* transaction = transactions.getTransactionAt(i);
        if (transaction == nullptr) {
            return false;
        }

        Date date = transaction->getDate();
        if (date < start || date > end) {
            continue;
        }

        long long amount = transaction->getAmount();
        if (amount <= 0) {
            return false;
        }

        if (transaction->getType() == TransactionType::INCOME) {
            if (amount > MAX_AMOUNT - summary.totalIncome) {
                return false;
            }
            summary.totalIncome += amount;
        } else if (transaction->getType() == TransactionType::EXPENSE) {
            if (amount > MAX_AMOUNT - summary.totalExpense) {
                return false;
            }
            summary.totalExpense += amount;
        } else {
            return false;
        }
    }

    summary.netBalance = summary.totalIncome - summary.totalExpense;
    result = summary;
    return true;
}

bool StatisticsManager::calculateWalletSummary(
    const TransactionManager&, const Date&, const Date&, int,
    const DynamicArray<Wallet>&, StatisticsResult&) const {
    return false;
}

bool StatisticsManager::calculateAnnualOverview(
    const TransactionManager&, const DynamicArray<int>&,
    DynamicArray<AnnualResult>&) const {
    return false;
}

bool StatisticsManager::calculateIncomeBySource(
    const TransactionManager&, const DynamicArray<int>&,
    DynamicArray<BreakdownResult>&) const {
    return false;
}

bool StatisticsManager::calculateExpensesByCategory(
    const TransactionManager&, const DynamicArray<int>&,
    DynamicArray<BreakdownResult>&) const {
    return false;
}
