/**
 * @file Check.cpp
 * @brief Defines check construction and queries.
 */
#include "Check.h"
#include <stdexcept>

Check::Check(double amount, const std::string& number, const std::string& bank)
    : amount_(amount), checkNumber_(number), issuingBank_(bank) {
    if (amount < 0.0) {
        throw std::invalid_argument("Check amount cannot be negative.");
    }
}

double Check::GetAmount() const {
    return amount_;
}

bool Check::IsCheck() const {
    return true;
}
