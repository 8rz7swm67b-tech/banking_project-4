/**
 * @file Cash.h
 * @brief Declares cash with a denomination breakdown.
 */
#ifndef CASH_H
#define CASH_H
#include "MonetaryInstrument.h"

/** @brief Stores the number of twenty, ten, five, and one dollar bills. */
class Cash : public MonetaryInstrument {
private:
    int twenties_;
    int tens_;
    int fives_;
    int ones_;
public:
    /**
     * @brief Creates a cash instrument.
     * @param twenties Number of twenty dollar bills.
     * @param tens Number of ten dollar bills.
     * @param fives Number of five dollar bills.
     * @param ones Number of one dollar bills.
     * @throws std::invalid_argument If any bill count is negative.
     */
    Cash(int twenties, int tens, int fives, int ones);
    /** @return The total dollar value of all bills. */
    double GetAmount() const override;
    /** @return True because this instrument is cash. */
    bool IsCash() const override;
};
#endif
