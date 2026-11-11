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
    Transaction();
    Transaction(int id, const Date& date, double amount,
                const std::string& description, int walletId);
    virtual ~Transaction();

    double getAmount() const;
    Date getDate() const;
    std::string getDescription() const;
    int getId() const;
    virtual TransactionType getType() const = 0;
    int getWalletId() const;

    void setAmount(double amount);
    void setDate(const Date& date);
    void setDescription(const std::string& description);
    void setId(int id);
    void setWalletId(int walletId);

private:
    int m_id;
    Date m_date;
    double m_amount;
    std::string m_description;
    int m_walletId;
};

#endif