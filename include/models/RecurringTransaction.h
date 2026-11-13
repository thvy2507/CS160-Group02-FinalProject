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
    // Constants
    static constexpr int INVALID_ID = 0;

    // Constructors
    RecurringTransaction();
    RecurringTransaction(int id, TransactionType type, long long amount,
                         const std::string& description, int walletId,
                         int sourceOrCategoryId, const Date& startDate,
                         int dayOfMonth);

    // Getters
    long long getAmount() const;
    int getDayOfMonth() const;
    std::string getDescription() const;
    Date getEndDate() const;
    int getId() const;
    Date getLastGeneratedDate() const;
    int getSourceOrCategoryId() const;
    Date getStartDate() const;
    TransactionType getType() const;
    int getWalletId() const;
    bool hasEndDate() const;
    bool hasLastGeneratedDate() const;
    bool isActive() const;

    // Setters
    void setActive(bool isActive);
    void setAmount(long long amount);
    void setDayOfMonth(int dayOfMonth);
    void setDescription(const std::string& description);
    void setEndDate(const Date& endDate);
    void setId(int id);
    void setLastGeneratedDate(const Date& date);
    void setSourceOrCategoryId(int sourceOrCategoryId);
    void setStartDate(const Date& startDate);
    void setWalletId(int walletId);

    // Date Operations
    void clearEndDate();

private:
    // Private Members
    int m_id;
    TransactionType m_type;
    long long m_amount;
    std::string m_description;
    int m_walletId;
    int m_sourceOrCategoryId;
    Date m_startDate;
    bool m_hasEndDate;
    Date m_endDate;
    // Use the last day for shorter months
    int m_dayOfMonth;
    bool m_hasLastGeneratedDate;
    Date m_lastGeneratedDate;
    bool m_isActive;
};


#endif
