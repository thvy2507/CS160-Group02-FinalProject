/**
 * @file NumberFormatter.h
 * @author TV4
 * @brief Week 1 statistics and dashboard interface skeleton.
 * @note Proposed interfaces; shared model APIs require team review.
 */

#pragma once

#include <string>

class NumberFormatter {
public:
    // Proposed format: 1,234,567 VND. False leaves output unchanged.
    static bool formatMoney(long long amount, std::string& output);
};
