# Arduino EEP

The Arduino EEP is an Arduino standalone or RS485 networked system, with LCD display, keypad, RTC, bluetooth, multiple digital & analog I/Os, RGB LEDs, buzzer, temperature and humidity sensors, and full remote EEPROM reconfigurability.

## Key Features:
1. Uses any Arduino ATmega328 16MHz (Pro Mini, Uno, Deicimillenove...)
2. Nokia 5110 84 character display, able to show time, analog or digital I/O states, temperatures, or text
3. 16-key keypad can control any digital output, LED state, buzzer
4. Up to 6 DS18B20 temperature sensors with min/max values
5. SHT21 (with built-in 5V to 3V3 conversion) for temperature & humidity
6. DS1307 RTC for time
7. Up to 4 MCP23017 digital 16 I/O expansion (64 I/O ports) all independently set as inputs or outputs with optional pull-ups
8. WS2812 LED controller (FastLED) with up to 60 LEDs and some fancy sequences (rainbows, breathe, worms, random, flashing)
9. Serial communications via Bluetooth or RS485, reading I/O states, switching relays etc., printing to LCD, controlling LEDs, updating EEPROM
10. Simply ascii communication protocol for addressing up to 99 devices with checksum, some collision avoidance
11. EEPROM pcode for any input (digital, analog, temp etc) to control any output (digital, LCD, LED, buzzer)
12. Up to 16 timers or alarms with EEPROM pcode for controlling outputs
13. Two interrupt driven counters

 


