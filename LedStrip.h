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


#ifndef LEDSTRIP_H
#define LEDSTRIP_H

// #include <Arduino.h>
#include <inttypes.h>
#include <Time.h>
#include <FastLED.h>

#include "Limits.h"
#include "EEpromMap.h"
#include "EEPROMAnything.h"

#define LED_SAMPLE_RATE_MS          25UL        // 25ms = 40Hz (NB unsigned long)

extern eeMap EeMap ;
extern CEepromAnything EEPROM ;

class ledStrip : public CFastLED
{

public:
  ledStrip () ;

  typedef enum LedModeState
  {
    LedModeNormal,                  // = 0   Ls,f,rrr,ggg,bbb   or Ls,f,rgb   normal mode (fill from s to f with colour rgb
    LedModeBreathe,                 // L1: fill all the leds slowly with a gradually brightening then dimming random colour
    LedModeFlashRandom,             // L2: all leds flash a random colour
    LedModeChaseRandom,             // L3: single led of random colour chases up and down the leds
    LedModeChaseTails,              // L4: a number of heads with diminishing tails chase each other up and down the strip
    // LedModeChaseSlowRandom,      // L4: single led of set colour chases up and down the leds,
                                    //     and changes colour at end of each chase (boring!)
    // LedModeFillRandom,           // L5: fill all the leds with random colours that change every sample rate
    LedModeFillSlow,                // L5: fill all the leds with random colours and chase up and down changing the colours
    LedModeRainbow,                 // L6: a rainbow of colours moves through the display
    // LedModeFlowRandom,           // L8: fill with random colours and flow through the display
    LedModeColourCycle,             // L7: cycle through the hue 0-255, all leds with same colour
    LedModeWarningFlash,            // L8: flashing red for warning condition
    LedModeMax
  } ;

  void checkUpdate () ;
  void setLedColour (uint8_t, uint8_t, uint8_t) ;
  void setLedColourFromRGB ( uint8_t ) ;
  CRGB getLedColour () ;
  void setLedMode (uint8_t) ;
  void setLedModeFromPcode (uint8_t, uint8_t) ;
  void setLedPosn (uint8_t) ;
  CRGB leds [LEDS_MAX] ;  
  
  
private:

  LedModeState led_mode = LedModeNormal ;  // 
  
  unsigned long led_millis_start = 0 ;

  uint8_t led_posn  ;       // position of the current 'cursor' for special led modes
  uint8_t led_delay = 0 ;   // counter to delay incrementing the led_posn
  CRGB led_colour ;         // current rgb colour for special led modes
  CHSV led_hsv ;

} ;

extern ledStrip LedStrip ; 


#endif    // LEDSTRIP_H
