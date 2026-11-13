/**
 * @file StatisticsManager.h
 * @author TV4
 * @brief Week 1 statistics and dashboard interface skeleton.
 * @note Proposed interfaces; shared model APIs require team review.
 */

#pragma once

class Date;
class TransactionManager;
class Wallet;
template <typename T> class DynamicArray;

struct StatisticsResult {
    long long totalIncome = 0;
    long long totalExpense = 0;
    long long netBalance = 0;
};

struct AnnualResult {
    int year = 0;
    StatisticsResult summary;
};

struct BreakdownResult {
    int id = 0;
    long long amount = 0;
};

class StatisticsManager {
public:
    // Statistics Operations
    bool calculateAnnualOverview(const TransactionManager& transactions,
        const DynamicArray<int>& years, DynamicArray<AnnualResult>& result) const;

    bool calculateExpensesByCategory(const TransactionManager& transactions,
        const DynamicArray<int>& years, DynamicArray<BreakdownResult>& result) const;

    bool calculateIncomeBySource(const TransactionManager& transactions,
        const DynamicArray<int>& years, DynamicArray<BreakdownResult>& result) const;

    bool calculateSummary(const TransactionManager& transactions,
        const Date& start, const Date& end, StatisticsResult& result) const;

    bool calculateWalletSummary(const TransactionManager& transactions,
        const Date& start, const Date& end, int walletId,
        const DynamicArray<Wallet>& wallets, StatisticsResult& result) const;
};
