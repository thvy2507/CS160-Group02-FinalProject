/**
 * @file Dashboard.cpp
 * @author TV4
 * @brief Week 1 statistics and dashboard interface skeleton.
 * @note Proposed interfaces; shared model APIs require team review.
 */
#include "ui/Dashboard.h"

#include <iostream>

bool Dashboard::show(const DynamicArray<Wallet>&) const {
    return false;
}

void Dashboard::showPreview() const {
    std::cout << "=== DASHBOARD: DESIGN PREVIEW ===\n"
              << "Total balance: [sum of current wallet balances]\n"
              << "Wallet name | Current balance (VND)\n"
              << "[wallet]    | [balance]\n"
              << "No wallets: No wallets available; total 0 VND.\n";
}
