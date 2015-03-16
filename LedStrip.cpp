/* 
  LedStrip.h      LedStrip library
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

#include "LedStrip.h"

ledStrip::ledStrip ()  
{
}


void ledStrip::setLedColourFromRGB (uint8_t rgb) 
{
  // convert a single byte rrrggbbb to CRGB format and put in led_colour
  uint8_t r, g, b ;
  r = rgb & B11100000 ;
  r |=  (r >> 3) | (r >> 5) ;
  g = (rgb & B00011000) << 3 ;
  g |= (g >> 2) | (g >> 4) | (g >> 6) ;
  b = rgb & B00000111 ;
  b = (b << 5) | (b << 2) | (b >> 1) ;
  led_colour = CRGB (r, g, b) ;
}


void ledStrip::setLedColour (uint8_t r, uint8_t g, uint8_t b) 
{
  led_colour = CRGB( r, g, b) ;
}


CRGB ledStrip::getLedColour ()
{
  return led_colour ;
}


void ledStrip::setLedMode (uint8_t new_mode) 
{
  led_mode = static_cast <LedModeState> (new_mode) ;
}


void ledStrip::setLedPosn (uint8_t new_posn)
{
  led_posn = new_posn ;
}


void ledStrip::checkUpdate ()
{
  if (led_mode && ((millis () - led_millis_start) >= LED_SAMPLE_RATE_MS ))
  {
    led_millis_start = millis () ;
    uint8_t leds_max = EEPROM.read (eeMap::LedNumLeds) ;
    uint8_t led_mode_max_tails = EEPROM.read (eeMap::LedModeMaxTails) ;
    uint8_t led_mode_tail_len = EEPROM.read (eeMap::LedModeTailLen) ;
    uint8_t led_mode_flash_rate = EEPROM.read (eeMap::LedModeFlashRate) ;
    switch (led_mode)
    {
    case LedModeFlashRandom :
      // flashes all the leds to a specified colour
      if (led_posn == 0)
      {
        // fill_solid ( &(leds[0]), leds_max, CRGB::Black ) ;
        // CRGB black = CRGB::Black ;
        // showColor (black, 0) ;
        led_posn = led_mode_flash_rate ;
      } else if (led_posn == led_mode_flash_rate)
      {
        setLedColourFromRGB (micros ()) ;  // low byte of micros () gives a random number 0-255
        fill_solid ( &(leds[0]), leds_max, led_colour ) ;
        led_posn -- ;
      } else
      {
        led_posn -- ;
      }
      break ;
    case LedModeWarningFlash :
      if (led_posn == 0)
      {
        setLedColourFromRGB (EEPROM.read (eeMap::LedOnRgb)) ;
      }
      if (led_posn == led_mode_flash_rate)
      {
        led_colour = CRGB::Black ;
      }
      fill_solid ( &(leds[0]), leds_max, led_colour ) ;
      if (++led_posn >= led_mode_flash_rate * 2) led_posn = 0 ;
      break ;
    case LedModeChaseRandom :
    // case LedModeChaseSlowRandom :
      // 
      fill_solid ( &(leds[(led_posn < leds_max) ? led_posn : leds_max * 2 - led_posn - 1 ]), 1, CRGB::Black ) ; // switch off the current led
      led_posn ++ ;
      if (led_posn >= leds_max * 2) led_posn = 0 ; // back to the start
      // if (led_mode == LedModeChaseRandom || (led_mode == LedModeChaseSlowRandom && led_posn == 0 ))
      {
        setLedColourFromRGB (micros ()) ;  // low byte of micros () gives a random number 0-255
      }
      fill_solid ( &(leds[(led_posn < leds_max) ? led_posn : leds_max * 2 - led_posn - 1]), 1, led_colour ) ; // switch off the current led
      break ;
    case LedModeChaseTails :
      if (led_delay == 0)
      {
        fill_solid ( &(leds[0]), leds_max, CRGB::Black ) ;  // put all the leds off
        led_hsv.s = 255 * 0.8 ;
        for (uint8_t head = led_mode_max_tails ; head ; head-- )
        {
          led_hsv.h = (head - 1) * (255 / led_mode_max_tails) ; // evenly spaced colours
          for (uint8_t tail = led_mode_tail_len ; tail ; tail-- )
          {
            led_hsv.v = (255 / led_mode_tail_len ) * tail ;
            int new_posn = led_posn + leds_max * 2 * (head - 1) / led_mode_max_tails - led_mode_tail_len + tail ; 
            if (new_posn >= leds_max * 2) new_posn -= leds_max * 2 ;
            if (new_posn >= leds_max) new_posn = leds_max * 2 - new_posn - 1 ;
            if (new_posn < 0) new_posn = -new_posn ;
            if (leds [new_posn] == CRGB(CRGB::Black) || led_posn < leds_max) 
            {
              hsv2rgb_rainbow (led_hsv, led_colour) ;
              leds [new_posn] = led_colour ;
            }
          }
        }
        led_delay = led_mode_flash_rate >> 2 ;
        if (++led_posn >= leds_max * 2) led_posn = 0 ; // back to the start    
      } else
      {
        led_delay -- ;
      }
      break ;
      /*
    case LedModeFillRandom :
      // boring and blocky - distracting
      if (led_posn == 0)
      {
        led_posn = 5 ;
        for (int i = leds_max - 1 ; i >= 0 ; i--)
        {
          setLedColourFromRGB (micros ()) ;
          leds [i] = led_colour ;
        }
      } else
      {
        led_posn -- ;
      }
      break ;
      */
    case LedModeFillSlow :
      setLedColourFromRGB (micros ()) ;
      leds [(led_posn < leds_max) ? led_posn : leds_max * 2 - led_posn - 1 ] = led_colour ;
      led_posn ++ ;
      if (led_posn >= leds_max * 2) led_posn = 0 ; // back to the start
      break ;
    case LedModeBreathe :
      if (led_posn == 0)
      {
        // get a new colour every breath cycle 
        led_hsv.h = micros () ; // bottom 8 bits of micros () is a pseudo-random number
        led_hsv.s = 255 * 0.8 ;
      } 
      led_hsv.v = (led_posn < 128) ? led_posn * 2 : (255 - led_posn) * 2 ;
      hsv2rgb_rainbow (led_hsv, led_colour) ; // convert the hsv value to rgb
      fill_solid ( &(leds[0]), leds_max, led_colour ) ; // set all the leds to the new colour/brightness
      led_posn ++ ;
      break ;
    case LedModeColourCycle :
      led_hsv.h = led_posn ;
      led_hsv.s = 255 * 0.8 ;
      led_hsv.v = 255 * 0.8 ;
      hsv2rgb_rainbow (led_hsv, led_colour) ;
      fill_solid ( &(leds[0]), leds_max, led_colour ) ; // set all the leds to the new colour
      led_posn ++ ;
      break ;
    case LedModeRainbow :
      if (led_posn == 0 )
      {
        fill_rainbow ( &(leds[0]), leds_max, 0, 5 ) ; 
        led_posn = 1 ;
      } else
      {
        led_colour = leds [leds_max - 1] ;
        for (uint8_t i = leds_max - 1 ; i ; i--)
        {
          leds [i] = leds [i - 1] ;
        }
        leds [0] = led_colour ;
      }
      break ;
    }
    this -> show () ; // Sends the values to the LEDs        
  }
}  


void ledStrip::setLedModeFromPcode (uint8_t led_on, uint8_t new_mode) 
{  
  // new_mode = 0 to 6 - switch on or off one of up to 8 groups of leds
  // new_mode = 7 - 
  // new_mode = 8 to 14 - put into LED_MODE L1: to L7:
  // new_mode = 15 put all leds off
  setLedColourFromRGB ( EEPROM.read (led_on ? eeMap::LedOnRgb : eeMap::LedOffRgb )) ;  // updates led_colour
  if (new_mode < 7)
  {
    uint8_t nLedsInGroup = EEPROM.read (eeMap::LedGroup) ;
    fill_solid ( &(leds[nLedsInGroup * new_mode]), nLedsInGroup, led_colour ) ;
    led_mode = LedModeNormal ;
  } else if (new_mode == 7)
  {
    led_mode = static_cast <LedModeState> (static_cast <int> (led_mode + 1) ) ;
    if (led_mode == LedModeMax) led_mode = LedModeBreathe ;
    led_posn = 0 ;
  } else
  {
    fill_solid ( &(leds[0]), EEPROM.read (eeMap::LedNumLeds), CRGB::Black ) ; // start with all leds off
    led_mode = (new_mode >= 15) ? LedModeNormal : static_cast <LedModeState> (new_mode - 7) ;
    led_posn = 0 ;
  }
  this -> show () ; // Sends the values to the LEDs
}


ledStrip LedStrip ; 
