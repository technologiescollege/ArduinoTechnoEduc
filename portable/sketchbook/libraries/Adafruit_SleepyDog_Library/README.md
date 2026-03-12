# Adafruit SleepyDog Arduino Library [![Build Status](https://github.com/adafruit/Adafruit_SleepyDog/workflows/Arduino%20Library%20CI/badge.svg)](https://github.com/adafruit/Adafruit_SleepyDog/actions)[![Documentation](https://github.com/adafruit/ci-arduino/blob/master/assets/doxygen_badge.svg)](http://adafruit.github.io/Adafruit_SleepyDog/html/index.html)

Arduino library to use the watchdog timer for system reset. Also provides  simple API access for entering/exiting from low power sleep modes.

## Supported Platforms
| Support Level | Platform / Board Family | MCU(s) | Notes / Caveats |
|--------------|------------------------|--------|-----------------|
| **Full** | Raspberry Pi Pico | RP2040, RP2350 | — |
| **Full** | ESP32 Family | ESP32, ESP32-S2, ESP32-S3 | — |
| **Limited** | ESP8266 | ESP8266 | Software and hardware watchdog timers are fixed-interval and not programmable. See `utility/WatchdogESP8266.cpp`. |
| **Full** | Arduino Uno / ATmega328P Boards | ATmega328P | — |
| **Full** | Arduino Mega | ATmega2560, ATmega1280 | — |
| **Full** | Arduino Zero / Adafruit Feather M0 | ATSAMD21 | — |
| **Full** | Adafruit Feather M4 | ATSAMD51 | — |
| **Full** | Arduino Leonardo / 32u4 Boards | ATmega32u4 | USB Serial is disabled during sleep. Logic continues running but Serial appears frozen. Restore after wake with `USBDevice.attach();` and reconnect from host. |
| **Full** | ATtiny Series | ATtiny24/44/84, ATtiny25/45/85 | — |
| **Partial** | Teensy 3.x / LC | Teensy 3.x, LC | Watchdog supported; sleep not supported. |

