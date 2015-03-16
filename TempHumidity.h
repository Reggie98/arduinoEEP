/* 
  TempHumidity.h    DS18B20 (onewire temperature sensors) and HTU21D (I2C temp and humidity)
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

#ifndef TEMPHUMIDITY_H
#define TEMPHUMIDITY_H


#include <Arduino.h>
#include <inttypes.h>
#include <Time.h>
#include <Wire.h>
#include <OneWire.h>

#include "Limits.h"
#include "EEpromMap.h"
#include "EEPROMAnything.h"

extern CEepromAnything EEPROM ;
extern OneWire oneWire ;
extern TwoWire Wire;

const uint8_t TEMP_HUMIDITY_SENSORS_MAX = TEMP_SENSORS_MAX + 2 ;    // one humidity and one temperature sensor on HTU21D
const uint8_t HTU21D_TEMP_SENSOR        = TEMP_SENSORS_MAX ;        // offset in array for HTU21D temperature sensor data
const uint8_t HTU21D_HUMIDITY_SENSOR    = TEMP_SENSORS_MAX + 1 ;     // offset in array for HTU21D humidity sensor data

#define TEMP_HUMIDITY_TO_FLOAT  64     // divide by this number to get decimal value


class tempHumidity 
{

public:
  tempHumidity () ;
  void begin () ;
  void clearMinMax () ;
  void checkTempHumidity ()  ;
  bool isTempHumidityReady () ;
  void setTempHumidityWait () ;
  void setTempHumidityStart () ;

  enum TempValMinMax
  {
    TEMP_VAL,
    TEMP_MIN,
    TEMP_MAX
  } ;

  void readTemp (uint8_t, TempValMinMax, int&, uint8_t&) ; // splits the temp into integer and fraction (0.00 to 0.99)
  
  int temp_values [TEMP_HUMIDITY_SENSORS_MAX] ;     // current temperature (in DS18B20 raw format) or temp/humidity from HTU21D
  int temp_values_min [TEMP_HUMIDITY_SENSORS_MAX] ; // minimum temperature (in DS18B20 raw format) or temp/humidity from HTU21D
  int temp_values_max [TEMP_HUMIDITY_SENSORS_MAX] ; // maximum temperature (in DS18B20 raw format) or temp/humidity from HTU21D

  uint8_t ds18b20_address [TEMP_SENSORS_MAX] [8] ;  // 8 bytes of sensor ID for each sensor
  uint8_t ds18b20_num_sensors ;                     // total number of temperature sensors on the system

  uint8_t temp_previous_checked ;                   // for up to 8 sensors (6 DS18B20, 2 for HTU21D)
                                                    // bits indicates whether a high condition (1) 
                                                    // or a low condition (0) was last seen / responded to
  
private:

  void ds18b20SelectReadTemp (uint8_t) ;
  void ds18b20ReadTemp (uint8_t) ;
  void ds18b20RequestAllTemps () ;
  
  void htu21dRequest (uint8_t) ;
  void htu21dReadBytes () ;             // read three bytes from htu21d into ds18b20_buffer []
  void htu21dReadHumidity () ;
  void htu21dReadTemp () ;
  

  uint8_t buffer [9] ;                  // buffer for reading from DS18B20 (9 bytes) or HTU21D (3 bytes)
  
  unsigned long temp_time_start ;       // next action time for the temperatures (requestTemperatures or read them into array)
  unsigned long temp_millis_start ;     // put this to millis () at the start of checkTempHumidity ()

  uint8_t ds18b20_check_sensor ;

  enum TempCheckState
  {
    TEMP_WAIT,
    TEMP_ALL_READY,
    TEMP_CHECK_HTU21D_HUMIDITY,
    TEMP_READ_DS18B20,
    TEMP_CHECK_DS18B20,
    TEMP_CHECK_HTU21D_TEMP,
    TEMP_NOT_READY
  } ;
  TempCheckState temp_check_state ;

  const uint8_t HTU21D_ADDRESS                  = 0x40 ; // Unshifted 7-bit I2C address for the sensor

  const uint8_t HTU21D_MEASURE_TEMP_HOLD        = 0xE3 ;  // not used - blocks the I2C line
  const uint8_t HTU21D_MEASURE_HUMDITY_HOLD     = 0xE5 ;  // not used - blocks the I2C line
  const uint8_t HTU21D_WRITE_USER_REG           = 0xE6 ;  // not used
  const uint8_t HTU21D_READ_USER_REG            = 0xE7 ;  // not used (assume default 12 bit humidity and 14 bit temperature)
  const uint8_t HTU21D_MEASURE_TEMP_NOHOLD      = 0xF3 ;
  const uint8_t HTU21D_MEASURE_HUMIDITY_NOHOLD  = 0xF5 ;
  const uint8_t HTU21D_SOFT_RESET               = 0xFE ;  

  const uint16_t DS18B20_DELAY_MS               = 750 ;    // don't read the DS18B20 temperatures until 750 mS after requestTemperatures ()
  const uint16_t HTU21D_DELAY_MS                = 60 ;

  const uint8_t DS18B20_START_CONVERSION        = 0x44 ;
  const uint8_t DS18B20_READ_SCRATCHPAD         = 0xBE ;
  const uint8_t DS18B20_POWER_ON                = 1 ;
  const uint8_t DS18B20_POWER_OFF               = 0 ;

    
};


extern tempHumidity TempHumidity ; 

#endif // TEMPHUMIDITY_H

