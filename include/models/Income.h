/**
 * @file Income.h
 * @author TV3
 * @brief Income transaction linked to an income source
 * @note Week 1 - interface only
 */

#ifndef INCOME_H
#define INCOME_H

#include "models/Transaction.h"

class Income : public Transaction {
public:
    // Constructors & Destructor
    Income();
    Income(int id, const Date& date, long long amount,
           const std::string& description, int walletId, int incomeSourceId);
    ~Income() override;

    // Getters
    int getIncomeSourceId() const;
    TransactionType getType() const override;

    // Setters
    void setIncomeSourceId(int incomeSourceId);

private:
    // Private Members
    int m_incomeSourceId;
};


#endif
