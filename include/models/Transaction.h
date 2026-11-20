/**
 * @file Transaction.h
 * @author TV3
 * @brief Base model for a single income or expense transaction
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
    int getId() const;
    Date getDate() const;
    long long getAmount() const;
    std::string getDescription() const;
    int getWalletId() const;
    virtual TransactionType getType() const = 0;

    // Setters
    void setId(int id);
    void setDate(const Date& date);
    void setAmount(long long amount);
    void setDescription(const std::string& description);
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
