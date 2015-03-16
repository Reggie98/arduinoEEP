/* 
  Limits.h        system limits for project, and arduino fixed pin definitions
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


#ifndef LIMITS_H
#define LIMITS_H

// System limitations
#define COMPILE_NOKIA_5110   1          // include the Nokia 5110 code for 6 rows, 14 column B/W display
#define COMPILE_LEDSTRIP     1          // include the single wire WS8211 chip RGB LED driver code
#define COMPILE_DS1307RTC    1          // rtc module

#define LEDS_MAX             60         // don't exceed 127; memory used = 3* = 150 bytes; could also limit max in EEPROM (not yet implemented)
#define TEMP_SENSORS_MAX     6          // to increase - need to increase size of index in EE_PCODE_TEMPS from 16 (6 DS18B20s, 2 for HTU21D)
#define ANALOG_INPUTS_MAX    8          // maximum number of analog inputs (NB. 8 is only 6 because of I2C pins)
#define MCP23017_MAX         4          // maximum number of 16 port MCP23107 chips on system


// Fixed external pin connections to Arduino

//                           0          // pin 0 RXD
//                           1          // pin 1 TXD
//                           2          // pin 2 IRQ0 attached to counter2
//                           3          // pin 3 IRQ1 attached to counter3; PWM3-Timer2

#define TXE_PIN              4          // enable the transmition onto the RS485 wires when high, 
                                        //   or receiving (reading) the RS485 incoming data when low
#define PORTD_TXE_BIT        B00010000  // pin 4
#define RS485_TX_ENABLE      1          // TXE
#define RS485_RX_ENABLE      0          // RXE - but maybe always allow read so that transmitted data can be confirmed
#define RS485_TX_DISABLE     0          // on the prototype, the rx_enable is hard-wired to off; 
                                        //   this only disables transmission (i.e. doesn't enable receipt of data)

#define BEEPER_PIN           5          // PWM5-Timer0; MUST be defined before including "Beeper.h"

#define LED_PIN              6          // LED WS2812 output; PWM6-Timer0

#define ONE_WIRE_PIN         7          // DS18B20 temperature sensor on pin 7

//                           D8         // PIN_DC for Nokia_5110
//                           D9         // PIN_RESET for Nokia_5110
//                           D10        // PIN_SCE for Nokia_5110
//                           D11        // SPI MOSI
//                           D12        // SPI MISO (not used by Nokia_5110)
//                           D13        // SPI CLK

//                           A0
//                           A1
//                           A2
//                           A3
//                           A4         // Analog Input 4 = I2C SDA
//                           A5         // Analog Input 5 = I2C SCL 
//                           A6
//                           A7

#endif    // LIMITS_H
