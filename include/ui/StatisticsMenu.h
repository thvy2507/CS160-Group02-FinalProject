/**
 * @file StatisticsMenu.h
 * @author TV4
 * @brief Week 1 statistics and dashboard interface skeleton.
 * @note Proposed interfaces; shared model APIs require team review.
 */
#pragma once

class StatisticsMenu {
public:
    // Navigation only: FinanceManager supplies data and dispatches reports.
    int readChoice() const;
    void show() const;
};
