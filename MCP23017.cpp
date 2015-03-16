/* 
  MCP23017.cpp    MCP23017 library
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

#include "MCP23017.h"


mcp23017::mcp23017 ()    // constructor - nothing to do here
{
}

void mcp23017::begin ()
{
  mcp_num_devices = EEPROM.read (eeMap::McpNumDevices) ;  // there are up to 4 MCPs
  for (uint8_t mcp_num = 0; mcp_num < mcp_num_devices * 2; mcp_num++)
  {
    bool mcp_bank = mcp_num & B00000001 ;  // Register A (false) or B (true)
    uint8_t mcp_port_sel = mcp_bank ? MCP_PORT_SEL_BIT : 0 ;
    uint8_t mcp_i2c_addr = MCP_I2C_ADDR + (mcp_num >> 1) ;
    
    mcp_gpio_current [mcp_num] = 0xFF ;  // all outputs will be set to high

    // set the bank bit to 1 (it is initially 0) only on the A 
    if (!mcp_bank)
    {
      write_byte (mcp_i2c_addr, MCP_IOCON_INIT, MCP_IOCON_BANK_SEQOP ) ;   // set the bank bit to 1 so that A and B registers are bank selectable
                                                                          // (by bit 4), and sequential operation is off
    }

    // make sure all the outputs are switched off (= high for relays) before we enable all the outputs (default value on reset is 0 = relays ON)
    write_byte (mcp_i2c_addr, MCP_GPIO + mcp_port_sel, mcp_gpio_current [mcp_num] ) ;

    // set pullups
    write_byte (mcp_i2c_addr, MCP_GPPU + mcp_port_sel, EEPROM.read (eeMap::McpGppu + mcp_num) ) ; 

    // now select the IO direction for the port
    write_byte (mcp_i2c_addr, MCP_IODIR + mcp_port_sel, EEPROM.read (eeMap::McpIodir + mcp_num) ) ; 
    
    // make sure all the relays are switched off - MAYBE this is not needed
    write_byte (mcp_i2c_addr, MCP_GPIO + mcp_port_sel, mcp_gpio_current [mcp_num] ) ; 
    mcp_gpio_current [mcp_num] = read_gpio (mcp_num) ; // read the gpio inputs
    mcp_gpio_last [mcp_num] = mcp_gpio_current [mcp_num] ;  // set all the last states, too
  }
}


uint8_t mcp23017::read_gpio (uint8_t mcp_num)
{
  // CHANGE - potential bug if MCP23017_MAX is not 1,2,4,8 etc.
  mcp_num &= MCP23017_MAX * 2 - 1 ; // = B0000111 for MCP23017_MAX == 4

  // mcp_num is now 0 to 7 where even numbers are GPIOA and odd numbers are GPIOB
  return read_byte (MCP_I2C_ADDR + (mcp_num >> 1), MCP_GPIO + ((mcp_num & 0x01) ? MCP_PORT_SEL_BIT : 0)) ; // read the gpio inputs
}

uint8_t mcp23017::get_gpio_current (uint8_t mcp_num) 
{
  return mcp_gpio_current [mcp_num] ;
}

uint8_t mcp23017::get_gpio_current_bit (uint8_t mcp_num, uint8_t requested_bit) // mcp_num is not checked - should be 0-7 only
{
  return (mcp_gpio_current [mcp_num] & (B00000001 << requested_bit)) ? 1 : 0 ;
}

uint8_t mcp23017::get_gpio_last (uint8_t mcp_num) 
{
  return mcp_gpio_last [mcp_num] ;
}

void mcp23017::read_gpio_all () 
{
  for (uint8_t mcp_num = 0 ; mcp_num < MCP23017_MAX * 2 ; mcp_num++)
  {
    mcp_gpio_current [mcp_num] = read_gpio (mcp_num) ;
  }
}

void mcp23017::save_gpio_all () 
{
  for (uint8_t mcp_num = 0 ; mcp_num < MCP23017_MAX * 2 ; mcp_num++)
  {
    mcp_gpio_last [mcp_num] = mcp_gpio_current [mcp_num] ;
  }
}


void mcp23017::write_gpio (uint8_t mcp_num, uint8_t value)
{
  // CHANGE - potential bug if MCP23017_MAX is not 1,2,4,8 etc.
  mcp_num &= MCP23017_MAX * 2 - 1 ; // = B0000111 for MCP23017_MAX == 4

  // mcp_num is now 0 to 7 where even numbers are GPIOA and odd numbers are GPIOB
  write_byte (MCP_I2C_ADDR + (mcp_num >> 1), MCP_GPIO + ((mcp_num & 0x01) ? MCP_PORT_SEL_BIT : 0), value) ; // read the gpio inputs
}


uint8_t mcp23017::read_byte (uint8_t i2c_addr, uint8_t i2c_register)
{
  Wire.beginTransmission (i2c_addr);    
  Wire.write (i2c_register); 
  Wire.endTransmission (); 
  Wire.requestFrom( (int) i2c_addr, 1 ); 
  return Wire.read () ;  
}


void mcp23017::write_byte (uint8_t i2c_addr, uint8_t i2c_register, uint8_t i2c_data)
{
  Wire.beginTransmission (i2c_addr);    
  Wire.write (i2c_register); 
  Wire.write ( i2c_data );
  Wire.endTransmission (); 
}

mcp23017 Mcp23017 ; 
