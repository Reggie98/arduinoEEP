/* 
  MCP23107.h      MCP23107 library
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


#ifndef MCP23107_H
#define MCP23107_H

#include <Arduino.h>
#include <inttypes.h>
#include <Wire.h>

#include "Limits.h"
#include "EEpromMap.h"
#include "EEPROMAnything.h"

extern CEepromAnything EEPROM ;

class eeMap ;

extern TwoWire Wire;


#define MCP_I2C_ADDR 0x20                 // this is the fixed part of the address for the MCP devices

#define MCP_PORT_BIT B00100000            // bit 5 set indicates MCP #x20 to #x27
#define MCP_PORT_SEL_BIT   B00010000      // bit 4 set indicates GPIOB on MCP
#define MCP_PORT_ADDR_BITS  B00000111     // I2C address (up to 8 MCPs = 64 I/O devices) 


// internal addresses for the I2C MCP - 'A' register only (for 'B' register add 0x10 = MCP_PORT_SEL_BIT)
#define MCP_IOCON_INIT 0x0A               // NOTE the MCP initially has IOCON.BANK=0. It MUST be set high before the MCP will act like two MCP23008s
#define MCP_IOCON_BANK_SEQOP B10100000    // sets the bank bit to 1, disables sequential operation

#define MCP_IODIR 0x00
#define MCP_IPOL 0x01
#define MCP_GPINTEN 0x02
#define MCP_DEFVAL 0x03
#define MCP_INTCON 0x04
#define MCP_IOCON 0x05
#define MCP_GPPU 0x06
#define MCP_INTF 0x07
#define MCP_INTCAP 0x08
#define MCP_GPIO 0x09
#define MCP_OLAT 0x0A


class mcp23017
{
public:
  mcp23017 () ;
  void begin () ;
  uint8_t read_gpio (uint8_t) ;  // mcp_num is constrained to 0-7
  void read_gpio_all () ;
  void save_gpio_all () ;
  uint8_t get_gpio_current (uint8_t) ;  // mcp_num is not checked - should be 0-7 only
  uint8_t get_gpio_current_bit (uint8_t, uint8_t) ;  // mcp_num is not checked - should be 0-7 only
  uint8_t get_gpio_last (uint8_t) ;      // mcp_num is not checked - should be 0-7 only
  void write_gpio (uint8_t, uint8_t) ;  // mcp_num is constrained to 0-7

private:
  uint8_t read_byte (uint8_t, uint8_t) ;
  void write_byte (uint8_t, uint8_t, uint8_t) ;
  uint8_t mcp_num_devices ;                         // the number of 16-port MCP23107 devices on this system

  // to save memory, these could be sized in the constructor depending on the actual number of MCP23107 devices attached
  uint8_t mcp_gpio_current [MCP23017_MAX * 2] ;      // the current values for the gpios [0] = #20 GPIOA, [1] = #20 GPIOB, [2] = #21 GPIOA etc.
  uint8_t mcp_gpio_last [MCP23017_MAX * 2] ;  // 
} ;

extern mcp23017 Mcp23017 ; 

#endif    // MCP23107_H
