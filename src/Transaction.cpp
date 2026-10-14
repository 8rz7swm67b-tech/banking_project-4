/**
 * @file Transaction.cpp
 * @brief Defines transaction construction and accessors.
 */
#include "Transaction.h"
#include <stdexcept>

Transaction::Transaction(double amount, const std::string& date,
                         TransactionType type,
                         std::shared_ptr<MonetaryInstrument> instrument)
    : amount_(amount), date_(date), type_(type), instrument_(instrument) {
    if (instrument_ == nullptr) {
        throw std::invalid_argument("A transaction needs an instrument.");
    }
}

double Transaction::GetAmount() const {
    return amount_;
}

std::string Transaction::GetDate() const {
    return date_;
}

TransactionType Transaction::GetType() const {
    return type_;
}

MonetaryInstrument& Transaction::GetInstrument() const {
    return *instrument_;
}
