/**
 * @file StatisticsManager.cpp
 * @author TV4
 * @brief Provides financial reports and statistics.
 * @note Supports date-range summaries for all wallets or one active wallet.
 */

#include "statistics/StatisticsManager.h"

#include <limits>
#include <optional>

#include "core/Date.h"
#include "core/DynamicArray.h"
#include "managers/TransactionManager.h"
#include "models/Transaction.h"
#include "models/Wallet.h"

namespace {

bool calculateSummaryForRange(
    const TransactionManager& transactions,
    const Date& start, const Date& end,
    StatisticsResult& result,
    std::optional<int> walletIdFilter) {
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

        if (walletIdFilter && transaction->getWalletId() != *walletIdFilter) {
            continue;
        }

        const Date date = transaction->getDate();

        if (!date.isValid()) {
            return false;
        }

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

} // namespace

// Statistics Operations
bool StatisticsManager::calculateSummary(
    const TransactionManager& transactions,
    const Date& start, const Date& end,
    StatisticsResult& result) const {
    return calculateSummaryForRange(
        transactions, start, end, result, std::nullopt);
}

bool StatisticsManager::calculateWalletSummary(
    const TransactionManager& transactions,
    const Date& start, const Date& end, int walletId,
    const DynamicArray<Wallet>& wallets, StatisticsResult& result) const {
    if (walletId <= Wallet::INVALID_ID) {
        return false;
    }

    for (int i = 0; i < wallets.getSize(); ++i) {
        const Wallet& wallet = wallets[i];
        if (wallet.getId() == walletId && !wallet.isDeleted()) {
            return calculateSummaryForRange(
                transactions, start, end, result, walletId);
        }
    }

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
