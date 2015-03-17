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


#ifndef EEPROMANYTHING_H
#define EEPROMANYTHING_H

#include <Arduino.h>    // for type definitions
// WARNING: modified so it doesn't have extern EEPROM (needs to be updated every time Arduino IDE is updated)
#include <EEPROM.h>
#include <Time.h>

#include "EEpromMap.h"


// extern eeMap EeMap ;
extern volatile unsigned long interrupt_count0 ;
extern volatile unsigned long interrupt_count1 ;

#include <inttypes.h>


class CEepromAnything
{

public:
  CEepromAnything () ;
  // void begin () ;                // not needed

  // BEGIN copied from ...\Arduino\hardware\arduino\avr\libraries\EEPROM\EEPROM.cpp
  uint8_t read (int) ;
  void write (int, uint8_t) ;
  // END copied from ...\Arduino\hardware\arduino\avr\libraries\EEPROM\EEPROM.cpp

  void checkUpdate () ;              // updates the time and interrupt counter values every hour
  void updateByte (int, uint8_t) ;  // writes the value to the EEPROM address only if it is different from the current stored value
  void updateLong (int, unsigned long) ;
  unsigned long readLong (int) ;

  /*
  template <class T> void writeAnything (int, const T&) ;
  template <class T> void readAnything (int, T&) ;
  */
  
  
private:
  typedef enum 
  {
    EEPROM_STATE_WAIT,
    EEPROM_STATE_SAVE_INT0,
    EEPROM_STATE_SAVE_INT1,
    EEPROM_STATE_SAVE_TIME    
  } ;
  uint8_t m_eeprom_state ;
  uint8_t m_last_hour ;                // save values every hour
  unsigned long m_eeprom_save_long ;   // the value currently being saved
} ;

extern CEepromAnything EEPROM ;

#endif   // EEPROMANYTHING_H

