/**
 * @file MonetaryInstrument.cpp
 * @brief Defines the default monetary instrument queries.
 */
#include "MonetaryInstrument.h"

MonetaryInstrument::~MonetaryInstrument() {}

bool MonetaryInstrument::IsCash() const {
    return false;
}

bool MonetaryInstrument::IsCheck() const {
    return false;
}
