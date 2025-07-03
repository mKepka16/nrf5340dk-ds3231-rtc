#include "convert.h"

uint8_t dec_to_bcd(uint8_t val)
{
  return ((val / 10) << 4) | (val % 10);
}

uint8_t bcd_to_dec_bits(uint8_t val, uint8_t bits)
{
  return (((val >> 4) & ((1 << (bits - 4)) - 1)) * 10) + (val & 0x0F);
}
