/* 
  EEpromMap.h      Definition of the use of the EEPROM
  Copyright       (C) 2015 Reggie Thomson.  All rights reserved.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef EEPROMMAP_H
#define EEPROMMAP_H
/*
#include "EEPROMAnything.h"

extern CEepromAnything EEPROM ;
*/

#include <Arduino.h>
#include <inttypes.h>

class eeMap
{
public:
  // eeMap () ;    // constructor - empty

  typedef enum 
  {
    // NameInEEProm         #     // offset:bank
    DeviceId =              0,    // 0:0     uint8_t - device #1 to #32
      
    BeeperTone,                   // 1:0     uint8_t
      
    // TempSampleRate,            // 2:0     uint8_t; sample temps (in seconds); if =0, don't sample temps (do with the T0:1 commands (send twice)
      
    // AnalogSampleRate,          // 3:0    uint8_t; sample analog inputs (in seconds)
      
    Time =                  4,    // 4:0    long ; stored every hour 
      
    DisplayVop =            8,    // 8:0    uint8_t contrast (188)
    DisplayTempcoef,                  // 9:0    uint8_t (5)
    DisplayBias,                      // 10:0    uint8_t (20)
      
    RxMillisDelay,                    // 11:0    uint8_t
                                      // after receiving an invalid char or a packet for another slave, wait for at least rx_millis_delay 
                                      // before sending any notify messages to the master
                                      // high priority devices can keep this value low (above 7ms), low priority devices should have higher delays
                                          
    PortDIO,                12,   // 12:0    digital pins 0 to 7 set as INPUT (0) or OUTPUT (1)
                                  /* NOTES:
                                   *    0 is RX (must be input)
                                   *    1 is TX (must be output)
                                   *    2, 3 can be outputs (e.g. relays) or inputs (e.g. meter flash counters, flow meter counters) 
                                   *    4 is TXE_PIN (must be output)
                                   *    5 is BUZZER_PIN (if there is a buzzer, therefore output) or input
                                   *    6 is the LED pin (output)
                                   *    7 is DS18B20 pin (output)
                                   */
    PortDPullup,                  // 13:0   pullups enabled on digital pins 0 to 7 (probably all set as no pullups (0))

    PortBIO,                      // 14:0   D8 to D13 set as INPUT (0) or OUTPUT (1) - NB: 14,15 are the crystal; 
                                  //      Can't use any if display enabled
    PortBPullup,                  // 15:0   pullups enabled on digital pins 8 to 13 (probably all set as no pullups (0))

    PortCIO,                      // 16:0   analogue pins 0 to 7 set as INPUT (0) or OUTPUT (1)
                                  /* NOTES:
                                   *    4 is I2C SDA (set as output)
                                   *    5 is I2C SCL (must be output)
                                   *    6, 7 cannot be used as digital output
                                  */ 
    PortCPullup,                  // 17:0   pullups enabled on digital pins 0 to 7 (probably all set as no pullups (0))

    TempSensorResolution,         // 18:0   DS18B20 temperature sensor resolution (10 to 12 bits)
    // SPARE                      // 19:0 to 22:0
    
    McpNumDevices =         23,   // 23:0   actual number of MCP devices (0 to 4 max)

    McpIodir =              24,   // 24:0   data direction for up to 4 MCP devices #20 GPA = 24, #20 GPB = 25; #21 GPA = 26 etc.
    McpGppu =               32,   // 32:0   pullups for up to 4 MCP devices #20 GPA = 24, #20 GPB = 25; #21 GPA = 26 etc.

    InterruptCount0 =       40,   // 40:0   uint32_t counter on interrupt 0 (pin 2), updated hourly
    InterruptCount1 =       44,   // 44:0   uint32_t counter on interrupt 1 (pin 3), updated hourly  

    OptionBeep =            48,   // 48:0  
    OptionNotify,                 // 49:0  
    OptionGpio,                   // 50:0  

    LedGroup =              51,   // 51:0   number of LEDs in a group - read directly from eeprom, so can be changed with e48:0:grp command
    LedOnRgb,                     // 52:0   rrrggbbb of LED on colour - read directly from eeprom, so can be changed with e49:0:rrrggbbb command
    LedOffRgb,                    // 53:0   rrrggbbb of LED off colour - read directly from eeprom; 3=blue; 24=green; 224=red
    LedNumLeds,                   // 54:0   total number of LEDs on this arduino
    LedModeMaxTails,              // 55:0
    LedModeTailLen,               // 56:0
    LedModeFlashRate,             // 57:0

    // SPARE                      // 58:0

    // These EEPROM values are set during startup to show the compiled state of the code (but it would be better to put these into PROGMEM)
    OptionCompile =         59,   // 59:0     

    LedsMax,                      // 60:0    shows the max number of leds allowed in this compiled version of the software
    TempSensorsMax,               // 61:0     shows the max number of temperature sensors allowed in this compiled version of the software
    AnalogInputsMax,              // 62:0    shows the max number of analog inputs allowed in this compiled version of the software
    Mcp23017Max,                  // 63:0    shows the max number of MCP23017 devices allowed in this compiled version of the software
    
    // SPARE                      // 64:0 to 95:0
    
    AnalogTypes  =          96,   // 96:0    8 bytes defining the reading, timing and interpretation of each analog input
    AnalogPrePcodeIndex =   104,  // 104:0  index for Pcode performed before the Analog values are read
    AnalogPostPcodeIndex =  105,  // 105:0  index for Pcode performed after the Analog values are read
    AnalogPreDelay =        106,    // 106:0  uint16_t after AnalogPrePcode, delay at least this value in ms before reading analog values

    // SPARE                      // 108:0 to 127:0
  
    KeypadMap  =            128,    // 128:0   16 bytes for the interpretation of keys on keypad (mapping from row:col to 0-15 values)
    PcodeKeysIndex =        144,    // 144:0  p-code for eeprom programmable code for the keypad only (112 bytes) 
                                            // 144-159 = index for keys 0-15; 
                                            // 160-255 space for output responses

    PcodeTempsIndex =       256,    // 0:1    p-code for temperature responses (128 bytes max, 8 temp sensors)
                                            // 256-271 indexes for low and high temperature comparison values
                                            // 272-383 space for temperature comparison value (2 bytes, int),
                                            //   followed by the output response list for each temp sensor

    PcodeAnalogIndex =      384,     // 128:1  p-code for 8 analog inputs (16 bytes, low, high for each analog port)
    PcodeInputsIndex =      512,    // 0:2     p-code for up to 16 i/o conditions and responses
    PcodeAlarmsIndex =      640,    // 128:2  p-code for up to 16 timer alarms
    // RelayMap =           800,     // 32:3    16 bytes for relays R0-R15 to actual i/o pins (use PCODE_CMD_MCP0 or PCODE_CMD_PORT)
                                  // NOT IN USE
    DisplayMap =            768,    // 0:3    16 * 2 bytes for the display on the display (char position, display item)
                                    // e.g. 0, date/time, 14, analog 0, 21, analog 1, 28, temp 0, 35, temp 1, 42, counter 2,
                                    //      56, relay 0-7 binary, 66, MCP GPIOB #4, 70, menu string: "Menu #"
    MessageIndex =          800      // 32:3  16 bytes for index of up to 16 messages
  } ;

//  bool isKeypad16keysSet () ;      
  
  // interpretation of bits of OptionGpio:

  typedef enum
  {
    GpioKeypad16Keys =      B00000001   // set bit 1 of OptionGpio for 16 keys, or 0 for 4 keys only
  } ;

  
  // set the fixed bits of the Arduino ports D, B and C
  typedef enum
  {
    PortDIOFixed =          B00110011,        // don't change pins 5, 4, 1, 0
    #if COMPILE_NOKIA_5110
      PortBIOFixed =        B11111111,      // don't change pins 15, 14, 13, 12, 11, 10, 9, 8
    #else
      PortBIOFixed =        B11000000,      // don't change pins 15, 14
    #endif
    PortCIOFixed =          B11110000        // don't change pins A4-A7   
  } ;
  
  // interpretation of bits of OptionBeep:
  typedef enum
  {
    BeepOnKeyPressed =      B00000001    // set bit 1 of OptionBeep to beep when any key is pressed
  } ;
  
  // interpretation of bits of OptionNotify:
  typedef enum
  {
    NotifyOnKeyPressed =    1 << 0,  // send an notify message when a key is pressed
    NotifyOnTemp =          1 << 1,  // send an notify message when a temp condition is met
    NotifyOnGpio =          1 << 2,  // send an notify message when a gpio condition is met
    NotifyOnAnalog =        1 << 3,  // send an notify message when an analog condition is met
    NotifyOnAlarm =         1 << 4  // send an notify message when an alarm has gone off
  } ;
  
  // interpretation of the Analog types (Not yet coded!)
  typedef enum
  {
    AnalogNotUsed =         0,
    AnalogAsIs =            1,
    AnalogArduinoVolts =    2,
    AnalogArduinoTemp =     3,
    AnalogPcode0 =          0x10,
    AnalogPcode1 =          0x11,
    AnalogPcode2 =          0x12,
    AnalogPcode3 =          0x13
  } ;
  
private:
  
} ;


#define  EE_PCODE_INPUTS_MAX  16      // maximum of 16 i/o conditions and responses to check for


// NOTE WS2812 leds (indoor strip) are in order gggrrbbb, outdoor strip is rrrggbbb

#define IS_BEEP_KEY_SET       (EEPROM.read (eeMap::OptionBeep) & eeMap::BeepOnKeyPressed)

#define IS_NOTIFY_KEY_SET     (EEPROM.read (eeMap::OptionNotify) & eeMap::NotifyOnKeyPressed)
#define IS_NOTIFY_TEMP_SET    (EEPROM.read (eeMap::OptionNotify) & eeMap::NotifyOnTemp)
#define IS_NOTIFY_COND_SET    (EEPROM.read (eeMap::OptionNotify) & eeMap::NotifyOnGpio)
#define IS_NOTIFY_ANALOG_SET  (EEPROM.read (eeMap::OptionNotify) & eeMap::NotifyOnAnalog)
#define IS_NOTIFY_ALARM_SET   (EEPROM.read (eeMap::OptionNotify) & eeMap::NotifyOnAlarm)


// CHANGE this should not be put into EEPROM - it should be stored in PROGMEM
#if COMPILE_NOKIA_5110
  #define EE_COMPILE_NOKIA_5110_BIT   B00000001  // set if the Nokia 5110 code has been compiled in this software
#else
  #define EE_COMPILE_NOKIA_5110_BIT   B00000000  // 
#endif

#if COMPILE_LEDSTRIP
  #define EE_COMPILE_LEDSTRIP_BIT     B00000010  // include the single wire WS8211 chip RGB LED driver code
#else
  #define EE_COMPILE_LEDSTRIP_BIT     B00000000  // 
#endif

 
#endif

