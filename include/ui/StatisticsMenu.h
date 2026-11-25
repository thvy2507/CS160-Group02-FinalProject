/**
 * @file StatisticsMenu.h
 * @author TV4
 * @brief Declares date-range and wallet statistics reports.
 */
#pragma once

class Date;
class TransactionManager;
class WalletManager;

class StatisticsMenu {
public:
    void show() const;
    int readChoice() const;
    void run(const TransactionManager& transactions, const WalletManager& wallets) const;

private:
    bool readDate(const char* prompt, Date& date) const;
    bool readDateRange(Date& start, Date& end) const;
    bool showDateSummary(const TransactionManager& transactions) const;
    bool showWalletSummary(const TransactionManager& transactions,
                           const WalletManager& wallets) const;
};