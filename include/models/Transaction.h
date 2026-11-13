/**
 * @file Transaction.h
 * @author TV3
 * @brief Base model for a single income or expense transaction
 * @note Week 1 - interface only
 */

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

#include "core/Date.h"

enum class TransactionType { INCOME, EXPENSE };


class Transaction {
public:
    // Constants
    static constexpr int INVALID_ID = 0;

    // Constructors & Destructor
    Transaction();
    Transaction(int id, const Date& date, long long amount,
                const std::string& description, int walletId);
    virtual ~Transaction();

    // Getters
    long long getAmount() const;
    Date getDate() const;
    std::string getDescription() const;
    int getId() const;
    virtual TransactionType getType() const = 0;
    int getWalletId() const;

    // Setters
    void setAmount(long long amount);
    void setDate(const Date& date);
    void setDescription(const std::string& description);
    void setId(int id);
    void setWalletId(int walletId);

private:
    // Private Members
    int m_id;
    Date m_date;
    long long m_amount;
    std::string m_description;
    int m_walletId;
};

#endif
