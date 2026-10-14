/**
 * @file CheckingAccount.cpp
 * @brief Defines the checking account rules.
 */
#include "CheckingAccount.h"

CheckingAccount::CheckingAccount(double initialBalance) : Account(initialBalance) {}

bool CheckingAccount::CanDeposit(const MonetaryInstrument& instrument) const {
    return instrument.IsCash() || instrument.IsCheck();
}

bool CheckingAccount::CanWithdraw(const MonetaryInstrument& instrument) const {
    return instrument.IsCash();
}

std::string CheckingAccount::GetAccountType() const {
    return "Checking";
}
