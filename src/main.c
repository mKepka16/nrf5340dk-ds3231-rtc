#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include "convert.h"
#include "datetime.h"

#define RTC_NODE DT_NODELABEL(rtcsensor)

static const struct i2c_dt_spec sensor = I2C_DT_SPEC_GET(RTC_NODE);

int main(void)
{
  printk("Starting the device...\n");
  if (!device_is_ready(sensor.bus))
  {
    printk("RTC Sensor %s is not ready yet.\n", sensor.bus->name);
    return 1;
  }

  printk("Device is ready.\n");

  datetime_data_s datetime_data = {
      .day = 3,
      .month = 7,
      .year = 2025,
      .dow = 4,
      .hour = 22,
      .min = 12,
      .sec = 30,
  };

  uint8_t reg_addr = 0x00; // DS3231 seconds register
  uint8_t time_data[7];
  char buf[200];

  // Encode and upload initial datetime to DS3231
  encode_datetime_data(&datetime_data, time_data);
  uint8_t write_buf[8];
  write_buf[0] = reg_addr;
  for (int i = 0; i < 7; ++i) write_buf[i+1] = time_data[i];

  int ret = i2c_write_dt(&sensor, write_buf, 8);
  if (ret != 0) {
    printk("Failed to set time on DS3231 (err %d)\n", ret);
  } else {
    printk("Time set on DS3231\n");
  }

  while (1)
  {
    ret = i2c_write_read_dt(&sensor, &reg_addr, 1, &time_data, 7);
    if (ret != 0)
    {
      printk("Failed to read seconds from DS3231 at address 0x%x\n", sensor.addr);
    }
    else
    {
      datetime_data = decode_datetime_data(time_data);
      get_datetime_str(&datetime_data, buf, 200);
      printk("%s\n", buf);
    }
    k_msleep(1000);
  }

  return 0;
}