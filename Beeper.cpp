/* 
  Beeper.cpp      beeper library
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

#include "Beeper.h"


beeper::beeper ()    // constructor - nothing to do here
{
}

void beeper::begin ()      // initialise the beeper
{
  beeper_state = BeepOff ;
  beeper_millis_start = 0 ;
  beeper_state_len = 0 ;
}

void beeper::setBeep (uint8_t new_beeper_state)
{
  beeper_millis_start = millis () ;
  beeper_state = new_beeper_state ;
  if (beeper_state & 0x01)
  {
    // switch on the beeper if new state is an odd number
    analogWrite (BEEPER_PIN, EEPROM.read (eeMap::BeeperTone)) ; 
    beeper_state_len = (beeper_state < BeepStateLong) ? BEEP_SHORT_ON_MS : BEEP_LONG_ON_MS ;
  } 
  else
  {
    // switch off the beeper if new state is an even number
    analogWrite (BEEPER_PIN, 0) ; 
    beeper_state_len = (beeper_state < BeepStateLong) ? BEEP_SHORT_OFF_MS : BEEP_LONG_OFF_MS ;
  }   
}

void beeper::setBeepPcode (uint8_t pcode_val)
{
  // pcode_val = 0 to 15; 0-7 gives 1-8 short beeps, 8-15 gives 1-8 long beeps
  setBeep ((pcode_val < 8) ? pcode_val * 2 + BeepOnce : (pcode_val - 8) * 2 + BeepOnceLong) ;
}

void beeper::processBeep ()
{
  if ( beeper_state && ((millis () - beeper_millis_start) >= (unsigned long) beeper_state_len))
  {
    beeper_state -- ;
    if (beeper_state == BeepStateLong) beeper_state = BeepOff ; // this switches off the beeper and terminates the beeps
    setBeep (beeper_state) ; // set the new state of the beeper
  }
}

beeper Beeper ;
