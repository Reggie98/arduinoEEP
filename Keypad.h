/* 
  Keypad.h        keypad library
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


#ifndef KEYPAD_H
#define KEYPAD_H

#include <Arduino.h>
#include <inttypes.h>

#include "Limits.h"
#include "EEpromMap.h"
#include "EEPROMAnything.h"

#include "MCP23017.h"

extern CEepromAnything EEPROM ;
extern mcp23017 Mcp23017 ; 
extern eeMap EeMap ;

#define KEYPAD_MCP_ADDR 0                   // the keypad is on the first MCP23017 device on GPIOA

#define KEYPAD_SAMPLE_RATE_MS  40           // 40ms for detecting keypad presses (20ms debounce) and also for sampling PIRs

#define KEYPAD_16KEYS_INPUT_BITS B00001111  // for 16-key keypads, D0-D3 are input bits, D4-D7 are output bits
#define KEYPAD_16KEYS_OUTPUT_BITS B11110000 // 


class keypad
{
public:
  keypad () ;
  void begin () ;
  void checkKeyPressed () ;
  bool isKeyReady () ;
  void setKeyProcessed () ;
  uint8_t getKeyPressed () ;
  
private:
  unsigned long keypad_millis_start ;
  uint8_t key_pressed ;
  enum KeyPressedState
  {
    KEY_WAIT,       // waiting for a new key pressed
    KEY_START,      // key pressed - but wait for the next sample to be sure (debounce)
    KEY_FOUND,      // new key pressed
    KEY_PROCESSED,  // new key has been processed (notify message sent to master, check for any PCODE commands)
    KEY_END         // key released - but wait for next sample to be sure (debounce)
  } ;
  KeyPressedState key_pressed_state ;
} ;

extern keypad Keypad ;

#endif  // KEYPAD_H
