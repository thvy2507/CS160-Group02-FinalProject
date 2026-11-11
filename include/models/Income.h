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
    Income();
    Income(int id, const Date& date, double amount,
           const std::string& description, int walletId, int incomeSourceId);
    ~Income() override;

    int getIncomeSourceId() const;
    TransactionType getType() const override;
    void setIncomeSourceId(int incomeSourceId);

private:
    int m_incomeSourceId;
};

#endif