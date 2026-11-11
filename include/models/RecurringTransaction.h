/**
 * @file RecurringTransaction.h
 * @author TV3
 * @brief Template for a monthly recurring income or expense
 * @note Week 1 - interface only. Generation is not implemented yet
 */

#ifndef RECURRING_TRANSACTION_H
#define RECURRING_TRANSACTION_H

#include <string>

#include "core/Date.h"
#include "models/Transaction.h"

const int MIN_DAY_OF_MONTH = 1;
const int MAX_DAY_OF_MONTH = 31;

class RecurringTransaction {
public:
    RecurringTransaction();
    RecurringTransaction(int id, TransactionType type, double amount,
                         const std::string& description, int walletId,
                         int sourceOrCategoryId, const Date& startDate,
                         int dayOfMonth);

    void clearEndDate();
    bool hasEndDate() const;
    bool hasLastGeneratedDate() const;
    bool isActive() const;

    double getAmount() const;
    int getDayOfMonth() const;
    std::string getDescription() const;
    Date getEndDate() const;
    int getId() const;
    Date getLastGeneratedDate() const;
    int getSourceOrCategoryId() const;
    Date getStartDate() const;
    TransactionType getType() const;
    int getWalletId() const;

    void setActive(bool isActive);
    void setAmount(double amount);
    void setDayOfMonth(int dayOfMonth);
    void setDescription(const std::string& description);
    void setEndDate(const Date& endDate);
    void setId(int id);
    void setLastGeneratedDate(const Date& date);
    void setSourceOrCategoryId(int sourceOrCategoryId);
    void setStartDate(const Date& startDate);
    void setWalletId(int walletId);

private:
    int m_id;
    TransactionType m_type;
    double m_amount;
    std::string m_description;
    int m_walletId;
    int m_sourceOrCategoryId;
    Date m_startDate;
    bool m_hasEndDate;
    Date m_endDate;
    int m_dayOfMonth;
    bool m_hasLastGeneratedDate;
    Date m_lastGeneratedDate;
    bool m_isActive;
};

#endif