/**
 * @file SavingsAccount.cpp
 * @brief Defines the savings account rules.
 */
#include "SavingsAccount.h"

SavingsAccount::SavingsAccount(double initialBalance) : Account(initialBalance) {}

bool SavingsAccount::CanDeposit(const MonetaryInstrument& instrument) const {
    return instrument.IsCash();
}

bool SavingsAccount::CanWithdraw(const MonetaryInstrument& instrument) const {
    return instrument.IsCash();
}

std::string SavingsAccount::GetAccountType() const {
    return "Savings";
}
