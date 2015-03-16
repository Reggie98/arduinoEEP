/* 
  TempHumidity.cpp  DS18B20 (onewire temperature sensors) and HTU21D (I2C temp and humidity)
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


#include "TempHumidity.h"

tempHumidity::tempHumidity ()
{
}


void tempHumidity::begin ()
{
  /*
  // doesn't make any noticable difference!
  Wire.beginTransmission (HTU21D_ADDRESS) ;
  Wire.write (HTU21D_SOFT_RESET) ;
  Wire.endTransmission () ;  
  // delay (15) ;  // the oneWire search will take more than this time, probably
  */
  ds18b20_num_sensors = 0 ;
  
  // Start up the DS18B20 library
  oneWire.reset_search () ;
  
  // ds18b20_address [ds18b20_num_sensors][ds18b20_num_sensors] = 0 ;
  while (oneWire.search (ds18b20_address [ds18b20_num_sensors]))
  {
    if (oneWire.crc8(ds18b20_address [ds18b20_num_sensors], 7) == ds18b20_address [ds18b20_num_sensors][7])
    {
      ds18b20_num_sensors ++ ;
      // ds18b20_address [ds18b20_num_sensors][ds18b20_num_sensors] = 0 ;
    }
  }
  
  temp_previous_checked  = 0 ;    // set all bits to zero - NB any temp above the high threshold on startup will send a notify message
  temp_time_start = 0 ;           // next action time for the temperatures (requestTemperatures or read them into array)
  temp_check_state = TEMP_NOT_READY ;
  while (temp_check_state != TEMP_ALL_READY)
  {
    checkTempHumidity () ;        // gets all the values
  }
  clearMinMax () ;
  temp_check_state = TEMP_WAIT ;  // don't check for any pcode commands
}


void tempHumidity::clearMinMax ()
{
  for (byte i = 0 ; i < TEMP_HUMIDITY_SENSORS_MAX ; i++ )
  {
    temp_values_min [i] = temp_values [i] ;
    temp_values_max [i] = temp_values [i] ;
  }
}


void tempHumidity::ds18b20RequestAllTemps ()
{
  oneWire.reset () ;
  oneWire.skip () ;
  oneWire.write (DS18B20_START_CONVERSION, DS18B20_POWER_ON) ;
}


void tempHumidity::ds18b20SelectReadTemp (uint8_t sensor)
{
  if (sensor < ds18b20_num_sensors)
  {
    oneWire.reset () ;                            // approx 1mS
    oneWire.select (ds18b20_address [sensor]) ;   // 64 bits * 65uS = 4.2mS
    oneWire.write (DS18B20_READ_SCRATCHPAD);      // 8 bits * 70uS = 0.6mS Request to read all data from DS18B20
  }
}


void tempHumidity::ds18b20ReadTemp (uint8_t sensor)
{
  if (sensor < ds18b20_num_sensors)
  {
    /*
    // this is now done in ds18b20SelectReadTemp (sensor)
    oneWire.reset () ;                            // approx 1mS
    oneWire.select (ds18b20_address [sensor]) ;   // 64 bits * 65uS = 4.2mS
    oneWire.write (DS18B20_READ_SCRATCHPAD);      // 8 bits * 70uS = 0.6mS Request to read all data from DS18B20
    */
    
    for ( byte i = 0; i < 9; i++)         // there are 9 bytes in the DS18B20 scratchpad
    {
      buffer [i] = oneWire.read () ;      // 9 * 8 * 65uS = 4.7mS [0] = lsb, [1] = msb, [8] = checksum
    }
    if (oneWire.crc8(buffer, 8) == buffer [8])
    {
      // temp range is -511°C to +512°C resolution 0.016°C
      // ERROR! this probably doesn't work for negative values! - or needs to be checked)
      temp_values [sensor] = ((buffer [1] << 8) | buffer [0]) << 2 ; 
    }
  }
}


void tempHumidity::htu21dRequest (uint8_t htu21dCommand)
{
  Wire.beginTransmission (HTU21D_ADDRESS) ;
  Wire.write (htu21dCommand) ;   // Start to calculate humidity without blocking the I2C bus
  Wire.endTransmission ();  
}


void tempHumidity::htu21dReadBytes ()
{
  Wire.requestFrom (HTU21D_ADDRESS, (uint8_t) 3) ;
  
  uint8_t counter = 0 ;
  while (Wire.available ())
  {
    buffer [counter++] = Wire.read () ;
  }

  /*  
  unsigned int raw = (buffer [0] << 8) | (buffer [1] & 0xFC) ;

  Serial.print ("raw=") ;
  Serial.print (raw) ;

  Serial.print (" crc=") ;
  Serial.print (buffer [2]) ;

  Serial.print (" computed=") ;
  Serial.print (oneWire.crc8(buffer, 3)) ;

  Serial.print (" counter=") ;
  Serial.print (counter) ;
  */
}

void tempHumidity::htu21dReadHumidity ()
{
  // Serial.print ("Humidity ") ;
  htu21dReadBytes () ;
  // Serial.println () ;
  
  // buffer [0] = 0x16 ;
  // buffer [1] = 0x87 ;
  
  // 60% rh = 34603 = 0x872B
  // (60*128*2 + 6*256)*256/125 = 34603
  
  // 95% rh = 52953 = 0xCED9
  // 5% rh = 5767 = 0x1687
  
  // (1508 * 125 / 256 - 1536) / 256 =
  
  // if (oneWire.crc8(buffer, 2) == buffer [2])
  {
  
    // works with long
    unsigned int raw = (buffer [0] << 8) | (buffer [1] & 0xFC) ;
    unsigned long raw_value ;
    raw_value = raw ;
    // Serial.print (" raw1=") ;
    // Serial.print (raw_value) ;
    raw_value *= 125 ;
    raw_value >>= 8 ;         // raw_value /= 256  : no negative values for humidity
    // Serial.print (" raw2=") ;
    // Serial.print (raw_value) ;
    raw_value -= 1536 ;    // 6 * 256
    raw_value >>= 2 ;         // divide by 4 = (256 / TEMP_HUMIDITY_TO_FLOAT)
    // Serial.print (" raw3=") ;
    // Serial.print (raw_value) ;
    
    // Serial.print (" val=") ;
    // Serial.print ((int) raw_value) ;
    // Serial.print (" %=") ;
    // Serial.println ((int) (raw_value / (unsigned int) TEMP_HUMIDITY_TO_FLOAT)) ;    
    
    temp_values [HTU21D_HUMIDITY_SENSOR] = (int) raw_value ; 
  }
}


void tempHumidity::htu21dReadTemp ()
{
  // Serial.print ("Temp ") ;
  htu21dReadBytes () ;
  // Serial.println () ;
  

  // buffer [0] = 0x4E ;
  // buffer [1] = 0x85 ;
  // buffer [2] = 0x6B ;
  
  // (1508 * 176 / 512 - 5997)/128 =

  // if (oneWire.crc8(buffer, 2) == buffer [2])
  {
    // long raw_value = (((buffer [0] << 8) | (buffer [1] & 0xFC)) * 175.72 * 2^16 - 46.85 * 2^16) >> 9 ;
    // 
    int raw = (buffer [0] << 8) | (buffer [1] & 0xFC) ;
    // Serial.print (" raw1=") ;
    // Serial.print (raw) ;
    long raw_value ;
    raw_value = raw ;
    // Serial.print (" raw2=") ;
    // Serial.print (raw_value) ;
    raw_value *= 176 ;    // 175.72 rounded up
    // Serial.print (" raw3=") ;
    // Serial.print (raw_value) ;
    raw_value /= 1024 ;    // 2^16 / TEMP_HUMIDITY_TO_FLOAT
    raw_value -= 2998 ;    // 46.85 * TEMP_HUMIDITY_TO_FLOAT
    // Serial.print (" raw4=") ;
    // Serial.print (raw_value) ;
    // Serial.print (" temp=") ;
    // Serial.println ((int) raw_value) ;
  
    /*
    Serial.print (" val=") ;
    Serial.print ((int) raw_value) ;
    Serial.print (" %=") ;
    Serial.println ((int) raw_value / TEMP_HUMIDITY_TO_FLOAT) ;
    */
    temp_values [HTU21D_TEMP_SENSOR] = raw_value ; // this assumes the conversion to int takes the bottom 16 bits
  }
}


void tempHumidity::checkTempHumidity ()
{
  if (temp_check_state) // i.e.  temp_check_state != TEMP_WAIT
  {
    switch (temp_check_state)
    {
    // case TEMP_WAIT :
    case TEMP_NOT_READY :
      ds18b20RequestAllTemps () ;                     // Start to calculate all DS18B20 temperatures
      htu21dRequest (HTU21D_MEASURE_TEMP_NOHOLD) ;    // Start to calculate HTU21D temperature
      temp_time_start = now () ;
      temp_millis_start = millis () ;
      temp_check_state = TEMP_CHECK_HTU21D_TEMP ;
      ds18b20_check_sensor = ds18b20_num_sensors ;
      break ;
    case TEMP_CHECK_HTU21D_TEMP :
      if (millis () - temp_millis_start > HTU21D_DELAY_MS)
      {
        htu21dReadTemp () ;
        htu21dRequest (HTU21D_MEASURE_HUMIDITY_NOHOLD) ;  // Start to calculate HTU21D humidity
        temp_check_state = TEMP_CHECK_DS18B20 ;           // Start to read all DS18B20 temperatures
      }
      break ;
    case TEMP_CHECK_DS18B20 :
      if (millis () - temp_millis_start > DS18B20_DELAY_MS)
      {
        if (ds18b20_check_sensor)
        {
          ds18b20SelectReadTemp (ds18b20_check_sensor - 1) ;    // this will send out the request to read from one of the DS18B20 devices
          temp_check_state = TEMP_READ_DS18B20 ;
        }
        if (!ds18b20_check_sensor)
        {
          temp_check_state = TEMP_CHECK_HTU21D_HUMIDITY ; 
          temp_millis_start = millis () ; // just to be sure
        }
      }
      break ;
    case TEMP_READ_DS18B20 :
      ds18b20ReadTemp ( --ds18b20_check_sensor) ;  // this will actually read the full scratchpad on selected DS18B20 device and update temps
      temp_check_state = TEMP_CHECK_DS18B20 ;      // now go back to reading the next available DS18B20 device
      break ;
    case TEMP_CHECK_HTU21D_HUMIDITY :
      if (millis () - temp_millis_start > HTU21D_DELAY_MS)
      {
        htu21dReadHumidity () ;
        // now recompute the min and max values
        for (byte i = 0 ; i < TEMP_HUMIDITY_SENSORS_MAX ; i++)
        {
          temp_values_min [i] = min (temp_values [i], temp_values_min [i]) ;
          temp_values_max [i] = max (temp_values [i], temp_values_max [i]) ;  
        }        
        temp_check_state = TEMP_ALL_READY ;        // all temperatures/humidity values have been read and can now be checked against Pcode
      }
      break ;
    }
  }
}


bool tempHumidity::isTempHumidityReady ()
{
  // called by the Pcode routine to see if new values are ready from all the temperature and humidity sensors
  return temp_check_state == TEMP_ALL_READY ;
}


void tempHumidity::setTempHumidityWait () 
{
  temp_check_state = TEMP_WAIT ;
}


void tempHumidity::setTempHumidityStart ()
{
  temp_check_state = TEMP_NOT_READY ;
}


void tempHumidity::readTemp (uint8_t sensor, TempValMinMax val_min_max, int& temp_value, uint8_t& temp_fraction)
{
  // returns the signed integer part and the fractional part (2 decimal places) of the temperature or humidity
  unsigned int fract ;
  switch (val_min_max)
  {
  case TEMP_VAL :
    temp_value = TempHumidity.temp_values [sensor] ;
    break ;
  case TEMP_MIN :
    temp_value = TempHumidity.temp_values_min [sensor] ;
    break ;
  case TEMP_MAX :
    temp_value = TempHumidity.temp_values_max [sensor] ;
    break ;    
  }
  fract = temp_value ;
  temp_value /= TEMP_HUMIDITY_TO_FLOAT ;  

  fract &= TEMP_HUMIDITY_TO_FLOAT - 1 ;  
  fract *= 100 ;  // fract needs to be unsigned int here
  fract /= TEMP_HUMIDITY_TO_FLOAT ;
  temp_fraction = fract ;  // convert to uint8_t
} 



tempHumidity TempHumidity ; 
