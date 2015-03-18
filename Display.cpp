/* 
  Display.cpp     Display on Nokia 5110 6*14 character LCD
  Copyright       (C) 2015 Reggie Thomson.  All rights reserved.

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

#include "Display.h"


display::display ()  
{
  is_display_interactive = false ;
  display_normal_min_max = 0 ;
}

void display::gotoCharPosition (uint8_t char_position) 
{  
  // uint8_t y = char_position / PCD8544_COLS ;
  // while (char_position >= PCD8544_COLS) char_position -= PCD8544_COLS ;
  gotoXY ((char_position % PCD8544_COLS) * PCD8544_COLS_PER_CHAR, char_position / PCD8544_COLS ) ;
}


void display::printDigits (int digits)
{
  if (digits < 10)
  {
    print ('0') ;
  }
  print (digits) ;
}

bool display::isDisplayInteractive ()
{
  return is_display_interactive ;
}

void display::setDisplayMode (bool new_mode)
{
  is_display_interactive = new_mode ;
}

void display::setDisplayNormalMinMax (uint8_t new_normal_min_max)
{
  display_normal_min_max = new_normal_min_max ;
}

void display::printTime ()
{
  // fixed size 14 bytes
  printDigits (day ()) ;
  print ('/') ;
  printDigits (month ()) ;
  print (' ') ;
  
  printDigits (hour ()) ;
  print (':') ;
  printDigits (minute ()) ;
  print (':') ;
  printDigits (second ()) ;
}

uint8_t display::printValueFraction (int value, uint8_t fraction, bool withTwoDecimalPlaces = false)
{
  uint8_t display_position = 0 ;
  display_position += print (value) ;      
  print ('.') ;
  if (fraction < 10 && withTwoDecimalPlaces)
  {
    print ('0') ;
    display_position ++ ;
  }
  if (fraction > 10 && !withTwoDecimalPlaces) fraction /= 10 ;
  display_position += print (fraction) + 1 ; // for the '.'  
  return display_position ;
}

uint8_t display::printFixedMessage (uint8_t message_number)
{
  uint8_t chars_printed = 0 ;
  if (!is_display_interactive)
  {
    unsigned int eeprom_position = EEPROM.read (eeMap::MessageIndex + message_number) ;
    /*
    Serial.print (", eeprom_position=") ;
    Serial.print (eeprom_position) ;
    */
    if (eeprom_position)
    {
      eeprom_position += (eeMap::MessageIndex & 0xFF00) ;
      // Serial.print (',') ;
      // Serial.print (eeprom_position) ;
      char ch = EEPROM.read (eeprom_position++) ;
      // Serial.print (", char=") ;
      while (ch)
      {
        print (ch) ;
        // Serial.print (ch, DEC) ;
        ch = EEPROM.read (eeprom_position++) ;
        // if (ch) Serial.print (',') ;
        chars_printed ++ ;
      }
    }
    // Serial.println () ;
  }
  return chars_printed ;
}

/*
    DisplayMap =              816      // 48:3    16 * 2 bytes for the display on the display (char position, display item)
                                          // e.g. 0, date/time, 14, analog 0, 21, analog 1, 28, temp 0, 35, temp 1, 42, counter 2,
                                          //      56, relay 0-7 binary, 66, MCP GPIOB #4, 70, menu string: "Menu #"
*/

void display::checkUpdate ()
{
  // update the display screen every second
  // NOTE: cannot use other SPI device during display update
  if (now () != display_start_time && !is_display_interactive)
  {
    display_start_time = now () ;

    unsigned int display_map_ptr = 0 ;
    uint8_t display_position = EEPROM.read (eeMap::DisplayMap + display_map_ptr++) ;
    uint8_t display_position_start = display_position ;
    
    // writeLcdPrep () ;  // make sure the SPI is ready to accept data transfers to the LCD
    while (display_position < PCD8544_ROWS * PCD8544_COLS)
    {
      gotoCharPosition (display_position) ;
      uint8_t display_item = EEPROM.read (eeMap::DisplayMap + display_map_ptr++) ;
      uint8_t display_item_high = display_item & 0xF0 ;
      uint8_t display_item_low = display_item & 0x0F ;
      if (display_item == DisplayDateTime)
      {
        printTime () ;
        display_position += 14 ;
      } else
      if (display_item_high == DisplayPort0)
      {
        // digital IO ports, where 2-13 => digital pins D2-D13, 
        // 14-15 => analog A0-A1 (as digital), 0-1 => analog A2-A3 (as digital)
        // display e.g. 0D1 or 17D0
        
        if (display_item_low < 2) display_item_low += 16 ;
        display_position += print (display_item_low) ;
        print ('D') ;
        print (digitalRead (display_item_low)) ;
        display_position += 2 ; // 'D' and '0' or '1'
      } else
      if (display_item_high == DisplayAnalog0)
      {
        // analog ports A0-A3, A6-A7
        display_position += print (display_item_low) ;
        print ('A') ;
        display_position += print (AnalogInputs.readAnalog (display_item_low, static_cast <analogInputs::ArduinoValMinMax> (display_normal_min_max))) + 1 ;
      } else
      if (display_item_high == DisplayMCP0 && display_item_high <= DisplayMCP3)
      {
        display_position += print (display_item) ;
        display_item -= DisplayMCP0 ;
        print ('I') ;
        display_position += print (Mcp23017.get_gpio_current_bit (display_item >> 3, display_item & B00000111)) + 1 ;      
      } else 
      if (display_item == DisplayCounter2 || display_item == DisplayCounter3)
      {
        display_position += print (display_item_low) ;
        print ('i') ;
        display_position += print ((display_item_low == 0) ? interrupt_count0 : interrupt_count1) + 1 ;      
      } 
      if (display_item_high == DisplayTemp0)
      {
        display_position += print (display_item_low) ;
        if (display_item_low == HTU21D_HUMIDITY_SENSOR)
        {
          print ('H') ;
        } else
        {
          print ('T') ;
        }
        int value ;
        uint8_t fraction ;
        TempHumidity.readTemp (display_item_low, static_cast <tempHumidity::TempValMinMax> (display_normal_min_max), value, fraction ) ;
        display_position += printValueFraction (value, fraction, false) + 1 ;
        } else
      if (display_item == DisplayArduinoTemp)
      {
        int value ;
        uint8_t fraction ;
        print ('A') ;
        print ('T') ;
        AnalogInputs.readArduinoTempC (static_cast <analogInputs::ArduinoValMinMax> (display_normal_min_max), value, fraction) ;
        display_position += printValueFraction (value, fraction, false) + 2 ;    // for "AT"          
      } else
      if (display_item == DisplayArduinoVolts)
      {
        int value ;
        uint8_t fraction ;
        print ('A') ;
        print ('V') ;
        AnalogInputs.readArduinoVcc (static_cast <analogInputs::ArduinoValMinMax> (display_normal_min_max), value, fraction) ;
        display_position += printValueFraction (value, fraction, true) + 2 ;  // for "AV"      

        /*
        Serial.print ("AV=") ;
        Serial.print (AnalogInputs.readArduinoVccInt ()) ;
        Serial.print ('.') ;
        Serial.println (AnalogInputs.readArduinoVccFract ()) ;
        */
      } else
      if (display_item_high == DisplayMessage)
      {
        display_position += printFixedMessage (display_item_low) ;
      }
      
      if (display_position == display_position_start) // display_position won't change unless there was a valid item printed
      {
        display_position_start = EEPROM.read (eeMap::DisplayMap + display_map_ptr++) ;
      } else
      {
        display_position_start = EEPROM.read (eeMap::DisplayMap + display_map_ptr++) ;
        while (display_position < display_position_start)
        {
          print (' ') ;   // fill the rest of the area with space
          display_position ++ ;
        }
      }
      display_position = display_position_start ;
    } 
    
    // now write the normal/min/max 
    display_position = EEPROM.read (eeMap::DisplayMinMaxPosition) ;
    if (display_position)
    {
      gotoCharPosition (display_position) ;
      printFixedMessage (display_normal_min_max) ;
    }
     
    // writeLcdDone () ; // now finished writing to the SPI LCD 
  }
}

display Display ; 