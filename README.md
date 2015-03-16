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
10. Simply ascii communication protocol for addressing up to 32 devices (max 1.2km apart) with checksums and some collision avoidance
11. EEPROM pcode for any input (digital, analog, temp etc) to control any output (digital, LCD, LED, buzzer)
12. Up to 16 timers or alarms with EEPROM pcode for controlling outputs
13. Two interrupt driven counters
14. HC05 or HC06 bluetooth module 
15. Can be constructed on breadboard with Arduino Pro Mini
16. TO DO: RaspberryPi web interface for control of multiple EEPs on one RS485 line
17. TO DO: Android application for bluetooth communication to standalone EEPs
 
## Examples of uses:
1. Soil moisture meter, soil temperature reading, controlling a mains unregulated propagator
2. Electricity meter readings via flashing LED
3. Room temperature/humidity sensing with PIR and LED lighting, MQ135 air quality sensing
4. Greenhouse or outdoor temperature/humidity/soil temperatures/soil moisture readings, controlling water relays and monitoring water flow sensors
5. External PIR connected to mains LED lighting, and sending message via RS485 to RaspberryPi controller to take photo/video of visitors, also sending beeps to other EEPs when visitors knock.
6. In kitchen, monitoring fridge/freezer temps, humidity near cooker to turn on extractor fan, PIRs to control LED lighting strips below cupboards, warning beeps if keys not 
7. Controlling LED lighting strips down stairs, or on garden paths 
8. Displaying useful information near external door (e.g. time of next bus gleaned from web via RaspberryPi)
9. Party lighting with "Puffometer" (count the ticks as you blow through a water flow sensor), "Drunkometer" (blow onto an MQ-3 alcohol sensor), lie detector etc. 

