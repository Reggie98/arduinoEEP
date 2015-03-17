/* 
  Keypad.cpp      keypad library
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

#include "keypad.h"


keypad::keypad ()         // constructor - nothing to do here
{
}

void keypad::begin ()     // initialise the keypad
{
  if (EEPROM.read (eeMap::OptionGpio) & eeMap::GpioKeypad16Keys)
  {
    // only need to set outputs if 16-key keypad in use 
    // 4-key keypad has common line permanently low
    Mcp23017.write_gpio (KEYPAD_MCP_ADDR, ~KEYPAD_16KEYS_OUTPUT_BITS ) ; // set all the keypad outputs D7 to D4 to zero
  }
  keypad_millis_start = 0 ;
  key_pressed_state == KEYPAD_WAIT ;
}

bool keypad::isKeyReady ()
{
  return key_pressed_state == KEYPAD_FOUND ;
}

void keypad::setKeyProcessed ()
{
  key_pressed_state = KEYPAD_PROCESSED ;
}

uint8_t keypad::getKeyPressed () 
{
  return key_pressed ;
}


void keypad::checkKeyPressed ()
{
  if ((millis () - keypad_millis_start) >= (unsigned long) KEYPAD_SAMPLE_RATE_MS )
  {
    keypad_millis_start = millis ()  ; 
    
    // read all but only select the four input lines D0 to D3
    key_pressed = (~ Mcp23017.read_gpio (KEYPAD_MCP_ADDR)) & KEYPAD_16KEYS_INPUT_BITS ;  
    
    if (key_pressed && (key_pressed_state == KEYPAD_WAIT))
    {
      key_pressed_state = KEYPAD_START ;
    } else
    if (key_pressed && (key_pressed_state == KEYPAD_START))
    {
      key_pressed_state = KEYPAD_FOUND ;
      // now check which key has been found and process it
      for (uint8_t i = 0; i <= 3 ; i++)
      {
        if (EEPROM.read (eeMap::OptionGpio) & eeMap::GpioKeypad16Keys)
        {
          Mcp23017.write_gpio (KEYPAD_MCP_ADDR, B11101111 << i ) ; // set one keypad output bit to zero
        }
        
        // read all but only select the four input lines D0 to D3
        key_pressed = ~ Mcp23017.read_gpio (KEYPAD_MCP_ADDR) & KEYPAD_16KEYS_INPUT_BITS ;  
        if (key_pressed) 
        {
          if (key_pressed == B00001000) 
          {
            key_pressed = 3 + i * 4 ;
          }
          else if (key_pressed == B00000100)
          {
            key_pressed = 2 + i * 4  ;
          }
          else 
          {
            key_pressed = (key_pressed - 1) + i * 4 ; // B00000010 -> i * 4 + 1, B00000001 -> i * 4
          }
          key_pressed = EEPROM.read (eeMap::KeypadMap + key_pressed) ; // convert to character
          break ; 
        }
      }
      if (EEPROM.read (eeMap::OptionGpio) & eeMap::GpioKeypad16Keys)
      {
        // set all the keypad outputs D7 to D4 to zero ready for next keypad read 
        Mcp23017.write_gpio (KEYPAD_MCP_ADDR, ~KEYPAD_16KEYS_OUTPUT_BITS ) ; 
      }
    } else 
    if (!key_pressed && key_pressed_state == KEYPAD_PROCESSED)
    {
      key_pressed_state = KEYPAD_END ;
    } else      
    if (!key_pressed && key_pressed_state == KEYPAD_END)
    {
      key_pressed_state = KEYPAD_WAIT ;
    }
  }
}

keypad Keypad ;
