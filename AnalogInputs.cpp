/* 
  AnalogInputs.cpp    analog inputs library
  Copyright           (C) 2015 Reggie Thomson.  All rights reserved.

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

#include "AnalogInputs.h"

analogInputs::analogInputs ()
{
}

void analogInputs::begin ()
{
  previous_checked = B00000000 ; // assume they are all below the high value on startup
  analog_time_start = 0 ;
  analog_check_state = ANALOG_NOT_READY ;  

  while (analog_check_state != ANALOG_ALL_READY)
  {
    checkAnalogInputs () ;      // gets all the values
  }
  clearMinMax () ;
  analog_check_state = ANALOG_WAIT ;  // don't check for any pcode commands
}

void analogInputs::clearMinMax ()
{
  for (byte i = 0 ; i < ANALOG_INPUTS_MAX ; i++ )
  {
    analog_values_min [i] = analog_values [i] ;
    analog_values_max [i] = analog_values [i] ;
  }
}

bool analogInputs::isAnalogReady ()
{
  // called by the Pcode routine to see if new values are ready from all the analog ports (+Vcc + Arduino temp)
  return analog_check_state == ANALOG_ALL_READY ;
}

void analogInputs::setAnalogWait () 
{
  analog_check_state = ANALOG_WAIT ;
}

void analogInputs::setAnalogReadStart ()
{
  analog_check_state = ANALOG_NOT_READY ;
}

void analogInputs::checkAnalogInputs ()
{
  if (analog_check_state) // i.e. analog_check_state != ANALOG_WAIT
  {
    switch (analog_check_state)
    {
    case ANALOG_NOT_READY :  
    // case ANALOG_WAIT :
      analog_time_start = now () ;
      
      // start with read Vcc volts
      // Read 1.1V reference against AVcc
      ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);  
      analog_millis_start = millis () ;
      analog_check_state = ANALOG_READ_VOLTS_SETTLE ;
      break ;
      
    case ANALOG_READ_VOLTS_SETTLE :
    case ANALOG_READ_TEMP_SETTLE :
      if (millis () - analog_millis_start > ANALOG_ARDUINO_READ_DELAY_MS)
      {
        ADCSRA |= _BV(ADSC); // start the convertion A to D only after the READ_DELAY_MS period
        if (analog_check_state == ANALOG_READ_VOLTS_SETTLE)  
        {
          analog_check_state = ANALOG_READ_VOLTS_CONVERT ;
        } else
        {
          analog_check_state = ANALOG_READ_TEMP_CONVERT ;
        }
      }
      break ;
    case ANALOG_READ_VOLTS_CONVERT :
      if (!bit_is_set(ADCSRA,ADSC))  // A to D conversion is complete
      {
        long result ; 
        result = ADCL ;
        result |= ADCH << 8 ;
        analog_values [ARDUINO_VCC] = 112640L / result; // Back-calculate AVcc in 10mV steps i.e. 500 = 5.00 volts  
        
        // Read temperature sensor against 1.1V reference
        ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX3);
        analog_millis_start = millis () ;
        analog_check_state = ANALOG_READ_TEMP_SETTLE ;
      }
      break ;        
    case ANALOG_READ_TEMP_CONVERT :
      if (!bit_is_set(ADCSRA,ADSC))  // A to D conversion is complete
      {
        long result ; 
        result = ADCL ;
        result |= ADCH << 8 ;
        result = (result - 125) * 1075 ;
        result /= (10000 / TEMP_HUMIDITY_TO_FLOAT) ;
        analog_values [ARDUINO_TEMP] = result ; 
        // currently stored here - but really should send it to Temp routine to store
        // check if there are any pcode commands to be done before starting analog reads
        if (EEPROM.read (eeMap::AnalogPrePcodeIndex))
        {
          analog_millis_start = millis () ;
          Pcode.change_ports ((eeMap::AnalogPrePcodeIndex & 0xFF00) + EEPROM.read (eeMap::AnalogPrePcodeIndex)) ;
          analog_check_state = ANALOG_PCODE_START ;
        } else
        {
          analog_check_state = ANALOG_READ_ALL ;
        }
      }
      break ;
    case ANALOG_PCODE_START :
      if (millis () - analog_millis_start > (EEPROM.read (eeMap::AnalogPreDelay) + EEPROM.read (eeMap::AnalogPreDelay + 1) << 8))
      {
        analog_check_state = ANALOG_READ_ALL ;
      } else
      {
        break ;
      }
      // NOTE: no break
    case ANALOG_READ_ALL :
      for (byte i = 0 ; i < 8 ; i++)
      {
        if (i < 4 || i > 5)    // skip the I2C pins
        {
          analog_values [i]  = analogRead (i) ; // 100uS per read
        }
      }
      analog_check_state = ANALOG_POST_PCODE ;
      break ;
    case ANALOG_POST_PCODE :
      for (byte i = 0 ; i < ANALOG_INPUTS_MAX ; i++)
      {
        analog_values_min [i] = min (analog_values [i], analog_values_min [i]) ;
        analog_values_max [i] = max (analog_values [i], analog_values_max [i]) ;  
      }
      if (EEPROM.read (eeMap::AnalogPostPcodeIndex))
      {
        Pcode.change_ports ((eeMap::AnalogPostPcodeIndex & 0xFF00) + EEPROM.read (eeMap::AnalogPostPcodeIndex)) ;
      } 
      analog_check_state = ANALOG_ALL_READY ; // all analog values have been read and are ready to be checked against Pcode
      break ;
    }
  }
}

int analogInputs::readAnalog (uint8_t analog_port, ArduinoValMinMax val_min_max)
{
  switch (val_min_max)
  {
  case ARDUINO_VAL :
    return analog_values [analog_port] ;
    break ;
  case ARDUINO_MIN :
    return analog_values_min [analog_port] ;
    break ;
  case ARDUINO_MAX :
    return analog_values_max [analog_port] ;
  }
}

void analogInputs::readArduinoTempC (ArduinoValMinMax val_min_max, int& value, uint8_t& fraction) 
{
  unsigned int fract ;
  value = readAnalog (ARDUINO_TEMP, val_min_max) ;
  fract = value ;
  value /= TEMP_HUMIDITY_TO_FLOAT ;

  fract &= TEMP_HUMIDITY_TO_FLOAT - 1 ;  
  fract *= 100 ;
  fract /= TEMP_HUMIDITY_TO_FLOAT ;
  fraction = fract ;
}

void analogInputs::readArduinoVcc (ArduinoValMinMax val_min_max, int& value, uint8_t& fraction)
{
  value = readAnalog (ARDUINO_VCC, val_min_max) ;
  fraction = value - (value / 100) * 100 ;
  value /= 100 ;
}

analogInputs AnalogInputs ; 
