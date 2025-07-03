# RTC Support Project for nRF5340DK + DS3231

This project demonstrates how to interface the nRF5340 Development Kit (nRF5340DK) with a DS3231 Real-Time Clock (RTC) module using Zephyr RTOS. It provides code to set and read the date/time from the DS3231 over I2C, and prints the current time every second.

## Hardware Setup

- **MCU Board:** Nordic nRF5340DK
- **RTC Module:** DS3231 (I2C interface)
- **Connections:**
  - **SDA:** nRF5340DK pin P1.02  ⟶  DS3231 SDA
  - **SCL:** nRF5340DK pin P1.03  ⟶  DS3231 SCL
  - **VCC:** 2.9V from nRF5340DK  ⟶  DS3231 VCC
  - **GND:** GND from nRF5340DK   ⟶  DS3231 GND
- **Pull-up resistors:** Required (typically 4.7kΩ) on SDA and SCL lines if not present on the DS3231 module (but they were present on mine).

## Features
- Sets the DS3231 time at startup (configurable in code)
- Reads and prints the current date/time every second via UART/console
- Handles BCD/decimal conversion and DS3231 register encoding/decoding

## Software
- **Zephyr RTOS** (see `prj.conf` for configuration)
- I2C peripheral and pin configuration via device tree overlay (see `nrf5340dk_nrf5340_cpuapp_ns.overlay`)

## Usage
1. Connect the DS3231 module to the nRF5340DK as described above.
2. Build and flash the firmware.
3. Open a serial terminal to the nRF5340DK UART.
4. On reset, the board will set the RTC and print the current time every second.

## File Overview
- `src/main.c` – Main application logic
- `src/datetime.c`, `src/datetime.h` – Date/time conversion and formatting
- `src/convert.c`, `src/convert.h` – BCD/decimal conversion helpers
- `prj.conf` – Zephyr configuration
- `nrf5340dk_nrf5340_cpuapp_ns.overlay` – Device tree overlay for I2C pin assignment

## Notes
- Ensure your DS3231 module is powered at 2.9V to match the nRF5340DK logic levels.
- If you see I2C errors, check for correct wiring and pull-up resistors.
- The initial time set on the RTC is hardcoded in `main.c` and can be changed as needed.