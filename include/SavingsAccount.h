/**
 * @file SavingsAccount.h
 * @brief Declares the savings account policy.
 */
#ifndef SAVINGSACCOUNT_H
#define SAVINGSACCOUNT_H
#include "Account.h"

/** @brief Implements the savings account rules. */
class SavingsAccount : public Account {
protected:
    /** @param instrument Proposed instrument. @return Whether a deposit is allowed. */
    bool CanDeposit(const MonetaryInstrument& instrument) const override;
    /** @param instrument Proposed instrument. @return Whether a withdrawal is allowed. */
    bool CanWithdraw(const MonetaryInstrument& instrument) const override;
public:
    /**
     * @brief Creates a savings account.
     * @param initialBalance Starting dollar amount.
     * @throws std::invalid_argument If the initial balance is negative.
     */
    SavingsAccount(double initialBalance);
    /** @return "Savings". */
    std::string GetAccountType() const override;
};
#endif
