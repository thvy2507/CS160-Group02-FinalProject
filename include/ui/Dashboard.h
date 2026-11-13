/**
 * @file Dashboard.h
 * @author TV4
 * @brief Week 1 statistics and dashboard interface skeleton.
 * @note Proposed interfaces; shared model APIs require team review.
 */
#pragma once

class Wallet;
template <typename T> class DynamicArray;

class Dashboard {
public:
    // Returns false until real wallet rendering is implemented.
    bool show(const DynamicArray<Wallet>& wallets) const;
    void showPreview() const;
};
