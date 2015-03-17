# Arduino EEP

The Arduino EEP is an Arduino standalone or RS485 networked system, with LCD display, keypad, RTC, bluetooth, multiple digital & analog I/Os, RGB LEDs, buzzer, temperature and humidity sensors, and full remote EEPROM reconfigurability.

## Key Features:
1. Uses any Arduino ATmega328 16MHz (Pro Mini, Uno, Deicimillenove...)
2. Nokia 5110 84 character display, able to show time, analog or digital I/O states, temperatures, or text
3. 16-key (or 4-key) keypad can control any digital output, LED state, buzzer
4. Up to 6 DS18B20 temperature sensors with min/max values
5. HTU21D module (with built-in 5V to 3V3 conversion) for temperature & humidity
6. DS1307 RTC for time
7. Up to 4 MCP23017 digital 16 I/O expansion (64 I/O ports) all independently set as inputs or outputs with optional pull-ups
8. WS2812 LED controller (using FastLED) with up to 60 LEDs and some fancy sequences (rainbows, breathe, worms, random, flashing)
9. Serial communications via Bluetooth or RS485, reading I/O states, switching relays etc., printing to LCD, controlling LEDs, updating EEPROM
10. Simply ascii communication protocol @115k2 for addressing up to 32 devices (max 1.2km apart) with checksums and some collision avoidance
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
9. Party LED lighting with "Puffometer" (count the ticks as you blow through a water flow sensor), "Drunkometer" (blow onto an MQ-3 alcohol sensor), lie detector etc. 

## Hardware Requirements:
Nokia 5110 uses 3v3 and needs a conversion such as in the diagram on http://forum.arduino.cc/index.php?topic=176794.0 (http://i.imgur.com/RwCVqnF.png). It is also recommended to use a 74HC4050 
Keypad connected as 4x4 (16-key) on MCP23017 GPIOA (see https://github.com/joeyoung/arduino_keypads/blob/master/Keypad_MC17/docs/usingMCP23017.jpg)

## Software Requirements
Time from http://www.pjrc.com/teensy/td_libs_Time.html
FastLED from https://github.com/FastLED/FastLED
DS1307RTC from http://www.pjrc.com/teensy/td_libs_DS1307RTC.html
OneWire from http://www.pjrc.com/teensy/td_libs_OneWire.html
PCD8544_SPI (for Nokia 5110) from http://forum.arduino.cc/index.php?topic=176794.0 (download: http://forum.arduino.cc/index.php?action=dlattach;topic=176794.0;attach=54196)

Change the following in the Arduino program folders: 
Comment out the line "extern EEPROMClass EEPROM;" in the file:
    ...\Arduino\hardware\arduino\avr\libraries\EEPROM\EEPROM.h
    
Comment out the line "EEPROMClass EEPROM;" in the file:
    ...\Arduino\hardware\arduino\avr\libraries\EEPROM\EEPROM.cpp

Change the SERIAL_RX_BUFFER_SIZE and SERIAL_TX_BUFFER_SIZE in the file:
    ...\Arduino\hardware\arduino\avr\cores\arduino\HardwareSerial.h

#if (RAMEND < 1000)
#define SERIAL_TX_BUFFER_SIZE 64
#define SERIAL_RX_BUFFER_SIZE 128
#else
#define SERIAL_TX_BUFFER_SIZE 64
#define SERIAL_RX_BUFFER_SIZE 128
#endif
    
Use Arduino version 1.5.7 or lower with FastLED 3.0 (see FastLED from https://github.com/FastLED/FastLED)


