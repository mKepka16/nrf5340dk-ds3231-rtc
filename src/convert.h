#ifndef CONVERT_H
#define CONVERT_H

#include <stdint.h>

#define bcd_to_dec(val) bcd_to_dec_bits(val, 8)

// Converts a decimal value (0-99) to its BCD (Binary-Coded Decimal) representation.
uint8_t dec_to_bcd(uint8_t val);

// Converts a BCD (Binary-Coded Decimal) value to its decimal representation (0-99).
uint8_t bcd_to_dec_bits(uint8_t val, uint8_t bits);

#endif // CONVERT_H