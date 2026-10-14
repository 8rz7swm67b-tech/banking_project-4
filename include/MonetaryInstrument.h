/**
 * @file MonetaryInstrument.h
 * @brief Declares the common interface for cash and checks.
 */
#ifndef MONETARY_INSTRUMENT_H
#define MONETARY_INSTRUMENT_H

/** @brief Abstract base class for a monetary instrument. */
class MonetaryInstrument {
public:
    /** @brief Allows safe destruction through a base pointer. */
    virtual ~MonetaryInstrument();
    /** @return The value represented by the instrument. */
    virtual double GetAmount() const = 0;
    /** @return True for cash; false otherwise. */
    virtual bool IsCash() const;
    /** @return True for a check; false otherwise. */
    virtual bool IsCheck() const;
};
#endif
