/* 
  EEPROMAnything.h  eeprom read or write anything, and eeprom update only if new value is different from current
  Copyright         (C) 2014 Reggie Thomson.  All rights reserved.
  
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

// BEGIN copied from ...\Arduino\hardware\arduino\avr\libraries\EEPROM\EEPROM.cpp
#include <avr/eeprom.h>
#include "Arduino.h"
#include "EEPROM.h"
// END copied from ...\Arduino\hardware\arduino\avr\libraries\EEPROM\EEPROM.cpp

#include "EEPROMAnything.h"
#include "Alarms.h"


CEepromAnything::CEepromAnything ()
{
}

/*
void CEepromAnything::begin ()
{
  m_last_hour = hour () ;
}
*/

// BEGIN copied from ...\Arduino\hardware\arduino\avr\libraries\EEPROM\EEPROM.cpp
uint8_t CEepromAnything::read (int address)
{
	return eeprom_read_byte ((unsigned char *) address) ;
}

void CEepromAnything::write (int address, uint8_t value)
{
	eeprom_write_byte ((unsigned char *) address, value) ;
}
// END copied from ...\Arduino\hardware\arduino\avr\libraries\EEPROM\EEPROM.cpp

void CEepromAnything::checkUpdate ()
{
  if (m_eeprom_state || m_last_hour != hour () )
  {
    int eeprom_address ;
    m_last_hour = hour () ;
    // Serial.println ("Eeprom") ;
    switch ((m_eeprom_state + 3) >> 2)
    {
    case EEPROM_STATE_WAIT :
      m_eeprom_save_long = now () ;
      m_eeprom_state = EEPROM_STATE_SAVE_TIME * 4 ;
      // NO break!
    case EEPROM_STATE_SAVE_TIME :
      eeprom_address = eeMap::Time ;
      /*
      Serial.println () ;
      Alarms.serialPrintTime (now ()) ;
      Serial.println () ;
      */
      break ;
    case EEPROM_STATE_SAVE_INT1 :
      eeprom_address = eeMap::InterruptCount1 ;
      break ;
    case EEPROM_STATE_SAVE_INT0 :
      eeprom_address = eeMap::InterruptCount0 ;
      break ;
    }
    
    /*
    Serial.print ("Save:") ;
    Serial.print (eeprom_address) ;
    Serial.print (",state:") ;
    Serial.println (m_eeprom_state) ;
    */
    
    uint8_t num_byte_to_save = m_eeprom_state & 0x03 ;    // 4 bytes per long value
    updateByte (eeprom_address + num_byte_to_save, m_eeprom_save_long >> (num_byte_to_save * 8)) ; // this will write low byte of long value shifted
    // note: the order of writing for successive checkUpdate () calls is 0, 3, 2, 1
    
    m_eeprom_state -- ;
    if (m_eeprom_state == EEPROM_STATE_SAVE_INT1 * 4)
    {
      m_eeprom_save_long = interrupt_count1 ;
    }
    if (m_eeprom_state == EEPROM_STATE_SAVE_INT0 * 4)
    {
      m_eeprom_save_long = interrupt_count0 ;
    }  
  }
}

void CEepromAnything::updateByte (int eeprom_address, uint8_t value)
{
  if (read (eeprom_address) != value) 
  {
    write (eeprom_address, value) ; 
  }
}

/*
template <typename T> void CEepromAnything::writeAnything (int eeprom_address, const T* &value)
{
  // can only write a max of 255 bytes at any one time
  const byte* p = (const byte*)(const void*) &value;
  for (uint8_t i = 0; i < sizeof(value); i++)
  {
    updateByte (eeprom_address++, *p++) ;
  }
}

template <typename T> void CEepromAnything::readAnything (int eeprom_address, T* &value)
{
  // can only read a max of 255 bytes at any one time
  byte* p = (byte*)(void*) &value ;
  for (uint8_t i = 0; i < sizeof(value); i++)
  {
    *p++ = read(eeprom_address++) ;
  }
}
*/

void CEepromAnything::updateLong (int eeprom_address, unsigned long value)
{
  for (uint8_t i = 0; i < 4 * 8 ; i += 8)
  {
    updateByte (eeprom_address++, (uint8_t) (value >> i)) ; // value >> i is the bottom 8 bits
  }  
}

unsigned long CEepromAnything::readLong (int eeprom_address)
{
  unsigned long value = 0 ;
  unsigned long value_unsigned ;
  for (uint8_t i = 0; i < 4 * 8 ; i += 8)
  {
    value_unsigned = read (eeprom_address++) ;
    value |= value_unsigned << i ;
  }  
  return value ;
}


CEepromAnything EEPROM ;  // declare one instance of the class CEepromAnything EEPROM ;

