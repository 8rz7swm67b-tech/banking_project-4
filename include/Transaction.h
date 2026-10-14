/**
 * @file Transaction.h
 * @brief Declares deposits and withdrawals.
 */
#ifndef TRANSACTION_H
#define TRANSACTION_H
#include "MonetaryInstrument.h"
#include <memory>
#include <string>

/** @brief Identifies the two supported transaction types. */
enum class TransactionType {
    Deposit,
    Withdrawal
};

/** @brief Stores the details of one transaction and its instrument. */
class Transaction {
private:
    double amount_;
    std::string date_;
    TransactionType type_;
    std::shared_ptr<MonetaryInstrument> instrument_;
public:
    /**
     * @brief Creates a transaction record.
     * @param amount Amount to deposit or withdraw.
     * @param date Date supplied by the caller.
     * @param type Deposit or Withdrawal.
     * @param instrument Cash or check used for the transaction.
     * @throws std::invalid_argument If the instrument is null.
     */
    Transaction(double amount, const std::string& date, TransactionType type,
                std::shared_ptr<MonetaryInstrument> instrument);
    /** @return The transaction amount. */
    double GetAmount() const;
    /** @return The date stored in the record. */
    std::string GetDate() const;
    /** @return Deposit or Withdrawal. */
    TransactionType GetType() const;
    /** @return A reference to the stored instrument. */
    MonetaryInstrument& GetInstrument() const;
};
#endif
