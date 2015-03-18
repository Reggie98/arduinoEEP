/* 
  Pcode.h          Pcode library
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


#ifndef PCODE_H
#define PCODE_H

#include <Arduino.h>
#include <inttypes.h>
#include <Wire.h>

#include "Limits.h"
#include "EEpromMap.h"            // this defines the use of the EEPROM
#include "EEPROMAnything.h"

#if COMPILE_LEDSTRIP
  #include "LedStrip.h"
#endif

#include "MCP23017.h"              // code for dealing with the MCP23017 16-bit GPIO extenders
#include "Beeper.h"    
#include "Keypad.h"
#include "AnalogInputs.h"
#include "Alarms.h"
#include "SerialComms.h"

#if COMPILE_NOKIA_5110
  #include <SPI.h>
  #include <PCD8544_SPI.h>
  #include "Display.h"
#endif


#define PCODE_MCP_PORT_B_BIT       B00001000  // 0-7 for GPIOA, 8-15 for GPIOB
#define PCODE_MCP_BIT_MASK         B00000111  // for GPIO 0 to 7

#define PCODE_LOW_LT               0x01       // input goes low or is lower than the value (analog, temp, counter?); or set output low
#define PCODE_HIGH_GT              0x02       // input goes high or is greater than the value (analog, temp, counter?); or set output high
#define PCODE_CHANGE               0x03       // input changes state; or change the state of the output
#define PCODE_AND_BIT              B10000000  // 128+      this condition is followed by another condition, 
                                              //             or this output is followed by another output
                                              // 129        eg PCODE_HIGH_GT and another condition or output follows


extern uint8_t arduino_gpio [] ;          // 0 is D0-D7, 1 is D8-D13, 2 is A0-A7
extern uint8_t arduino_gpio_last [] ;     // 0 is D0-D7, 1 is D8-D13, 2 is A0-A7; 
                                          // these are the previous values of the arduino digital inputs 
extern mcp23017 Mcp23017 ;
extern void set_notify_message (uint8_t, uint8_t) ;

#if COMPILE_LEDSTRIP
  extern ledStrip LedStrip ;
#endif

extern volatile unsigned long interrupt_count0 ;
extern volatile unsigned long interrupt_count1 ;
extern CEepromAnything EEPROM ;


class pcode
{
public:
  pcode () ;
  void begin () ;

  typedef enum PcodeCmd
  {
    PcodeCmdMask =          0xF0,
    PcodePortMask =         0x0F,

    PcodeCmdNull =          0x00,     // no further commands
    PcodeCmdPowerUp =       0x01,     // command sent on initial power up
    // 0x02 to 0x0F not used
    PcodeCmdKey  =          0x10,     // 16-31  (message) keypad 0-15 
    PcodeCmdPort =          0x20,     // 32-47  (input/output) digital IO ports, where 2-13 (34-45)=> digital pins D2-D13, 
                                      //   14-15 (46,47)=> analog A0-A1 (as digital), 0-1 (32,33) => analog A2-A3 (as digital)
    PcodeCmdAnalog =        0x30,     // 48-55  (input + 2 bytes) analog ports A0-A3, A6-A7 
                                      //           (8-15 spare, could be multiplexed analog on A7)
    // 0x34, 0x35 not used; 0x38-0x3F not used
    PcodeCmdMcp0 =          0x40,     // 64-71  (input/output) 0-7 on GPIOA, 
                                      // 72-79  8-15 on GPIOB of MCP_I2C_ADDR 0x20
    PcodeCmdMcp1 =          0x50,     //         (input/output) 0-7 on GPIOA, 8-15 on GPIOB of MCPI2CADDR 0x21
    PcodeCmdMcp2 =          0x60,     //         (input/output) 0-7 on GPIOA, 8-15 on GPIOB of MCP_I2C_ADDR 0x22
    PcodeCmdMcp3 =          0x70,     //         (input/output) 0-7 on GPIOA, 8-15 on GPIOB of MCP_I2C_ADDR 0x23
    PcodeCmdLed =           0x80,     // 128-134  (input/output) 0-6 leds 0 to 6 or groups of leds; 
                                      // 135      (output) increment the led mode 
                                      // 136-142  8-14 => LED modes 1 to 7
                                      // 143      set all LEDs off (black)
    PcodeCmdMisc =          0x90,     // 144-145   (input + 2 bytes) 0-1 interrupt counters 2-3; 
                                      // 144-145   (output) reset interrupt counters D2 (INT0) or D3 (INT1)to 0
    CmdMiscResetInt0 =      0x00,     // 144      (output) reset interrupt counters on D2 (INT0) to 0
    CmdMiscResetInt1 =      0x01,     // 145      (output) reset interrupt counters on D3 (INT1) to 0
    CmdMiscResetTemps =     0x02,     // 146      reset all min/max temps
    CmdMiscResetAnalog =    0x03,     // 147      reset all min/max analog
    CmdMiscReadTemps =      0x04,     // 148      read all temps (allows an alarm to be used to read temps)
    CmdMiscReadAnalog =     0x05,     // 149      read all analog inputs (allows an alarm to be used to read analog inputs)
    
    CmdMiscInteractive =    0x07,     // 151       set the keypad/display into interative mode 
                                      //           (send all key presses to master, update display from master)
    // CmdMiscDisplay =        0x08,     // 152      (output + n bytes) display a string (ends with 0) at the position on the display
    CmdMiscDisplayNormal =  0x08,     // 152      (output) set the display to show all the normal (current) values for analog/temp/humitidy
    CmdMiscDisplayMin =     0x09,     // 153      (output) set the display to show all the minimum values for analog/temp/humitidy
    CmdMiscDisplayMax =     0x0A,     // 154      (output) set the display to show all the minimum values for analog/temp/humitidy
                                      // 155-157  (output) 3-6 => PWM on pins D3, D5, D6; (NOT YET CODED)
    // 0xA0-0xAF not used
    PcodeCmdTemp =          0xB0,     // 176-183  (input + 2 bytes) Temps 0 to 7 (message)
    // 0xB8-0xBF not used
    PcodeCmdBeep =          0xC0,     // 192-199  (output) 0-7 => 1-8 short beeps, 
                                      //  200-207  (output) 8-15 => 1-8 long beeps
    PcodeCmdAlarm =         0xD0,     // 208-223  (output) switch on or off alarms 0-15
    PcodeCmdMessage =       0xE0,     // 224-238  (output + 1 byte) write message 0-14 to display at given position
    CmdMessageInline =      0x0F,     // 239      (output + n bytes) display a string (ends with 0) at the position on the display
    PcodeCmdCond =          0xF0      // 240-255  (message) condition met
  } ;
  
  
  uint8_t check_int_high_low (int pcode_index0, uint8_t sensor_number, int sensor_current_value, uint8_t & all_sensors_previous_checked ) ;
  void check_all_conditions (unsigned int pcode_ptr, uint8_t condition_num) ;
  void check_all_gpio_conditions () ;
  uint8_t change_ports (unsigned int pcode_ptr) ;

private:
  void check_condition_bit (uint8_t pcode_condition, uint8_t pcode_input, uint8_t pin_current, uint8_t pin_last, uint8_t &new_pin_state) ;
  void check_condition ( unsigned int &pcode_ptr, uint8_t &new_pin_state) ;
  uint8_t change_port (uint8_t new_val, unsigned int & pcode_ptr) ;

} ;

extern pcode Pcode ; 


#endif    // PCODE_H
