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
    int readChoice() const;
    void run(const TransactionManager& transactions) const;
    void show() const;

private:
    bool readDate(const char* prompt, Date& date) const;
    bool showDateSummary(const TransactionManager& transactions) const;
};