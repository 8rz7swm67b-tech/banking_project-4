/**
 * @file Check.h
 * @brief Declares a check and its identifying information.
 */
#ifndef CHECK_H
#define CHECK_H
#include "MonetaryInstrument.h"
#include <string>

/** @brief Stores a check amount, check number, and issuing bank. */
class Check : public MonetaryInstrument {
private:
    double amount_;
    std::string checkNumber_;
    std::string issuingBank_;
public:
    /**
     * @brief Creates a check.
     * @param amount Check value in dollars.
     * @param number Check number.
     * @param bank Name of the issuing bank.
     * @throws std::invalid_argument If the amount is negative.
     */
    Check(double amount, const std::string& number, const std::string& bank);
    /** @return The value of the check. */
    double GetAmount() const override;
    /** @return True because this instrument is a check. */
    bool IsCheck() const override;
};
#endif
