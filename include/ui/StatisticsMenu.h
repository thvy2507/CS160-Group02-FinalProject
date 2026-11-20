/**
 * @file StatisticsMenu.h
 * @author TV4
 * @brief Declares date-range reporting and statistics navigation.
 */
#pragma once

class Date;
class TransactionManager;

class StatisticsMenu {
public:
    // Menu & Navigation
    void show() const;
    int readChoice() const;
    void run(const TransactionManager& transactions) const;

private:
    // Private Helpers
    bool readDate(const char* prompt, Date& date) const;
    bool showDateSummary(const TransactionManager& transactions) const;
};
