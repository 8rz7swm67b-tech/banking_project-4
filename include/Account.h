/**
 * @file Account.h
 * @brief Declares an abstract account with a transaction history.
 */
#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "Transaction.h"
#include <string>
#include <vector>

/** @brief Stores the initial balance and accepted transactions. */
class Account {
private:
    double initialBalance_;
    std::vector<Transaction> transactions_;
protected:
    /**
     * @param instrument Instrument proposed for a deposit.
     * @return True if the account policy permits it.
     */
    virtual bool CanDeposit(const MonetaryInstrument& instrument) const = 0;
    /**
     * @param instrument Instrument proposed for a withdrawal.
     * @return True if the account policy permits it.
     */
    virtual bool CanWithdraw(const MonetaryInstrument& instrument) const = 0;
public:
    /**
     * @brief Creates an account with an empty history.
     * @param initialBalance Starting dollar amount.
     * @throws std::invalid_argument If the initial balance is negative.
     */
    Account(double initialBalance);
    /** @brief Allows safe destruction through an Account pointer. */
    virtual ~Account();
    /**
     * @brief Adds an allowed positive deposit to the history.
     * @param transaction Proposed deposit.
     * @return True on success; false for a wrong type, amount, or instrument.
     */
    bool Deposit(const Transaction& transaction);
    /**
     * @brief Adds an allowed withdrawal if enough money is available.
     * @param transaction Proposed withdrawal.
     * @return True on success; false if validation or account policy fails.
     */
    bool Withdraw(const Transaction& transaction);
    /** @return Initial balance plus deposits minus withdrawals. */
    double GetBalance() const;
    /** @return Read-only access to the accepted transactions, in order. */
    const std::vector<Transaction>& GetTransactions() const;
    /** @return The name of the account type. */
    virtual std::string GetAccountType() const = 0;
};
#endif
