/* 
  Display.cpp     Display on Nokia 5110 (6 row, 14 columns) LCD
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


#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <inttypes.h>
#include <PCD8544_SPI.h>
#include <SPI.h>
#include <Time.h>

#include "Limits.h"
#include "EEpromMap.h"
#include "EEPROMAnything.h"

#include "AnalogInputs.h"
#include "TempHumidity.h"

class analogInputs ;

extern eeMap EeMap ;
extern mcp23017 Mcp23017 ;
extern CEepromAnything EEPROM ;
extern tempHumidity TempHumidity ;
extern analogInputs AnalogInputs ;
extern volatile unsigned long interrupt_count0 ; 
extern volatile unsigned long interrupt_count1 ;


class display : public PCD8544_SPI
{
public:
  display () ;
  
  typedef enum
  {
    DisplayNormal,
    DisplayInteractive
  } ;
  void gotoCharPosition (uint8_t) ;
  void checkUpdate () ;
  // void begin (bool invert, uint8_t vop, uint8_t tempCoef, uint8_t bias) ; // this is in the PCD8544_SPI_FAST routine
  bool isDisplayInteractive () ;
  void setDisplayMode (bool) ;  // false = normal mode (able to print on display)
                                // true = interactive mode - all key presses are all sent to the master, 
                                // and the display is controlled by master
  
private:
  unsigned long display_start_time ;
  void printDigits (int) ;
  void printTime () ;
  bool is_display_interactive ;
  uint8_t printValueFraction (int, uint8_t, bool) ;
  
  typedef enum 
  {
    DisplayDateTime =       1,            // dd/mm hh:mm:ss 
    DisplayPort0 =          0x20,         // =32    digital IO ports, where 2-13 => digital pins D2-D13, 
                                          //        14-15 => analog A0-A1 (as digital), 0-1 => analog A2-A3 (as digital)
    DisplayAnalog0 =        0x30,         // =48    analog ports A0-A3, A6-A7
    DisplayMCP0 =           0x40,         // =64    0-7 on GPIOA, 8-15 on GPIOB of MCP_I2C_ADDR 0x20
    DisplayMCP3 =           0x70,         // =112   0-7 on GPIOA, 8-15 on GPIOB of MCP_I2C_ADDR 0x23
    DisplayCounter2 =       0x90,         // =144   interrupt counter 2
    DisplayCounter3 =       0x91,         // =145   interrupt counter 3
    DisplayArduinoTemp =    0x98,         // =152   last sampled Arduino internal temperature
    DisplayArduinoVolts =   0x99,         // =153   last sampled Arduino Vcc voltage
    DisplayMessage =        0xA0,         // =160   up to 16 text messages for the screen
    DisplayTemp0 =          0xB0          // =176   Temps 0 to 7 (182 = I2C temp, 183 = I2C humidity)
  } ;
} ;

// e48:3:    0,1,14,0,42,54,49,55,56,144,63,153,70,176,77,177,84,0   // COM13
/*
0    Date Time
14  0i.... 1i....
28  6T.... 7H....
42  6A.... 7A.... 
56  AT.... AV....
70  0T.... 1T.... 
*/

// e48:3:    0,1,14,0,42,48,49,49,56,144,63,0,70,152,77,153,84,0   // COM5
/*
0    Date Time
14  (blank)
28  (blank)
42  0A.... 1A.... 
56  2i.... 
70  AT.... AV.... 
*/

extern display Display ; 

#endif // DISPLAY_H