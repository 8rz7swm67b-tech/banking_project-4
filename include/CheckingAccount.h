/**
 * @file CheckingAccount.h
 * @brief Declares the checking account policy.
 */
#ifndef CHECKINGACCOUNT_H
#define CHECKINGACCOUNT_H
#include "Account.h"

/** @brief Implements the checking account rules. */
class CheckingAccount : public Account {
protected:
    /** @param instrument Proposed instrument. @return Whether a deposit is allowed. */
    bool CanDeposit(const MonetaryInstrument& instrument) const override;
    /** @param instrument Proposed instrument. @return Whether a withdrawal is allowed. */
    bool CanWithdraw(const MonetaryInstrument& instrument) const override;
public:
    /**
     * @brief Creates a checking account.
     * @param initialBalance Starting dollar amount.
     * @throws std::invalid_argument If the initial balance is negative.
     */
    CheckingAccount(double initialBalance);
    /** @return "Checking". */
    std::string GetAccountType() const override;
};
#endif
