/**
 * @file Cash.cpp
 * @brief Computes the value of a group of bills.
 */
#include "Cash.h"
#include <stdexcept>

Cash::Cash(int twenties, int tens, int fives, int ones)
    : twenties_(twenties), tens_(tens), fives_(fives), ones_(ones) {
    if (twenties < 0 || tens < 0 || fives < 0 || ones < 0) {
        throw std::invalid_argument("Bill counts cannot be negative.");
    }
}

double Cash::GetAmount() const {
    return 20.0 * twenties_ + 10.0 * tens_ + 5.0 * fives_ + ones_;
}

bool Cash::IsCash() const {
    return true;
}
