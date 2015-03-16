/* 
  Beeper.h        beeper library
  Copyright       (C) 2014 Reggie Thomson.  All rights reserved.

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

#ifndef BEEPER_H
#define BEEPER_H

#include <Arduino.h>
#include <inttypes.h>

#include "Limits.h"
#include "EEpromMap.h"
#include "EEPROMAnything.h"

extern CEepromAnything EEPROM ;

// CHANGE: Don't like having these as global variables when they are only used in this class
// but I wan't them defined in the .h file
#define BEEP_SHORT_ON_MS    20 
#define BEEP_SHORT_OFF_MS   (250 - BEEP_SHORT_ON_MS)
#define BEEP_LONG_ON_MS     250
#define BEEP_LONG_OFF_MS    (500 - BEEP_LONG_ON_MS)

class beeper
{
public:
  beeper () ;
  void begin () ;
  void setBeep (uint8_t) ; // not a beepTone_t, since all values are valid
  void setBeepPcode (uint8_t) ;
  void processBeep () ;
  
  // CHANGE: not sure I like this either, since all values become int 
  //  and the compiler might do all calculations inefficiently in int before converting
  enum beepTone_t
  {
    BeepOff =       0,
    BeepOnce =      1,
    BeepTwice =     3,
    BeepStateLong = 50,
    BeepOnceLong =  51,
    BeepTwiceLong = 53
  } ;

private:
  uint8_t beeper_state ;  // not a beepTone_t, since all values are valid
  unsigned long beeper_millis_start ;
  uint16_t beeper_state_len ;

} ;

extern beeper Beeper ;

#endif  // BEEPER_H
