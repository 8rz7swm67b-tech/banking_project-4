/**
 * @file Account.cpp
 * @brief Applies common checks and computes balances from history.
 */
#include "Account.h"
#include <stdexcept>

Account::Account(double initialBalance) : initialBalance_(initialBalance) {
    if (initialBalance < 0.0) {
        throw std::invalid_argument("Initial balance cannot be negative.");
    }
}

Account::~Account() {}

bool Account::Deposit(const Transaction& transaction) {
    if (transaction.GetType() != TransactionType::Deposit ||
        transaction.GetAmount() <= 0.0) {
        return false;
    }
    if (!CanDeposit(transaction.GetInstrument())) {
        return false;
    }
    transactions_.push_back(transaction);
    return true;
}

bool Account::Withdraw(const Transaction& transaction) {
    if (transaction.GetType() != TransactionType::Withdrawal ||
        transaction.GetAmount() <= 0.0) {
        return false;
    }
    if (!CanWithdraw(transaction.GetInstrument())) {
        return false;
    }
    if (transaction.GetAmount() > GetBalance()) {
        return false;
    }
    transactions_.push_back(transaction);
    return true;
}

double Account::GetBalance() const {
    double balance = initialBalance_;
    for (const Transaction& transaction : transactions_) {
        if (transaction.GetType() == TransactionType::Deposit) {
            balance += transaction.GetAmount();
        } else {
            balance -= transaction.GetAmount();
        }
    }
    return balance;
}

const std::vector<Transaction>& Account::GetTransactions() const {
    return transactions_;
}
