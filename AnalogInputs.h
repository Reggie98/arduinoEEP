/* 
  AnalogInputs.h    analog ports library
  Copyright         (C) 2015 Reggie Thomson.  All rights reserved.

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


#ifndef ANALOGINPUTS_H
#define ANALOGINPUTS_H

#include <Arduino.h>
#include <inttypes.h>
#include <Time.h>

#include "Limits.h"
#include "EEpromMap.h"
#include "EEPROMAnything.h"

#include "MCP23017.h"
#include "TempHumidity.h"
#include "Pcode.h"

extern CEepromAnything EEPROM ;

extern mcp23017 Mcp23017 ; 

extern eeMap EeMap ;

#define ANALOG_ARDUINO_READ_DELAY_MS  8   // minimum of 8ms delay after setting up to read Arduino temperature or supply voltage

#define ARDUINO_VCC  4    // analog_values[4] = I2C pin 
#define ARDUINO_TEMP 5    // analog_values[5] = I2C pin 


class analogInputs
{
public:
  analogInputs () ;
  void begin () ;
  void clearMinMax () ;
  void checkAnalogInputs () ;
  int analog_values [ANALOG_INPUTS_MAX] ;
  int analog_values_min [ANALOG_INPUTS_MAX] ;
  int analog_values_max [ANALOG_INPUTS_MAX] ;
  uint8_t previous_checked ; // 8 bits represent the 8 ports (max) A0 to A7

  enum ArduinoValMinMax
  {
    ARDUINO_VAL,
    ARDUINO_MIN,
    ARDUINO_MAX
  } ;
  
  int readAnalog (uint8_t, ArduinoValMinMax) ;
  void readArduinoTempC (ArduinoValMinMax, int&, uint8_t&) ;
  void readArduinoVcc (ArduinoValMinMax, int&, uint8_t&) ;  
  
  bool isAnalogReady () ;
  void setAnalogWait () ;  
  void setAnalogReadStart () ;  


private:
  unsigned long analog_time_start ;
  unsigned long analog_millis_start ;

  enum AnalogCheckState
  {
    ANALOG_WAIT,
    ANALOG_ALL_READY,
    ANALOG_READ_ALL,
    ANALOG_POST_PCODE,
    ANALOG_PCODE_END,
    ANALOG_READ_VALUE,
    ANALOG_START_CONVERT,
    ANALOG_DELAY,
    ANALOG_PCODE_START,
    ANALOG_READ_TEMP_CONVERT,
    ANALOG_READ_TEMP_SETTLE,
    ANALOG_READ_VOLTS_CONVERT,
    ANALOG_READ_VOLTS_SETTLE,
    ANALOG_NOT_READY
  } ;
  AnalogCheckState analog_check_state ;
  
  
} ;

extern analogInputs AnalogInputs ; 

#endif // ANALOGINPUTS_H
