#ifndef DATETIME_H
#define DATETIME_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
  uint8_t sec; // [0-59]
  uint8_t min; // [0-59]
  uint8_t hour; // [0-23] 
  uint8_t dow; // day of week [1-7]
  uint8_t day; // day of month [0-31]
  uint8_t month; // [1-12]
  uint16_t year; // [1900-2099]
} datetime_data_s;

datetime_data_s decode_datetime_data(uint8_t ds3231_data[7]);

void encode_datetime_data(const datetime_data_s *datetime_data, uint8_t ds3231_data[7]);

void get_datetime_str(datetime_data_s *datetime_data, char buf[], size_t buf_size);

#endif // DATETIME_H