/**
 * @file StatisticsManager.h
 * @author TV4
 * @brief Week 1 statistics and dashboard interface skeleton.
 * @note Proposed interfaces; shared model APIs require team review.
 */
#pragma once

class Date;
class Transaction;
class Wallet;
template <typename T> class DynamicArray;

struct StatisticsResult {
    double totalIncome = 0.0;
    double totalExpense = 0.0;
    double netBalance = 0.0;
};

struct AnnualResult {
    int year = 0;
    StatisticsResult summary;
};

struct BreakdownResult {
    int id = 0;
    double amount = 0.0;
};

class StatisticsManager {
public:
    // False means unavailable or invalid; output remains unchanged.
    bool calculateAnnualOverview(const DynamicArray<Transaction>& transactions,
        const DynamicArray<int>& years, DynamicArray<AnnualResult>& result) const;
    bool calculateExpensesByCategory(const DynamicArray<Transaction>& transactions,
        const DynamicArray<int>& years, DynamicArray<BreakdownResult>& result) const;
    bool calculateIncomeBySource(const DynamicArray<Transaction>& transactions,
        const DynamicArray<int>& years, DynamicArray<BreakdownResult>& result) const;
    bool calculateSummary(const DynamicArray<Transaction>& transactions,
        const Date& start, const Date& end, StatisticsResult& result) const;
    bool calculateWalletSummary(const DynamicArray<Transaction>& transactions,
        const Date& start, const Date& end, int walletId,
        const DynamicArray<Wallet>& wallets, StatisticsResult& result) const;
};
