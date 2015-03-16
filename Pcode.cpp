/* 
  Pcode.h          Pcode library
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

#include "Pcode.h"


pcode::pcode ()    // constructor - nothing to do here
{
}

void pcode::begin ()
{
}


uint8_t pcode::check_int_high_low (int pcode_index0, uint8_t sensor_number, int sensor_current_value, uint8_t & all_sensors_previous_checked )
{
  // NOTE: this presumes a maximum of 8 (uint8_t) temp sensors or analog devices

  bool sensor_last_checked_high = all_sensors_previous_checked & (1 << sensor_number) ; 

  unsigned int eeprom_position = pcode_index0 + (int) (sensor_number * 2)  + (sensor_last_checked_high ? 0 : 1) ;
  /*
  bool debug = false;
  // if ((device_id == 4) && (sensor_number == 7)) debug = true ;
  // if ((device_id == 5) && (sensor_number == 0)) debug = true ;
  
  if (debug)
  {
    Serial.print ("analog ") ;
    Serial.print (sensor_number) ;
    Serial.print ('=') ;    
    Serial.print (sensor_current_value) ;
    Serial.print (", last=") ;
    Serial.print (sensor_last_checked_high, BIN) ;
    Serial.print (", pcode0=") ;
    Serial.print (pcode_index0) ;
    Serial.print (", index=") ;
    Serial.print (eeprom_position) ;
  }
  */
  eeprom_position = EEPROM.read (eeprom_position) ;
  
  /*
  if (debug)
  {
    Serial.print (", index=") ;
    Serial.print (eeprom_position1) ;
    // eeprom_position1 = 152 ;
  }
  */
  
  if (eeprom_position)   // only check if a sensor has both 
  {
    // NOTE: 
    eeprom_position += pcode_index0 & 0xFF00 ; // add the eeprom block (256 bytes) to the offset
    /*
    if (debug)
    {
      Serial.print (", index=") ;
      Serial.print (eeprom_position1) ;  
    }
    */
    int sensor_compare ;
    sensor_compare = (int) EEPROM.read (eeprom_position) + ((int) (EEPROM.read (eeprom_position + 1)) << 8) ;
    eeprom_position += 2 ; // it is a signed integer so two bytes
    /*
    if (debug)
    {
      Serial.print (", compare=") ;
      Serial.print (sensor_compare) ;
      Serial.print (", prev=") ;
      Serial.print (all_sensors_previous_checked) ;
    }
    */
    if (sensor_last_checked_high)
    {
      // previously a ">" temp was reached - now check for a "<" temp
      if (sensor_current_value < sensor_compare)
      {
        change_ports (eeprom_position) ;
        all_sensors_previous_checked &= ~(1 << sensor_number) ;  // put the high/low bit to 0 - next time check for greater than the high value
        /*
        if (debug)
        {
          Serial.print (", new=") ;
          Serial.print (all_sensors_previous_checked) ;
          Serial.println () ;
        }
        */
        return 1 ; // return 1 if the value was lower than the bottom limit
      } 
    } else
    {
      // previously a "<" temp was reached - now check for a ">" temp
      if (sensor_current_value >= sensor_compare)
      {
        change_ports (eeprom_position) ;
        all_sensors_previous_checked |= (1 << sensor_number) ;  // put the high/low bit to 1 - next time check for less than the low value
        /*
        if (debug)
        {
          Serial.print (", new=") ;
          Serial.print (all_sensors_previous_checked) ;
          Serial.println () ;
        }
        */
        return 2 ; // return 2 if the value was higher than the upper limit
      } 
    }
    /*
    if (debug)
    {
      Serial.print (", new=") ;
      Serial.print (all_sensors_previous_checked) ;
      Serial.println () ;
    }
    */
  }
  /*
  if (debug)
  {
      Serial.println () ;
  }
  */
  return 0 ;   // return 0 if nothing has changed ;    
}


void pcode::check_condition_bit (uint8_t pcode_condition, uint8_t pcode_input, uint8_t pin_current, uint8_t pin_last, uint8_t &new_pin_state)
{
  // note: new_pin_state is only updated for an input port condition
  /*
  Serial.print ("check_condition_bit ") ;
  Serial.print (pcode_condition) ;
  Serial.print (',') ;
  Serial.print (pcode_input) ;
  Serial.print (',') ;
  Serial.print (pin_current) ;
  Serial.print (',') ;
  Serial.print (pin_last) ;
  */
  pcode_input = B00000001 << pcode_input ;
  if (pcode_condition < 3)
  {
    // pcode_condition == 2 check for bit going from 0 to 1; 
    // pcode_condition == 1 check for bit going from 1 to 0; 
    pcode_condition -- ;
    if ((~pin_last & pcode_input) && (pin_current & pcode_input) && pcode_condition)
    {
      // Serial.println (", return=2,bbb,1,0") ;
      new_pin_state = 2 ; // last = 0, current = 1, condition = 2
      return ; 
    }
    if ((pin_last & pcode_input) && (~pin_current & pcode_input) && !pcode_condition)
    {
      // Serial.println (", return=1,bbb,0,1") ;
      new_pin_state = 2 ; // last = 1, current = 0, condition = 1
      return ;
    }
    // Serial.println (", return=0,bbb,x,x") ;
  } else
  {
    // pcode_condition == 3 or 4 check for bit going from 0 to 1 or 1 to 0; 
    if ((pin_last ^ pin_current) & pcode_input)
    {
      // return the new state of the input pin (2= high, 1 = low)
      if (pcode_condition == 3)
      {
        new_pin_state = (pin_current & pcode_input) ? 2 : 1 ; // return the same state as the input has changed to
        return ;
      } else
      {
        new_pin_state = (pin_current & pcode_input) ? 1 : 2 ; // return the opposite state as the input has changed to
        return ;
      }      
    }
  }
  return ;
}


void pcode::check_condition ( unsigned int &pcode_ptr, uint8_t &new_pin_state)
{
  // check that the condition at pcode_ptr on the input pin at pcode_ptr+1 is met
  uint8_t pcode_condition = EEPROM.read (pcode_ptr++) & ~PCODE_AND_BIT ; // 
  uint8_t pcode_cmd = EEPROM.read (pcode_ptr++) ;
  uint8_t pcode_port = pcode_cmd & PcodePortMask ;   // 0-7 = GPIOA 0-7; 8-15 = GPIOB 0-7
  pcode_cmd &= PcodeCmdMask ;
  /*
  Serial.print (", pcode_condition=") ;
  Serial.print (pcode_condition) ;
  Serial.print (", pcode_cmd=") ;
  Serial.print (pcode_cmd) ;
  Serial.print (", pcode_port=") ;
  Serial.println (pcode_port) ;
  */
  switch (pcode_cmd)
  {
  case PcodeCmdPort :
    // check the condition of digital inputs D2-D13 or A0 to A3 on arduino
    if (pcode_port <= 1) 
    {
      // A2-A3 as digital
      check_condition_bit (pcode_condition, pcode_port + 2, arduino_gpio [2], arduino_gpio_last [2], new_pin_state) ;
    } else if (pcode_port <= 7) 
    {
      // D0-D7
      check_condition_bit (pcode_condition, pcode_port, arduino_gpio [0], arduino_gpio_last [0], new_pin_state) ;
    } else if (pcode_port <= 13) 
    {
      // D8-D13
      check_condition_bit (pcode_condition, pcode_port, arduino_gpio [1], arduino_gpio_last [1], new_pin_state) ;
    }  else
    {
      // A0-A1 as digital
      check_condition_bit (pcode_condition, pcode_port - 14, arduino_gpio [2], arduino_gpio_last [2], new_pin_state) ;
    }    
    break ;
  case PcodeCmdMcp0 :
  case PcodeCmdMcp1 :
  case PcodeCmdMcp2 :
  case PcodeCmdMcp3 :
    uint8_t mcp_val = ((pcode_cmd - PcodeCmdMcp0) >> 3) + (pcode_port && PCODE_MCP_PORT_B_BIT) ? 1 : 0  ;
    /*
    Serial.print (", v=") ;
    Serial.print (mcp_val) ;
     */
    check_condition_bit (pcode_condition, pcode_port & PCODE_MCP_BIT_MASK, Mcp23017.get_gpio_current (mcp_val), Mcp23017.get_gpio_last (mcp_val), new_pin_state) ;
    // break ;
  }
  return ;
}

void pcode::check_all_conditions (unsigned int pcode_ptr, uint8_t condition_num)
{
  uint8_t pcode_val ; 
  uint8_t new_pin_state = 0 ;

  do
  {
    pcode_val = EEPROM.read (pcode_ptr) ; // this is current input condition
    check_condition (pcode_ptr, new_pin_state) ; // check_condition will return with the pcode_ptr ..
                                                  // at the start of the next input condition AND update new_pin_state
    if (!new_pin_state) 
    {
      return ;
    }
  }
  while (pcode_val & PCODE_AND_BIT) ; // continue to the next condition if there are more available

  // pcode_ptr is now at the output instructions for this input condition
  
  uint8_t gpio_response = change_ports (pcode_ptr) ;
  
  // send an notify message to the master
  if (IS_NOTIFY_COND_SET)      // in EEpromMap, read the EEPROM to see whether to send notify messages on a Condition met
  {
    SerialComms.set_notify_message (PcodeCmdCond + condition_num, gpio_response) ;
  }
}


void pcode::check_all_gpio_conditions ()
{
  unsigned int pcode_ptr ;
  for (uint8_t i = 0; i < EE_PCODE_INPUTS_MAX; i++)
  {
    pcode_ptr = EEPROM.read (eeMap::PcodeInputsIndex + i) ;
    if (pcode_ptr) 
    {
      pcode_ptr = (eeMap::PcodeInputsIndex & 0xFF00) + pcode_ptr ;
      check_all_conditions (pcode_ptr, i) ;
    }
  }
}


uint8_t pcode::change_port (uint8_t new_val, unsigned int & pcode_ptr)
{
  /*
  Serial.print ("cmd=") ;
  Serial.print (new_val) ;
  Serial.print (", pcode_ptr=") ;
  Serial.print (pcode_ptr) ;
  */
  uint8_t cmd_port = EEPROM.read (pcode_ptr ++) ;
  uint8_t new_port = cmd_port & PcodePortMask ;  // only bits 0-3
  new_val &= ~PCODE_AND_BIT ; 
  
  switch (cmd_port & PcodeCmdMask)
  {
  case PcodeCmdPort :
    // request to directly manipulate D2-D13 or A0 to A3 on arduino
    if (new_port < 2) new_port += 16 ;  // 0, 1 => A2, A3 (= D16, D17) 
    if (new_val >= PCODE_CHANGE)
    {
      // change the state of the digital output - first read the current value
      new_val = digitalRead (new_port) ? PCODE_LOW_LT : PCODE_HIGH_GT ;
    }
    digitalWrite (new_port, new_val - 1) ;
    /*
    Serial.print (", =>") ;
    Serial.print (new_val - 1) ;
    */
    break ;
  case PcodeCmdMcp0 :
  case PcodeCmdMcp1 :
  case PcodeCmdMcp2 :
  case PcodeCmdMcp3 :
    {
      uint8_t mcp_num = (((cmd_port & PcodeCmdMask) - PcodeCmdMcp0) >> 3 ) + ((new_port & PCODE_MCP_PORT_B_BIT) ? 1 : 0 ) ;
      uint8_t read_byte = Mcp23017.read_gpio (mcp_num) ;
      uint8_t bit_mask = 1 << (cmd_port & PCODE_MCP_BIT_MASK) ;
      if (new_val >= PCODE_CHANGE)
      {
        // change the state of the digital output - first read the current value
        new_val = (read_byte & bit_mask) ? PCODE_LOW_LT : PCODE_HIGH_GT ;
      }
      if (new_val == PCODE_HIGH_GT)
      {
        read_byte |= bit_mask ; // set only the requested bit high
      } else
      {
        read_byte &= ~bit_mask ; // set only the requested bit low
      }

      Mcp23017.write_gpio (mcp_num, read_byte ) ;
      /*
      Serial.print (", MCP=>") ;  
      Serial.print (new_val - 1) ;
      */
    }
    break ;
  
  #if COMPILE_LEDSTRIP 
  case PcodeCmdLed :
    LedStrip.setLedModeFromPcode (new_val, new_port) ;
    break ;
  #endif
  
  case PcodeCmdBeep :
    Beeper.setBeepPcode (new_port) ;
    break ;

  case PcodeCmdAlarm :
    if (new_val == PCODE_HIGH_GT)
    {
      Alarms.setAlarm (new_port) ;
    } else
    {
      Alarms.alarmTime [new_port] = 0 ; // turn the alarm off
    }
    break ;
      
  case PcodeCmdMisc :
    switch (new_port)
    {
    #if COMPILE_NOKIA_5110
    case CmdMiscInteractive  :
      Display.setDisplayMode (display::DisplayInteractive) ;
      Display.clear () ;
      break ;
    case CmdMiscDisplay :
      if (!Display.isDisplayInteractive ())
      {
        Display.gotoCharPosition (EEPROM.read (pcode_ptr ++)) ;
        char ch = EEPROM.read (pcode_ptr ++) ;
        while (ch)
        {
          Display.print (ch) ;
          ch = EEPROM.read (pcode_ptr ++) ;
        }
      }
      break ;
    #endif  
    case CmdMiscResetInt0 :
      interrupt_count0 = 0 ;    
      break ;
    case CmdMiscResetInt1 :
      interrupt_count1 = 0 ;    
      break ;
    case CmdMiscResetTemps :
      TempHumidity.clearMinMax () ;
      break ;
    case CmdMiscResetAnalog :
      AnalogInputs.clearMinMax () ;
      break ;
    case CmdMiscReadTemps :
      TempHumidity.setTempHumidityStart () ;
      break ;
    case CmdMiscReadAnalog :
      AnalogInputs.setAnalogReadStart () ;
      break ;
    } 
    break ;

  }
  // Serial.println () ;
  return new_val ;  // new_val = 2 if an output has been set high; new_val = 1 if an output has been set low;
                    // new_val = 0 if nothing has changed ;
}

uint8_t pcode::change_ports (unsigned int pcode_ptr)
{
  // CHANGE: this is fine for pcodes that are 2 bytes only.
  
  // changes all the outputs according to pcode at pcode_ptr
  uint8_t pcode_val = EEPROM.read (pcode_ptr++) ; // this is now the output response
  uint8_t new_val = change_port (pcode_val, pcode_ptr) ;

  while (pcode_val & PCODE_AND_BIT)
  {
    // pcode_ptr += 2 ; // jump to the next response
    pcode_val = EEPROM.read (pcode_ptr++) ;
    if ((pcode_val & ~PCODE_AND_BIT) == PCODE_CHANGE) 
    {
      change_port (new_val, pcode_ptr) ;  // if the second responses also want to change, put them to the same output as the first response
    } else
    {
      new_val = change_port (pcode_val, pcode_ptr) ;
    }
  }
  return new_val ;  // new_val = 2 if an output has been set high; 
                    // new_val = 1 if an output has been set low;
                    // new_val = 0 if nothing has changed ;
}

pcode Pcode ; 

