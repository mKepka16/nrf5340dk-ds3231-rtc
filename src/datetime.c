#include "datetime.h"
#include "convert.h"
#include <stdbool.h>
#include <zephyr/sys/printk.h>

// Parses 7 bytes of DS3231 register data into a datetime_data_s struct.
// The input array ds3231_data[] must contain exactly 7 elements
datetime_data_s decode_datetime_data(uint8_t ds3231_data[7])
{
  uint8_t sec = bcd_to_dec(ds3231_data[0]);
  uint8_t min = bcd_to_dec(ds3231_data[1]);
  bool is_12h_mode = ds3231_data[2] >> 6;
  bool is_pm = (ds3231_data[2] >> 5) & 0x01;

  uint8_t hour = bcd_to_dec_bits(ds3231_data[2], is_12h_mode ? 5 : 6);
  if (is_12h_mode)
  {
    if (hour == 12)
    {
      hour = is_pm ? 12 : 0;
    }
    else
    {
      hour = is_pm ? (hour + 12) : hour;
    }
  }

  uint8_t dow = ds3231_data[3];
  uint8_t day = bcd_to_dec(ds3231_data[4]);
  uint8_t month = bcd_to_dec_bits(ds3231_data[5], 5);
  uint8_t century = ds3231_data[5] >> 7;
  uint16_t year = (century ? 2000 : 1900) + bcd_to_dec(ds3231_data[6]);

  datetime_data_s datetime_data = {
      .sec = sec,
      .min = min,
      .hour = hour,
      .dow = dow,
      .day = day,
      .month = month,
      .year = year};

  return datetime_data;
}

void get_datetime_str(datetime_data_s *datetime_data, char buf[], size_t buf_size)
{
  snprintk(buf, buf_size, "%04d-%02d-%02d %02d:%02d:%02d",
           datetime_data->year, datetime_data->month, datetime_data->day,
           datetime_data->hour, datetime_data->min, datetime_data->sec);
}

// Converts a datetime_data_s struct to 7 bytes suitable for DS3231 registers (BCD encoding, handles century bit)
void encode_datetime_data(const datetime_data_s *datetime_data, uint8_t ds3231_data[7])
{
  ds3231_data[0] = dec_to_bcd(datetime_data->sec);
  ds3231_data[1] = dec_to_bcd(datetime_data->min);

  ds3231_data[2] = dec_to_bcd(datetime_data->hour) & 0x3F; // bits 5:0 = hour, bit 6 = 0 (24h), bit 7 = 0

  ds3231_data[3] = datetime_data->dow;

  ds3231_data[4] = dec_to_bcd(datetime_data->day);

  uint8_t month_bcd = dec_to_bcd(datetime_data->month) & 0x1F; // bits 4:0 = month
  uint8_t century = (datetime_data->year >= 2000) ? 1 : 0;
  ds3231_data[5] = (century << 7) | month_bcd;
  uint8_t year_bcd = dec_to_bcd((uint8_t)(datetime_data->year % 100));
  ds3231_data[6] = year_bcd;
}