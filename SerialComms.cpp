/* 
  SerialComms.h    SerialComms library
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


#include "SerialComms.h"

#if COMPILE_DS1307RTC
  #include <DS1307RTC.h>
#endif

serialComms::serialComms ()
{
}


void serialComms::begin ()
{
  rx_state = RX_WAIT ;
  rx_get_number = false ;
  rx_recipient_id = -1 ;  // message intended for this recipient only (or all devices if id=0)

  rx_input_head = 0 ;
  rx_input_tail = 0 ;
  
  tx_response_ptr = 0 ;

  tx_notify_head = 0 ;
  tx_notify_tail = 0 ;
  tx_notify_attempt = 0 ;
  
  rx_input_overrun_ctr = 0 ;
  tx_notify_overrun_ctr = 0 ;

  rx_millis_ready = millis () ;
}


size_t serialComms::write (uint8_t character)
{
  tx_response [tx_response_ptr ++] = character ;
  return 1 ;
}


void serialComms::printBuffDeviceId ()
{
  tx_response_ptr = 0 ; // reset the pointer 
  write ('@') ;
  if (device_id < 10) write ('0') ;
  print (device_id) ;
}


void serialComms::printBuffChecksum ()
{
  write ('c') ;  // checksum includes the 'c' 
  uint8_t checksum = oneWire.crc8 (tx_response, tx_response_ptr) ;
  /* - adds 30 bytes, maybe faster but not needed very often
  uint8_t digit ;
  if (checksum > 100)
  {
    digit = checksum / 100 ;
    print (digit) ;
    checksum -= digit * 100 ;
  } else
  {
    write ('0') ;
  }
  if (checksum > 10)
  {
    digit = checksum / 10 ;
    print (digit) ;
    checksum -= digit * 10 ;
  } else
  {
    write ('0') ;
  }  
  print (checksum) ;
  */
  // uint8_t d1 = checksum / 100 ;       // e.g. 255 / 100 = 2
  print (checksum / 100) ;
  //  uint8_t d2 = checksum - d1 * 100 ;  // e.g. 255 - 2 * 100 = 55
  checksum %= 100 ;                    // e.g. 255 % 100 = 55
  print (checksum / 10) ;              // 55 / 10 = 5
  println (checksum % 10) ;            // 55 % 10 = 5
  /*
  d1 = d2 % 10 ;
  print (d2 / 10) ;
  println (d1) ;
  */
}


void serialComms::flushBuff ()
{
  delayMicroseconds(100) ;  // delay 100us before setting RS485 to transmit mode
  digitalWrite (TXE_PIN, RS485_TX_ENABLE) ; // allow this device to transmit on the RS485 lines
  
  Serial.write (tx_response, tx_response_ptr) ;
  Serial.flush () ;         // make sure all the characters have now been transmitted before...
  delayMicroseconds(100) ;  // delay 100us before setting RS485 back to receive mode (to be sure last char has been sent @ 86uS/char)
  digitalWrite (TXE_PIN, RS485_RX_ENABLE) ; // allow this device to receive on the RS485 lines 
  tx_response_ptr = 0 ;     // reset the pointer 
}


bool serialComms::isRxWaitState ()
{
  return (rx_state == RX_WAIT) ;
}

void serialComms::set_notify_message (uint8_t tx_cmd_port, uint8_t tx_value) 
{
  // puts the notify message into a circular buffer only if there is space
  if ((tx_notify_head == tx_notify_tail) || (tx_notify_tail - tx_notify_head >= 3))  // must always be 1 spare because tail must not catch up with head
  {
    tx_notify [tx_notify_head++] = tx_cmd_port ;
    // if (tx_notify_head >= TX_NOTIFY_MAX) tx_notify_head = 0 ; // not needed if TX_NOTIFY_MAX is even number
    tx_notify [tx_notify_head++] = tx_value ;
    // if (tx_notify_head >= TX_NOTIFY_MAX) tx_notify_head = 0 ;
    tx_notify_head &= (TX_NOTIFY_MAX - 1) ;   // circular buffer - keep only the lower bits
  } else
  {
    tx_notify_overrun_ctr ++ ;   // unable to put a notify message in the buffer
  }
}


void serialComms::check_notify_message ()
{
  // check if there is a message to send, and only send it if no characters have been received for a minimum of RxMillisDelay since last one
  if ((tx_notify_head != tx_notify_tail) && (rx_state == RX_WAIT) && (millis () - rx_millis_ready > EEPROM.read (eeMap::RxMillisDelay) ))
  {
    if (!tx_notify_attempt)
    {
      tx_notify_attempt = TX_NOTIFY_ATTEMPTS_MAX + 1 ;
    } 
    if (tx_notify_attempt--)
    {
      if (tx_notify_attempt)
      {      
        printBuffDeviceId () ;           // restart buffer and put the @xx device_id
        uint8_t tx_cmd = tx_notify [tx_notify_tail] & pcode::PcodeCmdMask ;
        uint8_t tx_port = tx_notify [tx_notify_tail] & pcode::PcodePortMask ;
        switch (tx_cmd)
        {
        case pcode::PcodeCmdKey :
          write ('k') ;                  // 'k' = keypad pressed
          print (tx_port) ;              // tx_port = the key (0-15)
          break ;
        case pcode::PcodeCmdCond :
          write ('P') ;                  // 'P' = 'PIR' ie input GPIO
          print (tx_port) ;              // tx_port = the result (1 = off, 2 = on)
          break ;
        case pcode::PcodeCmdAlarm :
          write ('A') ;                  // 'A' = 'Alarm'
          print (tx_port) ;              // tx_port = the result (1 = off, 2 = on)
          break ;
        case pcode::PcodeCmdTemp :
          write ('T') ;                  // 'T' = temperature 
          print (tx_port) ;              // tx_port = the temperature sensor (0-7)
          write (':') ;
          {
            int value ;
            uint8_t fraction ;
            TempHumidity.readTemp (tx_port, tempHumidity::TEMP_VAL, value, fraction ) ;
            print (value) ;
            write ('.') ;
            // BUG - 0-9 will appear as x.0 to x.9 not x.00 to x.09
            print (fraction) ;           // current temperature in celsius (2 decimal places )      
          }
          break ;
        case pcode::PcodeCmdAnalog :
          write ('a') ;                  // 'a' = analog input
          print (tx_port) ;              // tx_port = the analog input port (0-4 or 6-7)
          write (':') ;
          print (AnalogInputs.analog_values [tx_port]) ; // current analog reading
          break ;
        }
        if (tx_notify [tx_notify_tail] == pcode::PcodeCmdPowerUp)
        {
          write ('U') ;                  // 'U' = Power Up
        }
        if (tx_notify [tx_notify_tail + 1])
        {
          write (':') ;
          print (tx_notify [tx_notify_tail + 1] - 1) ;
        }
        printBuffChecksum () ;          // put the checksum at the end of the buffer
        flushBuff () ;                  // make sure the whole message is sent out
        rx_millis_ready = millis () ;
      } else
      {
        tx_notify_tail += 2 ;           // clear this notify message from the buffer
        tx_notify_tail &= TX_NOTIFY_MAX - 1 ;   // circular buffer - keep only the lower bits
        tx_no_acknowledge_ctr ++ ;      // no acknowledge received for this message
      }
    }  
  }    
}


void serialComms::getSerialInput ()
{
  while (Serial.available())  
  {
    rx_input [rx_input_head ++] = Serial.read () ; // this works provided rx_input defined as: uint8_t rx_input [256] ;
    if (rx_input_head == rx_input_tail) 
    {
      rx_input_tail ++ ; // just throw away one character from end (and lose the current input command)
      rx_input_overrun_ctr ++ ;
    }
  }
}


void serialComms::printTime (TimeElements &time_elements)
{
  print (time_elements.Year + 1970) ;
  write ('/') ;  
  print (time_elements.Month) ;
  write ('/') ;  
  print (time_elements.Day) ;
  write (' ') ;  
  print (time_elements.Hour) ;
  write (':') ;  
  print (time_elements.Minute) ;
  write (':') ;  
  print (time_elements.Second) ;
}


void serialComms::checkCharAvailable ()
{
  if (rx_input_tail != rx_input_head)
  {
    // read the incoming byte:
    char rx_byte = rx_input [rx_input_tail ++] ;
    rx_millis_ready = millis () ;
    
    // TEST only - loopback for serial input
    /*
    if (rx_byte != 10 && rx_byte != 13)
    {
      Serial.write (rx_byte) ;
    } else
    {
      Serial.write (' ') ;
    }  
    */
    // Serial.write (rx_byte) ;    
    
    if (rx_byte == '@')
    {
      rx_state = RX_RECIP ;  // receiving the recipient id for the message (2 digits)
      current_value_ptr = 0 ;
      // current_value [current_value_ptr] = 0 ; // just to be sure - probably not needed
      rx_get_number = true ;
      rx_new_number = 0 ;
      tx_response_ptr = 0 ;
      rx_num_digits = 2 ;  // the recipient id is always 2 digits (max 99 units on one RS485 line)
      write (rx_byte) ;
    } 
    else if (rx_state == RX_CMD)
    {
      current_cmd = rx_byte ;
      rx_state = RX_CMD_VALUE ;  // receive any value associated with the command
      rx_get_number = true ;
      rx_new_number = 0 ;
      rx_num_digits = 0 ;    // not set - any number of digits allowed, terminated by ':', ',' or 'c'
      rx_num_colons = 0 ;    // count the number of colons
      write (rx_byte) ;
    }
    else if (rx_state == RX_CMD_VALUE && (rx_byte == ':' || rx_byte == ','))
    {
      current_value [ current_value_ptr++ ]  = rx_new_number ; // 
      if (current_value_ptr > CURRENT_VALUE_PTR_MAX) 
      {
        // packet is too long - ignore entire packet (it won't have valid checksum anyway)
        rx_state = RX_INVALID ;
      } else
      {
        current_value [ current_value_ptr ]  = 0 ; // put a 0 into the next number
        rx_get_number = true ;
        rx_new_number = 0 ;
        rx_num_digits = 0 ;     // not set - any number of digits allowed, terminated by ':', ',' or 'c'
        write (rx_byte) ;       // put the ':' or ',' into the buffer
        if (rx_byte == ':') rx_num_colons ++ ;  // increment the number of colons in this command
      }
    } 
    else if (rx_state == RX_CMD_VALUE && rx_byte == 'c') 
    {
      current_value [ current_value_ptr++ ] = rx_new_number ; // no checking for current_value_ptr > 2
      rx_state = RX_CHKSUM ;  // receiving the checksum at the end of the message (3 digits)
      rx_get_number = true ;
      rx_new_number = 0 ;
      rx_num_digits = 3 ;   // the checksum is always 3 digits with leading zeros
      write (rx_byte) ;     // don't put the checksum into the buffer, but put the 'c' into the buffer for calculating the checksum
    } 
    else if (rx_get_number && rx_byte >= '0' && rx_byte <= '9')
    {
      // receiving a number 
      rx_new_number = rx_new_number * 10 + rx_byte - '0' ;
      if (rx_state != RX_CHKSUM) // don't put the checksum into the buffer
      {
        write (rx_byte) ; 
      }
      
      if (rx_num_digits) 
      {
        // expecting a fixed number of digits (@xx or cxxx)
        rx_num_digits -- ;
        if (rx_num_digits == 0)
        {
          // correct number of digits received for @xx (start of message) or cxxx (end of message)
          if (rx_state == RX_RECIP)
          {
            if  (rx_new_number == device_id || rx_new_number == BROADCAST_ID)
            {
              rx_recipient_id = rx_new_number ;
              rx_state = RX_CMD ; // next character is the command
              rx_get_number = false ;
              current_cmd = 0 ;
              current_value [0] = 0 ;
              current_value [1] = 0 ;   // probably now not needed (':' or ',' puts zero into the next current_value
              current_value [2] = 0 ;   // probably now not needed              
            } else
            {
              rx_state = RX_INVALID ;    // treat as invalid - just get the cr/lf to complete the command
            }
          }
          else if (rx_state == RX_CHKSUM)
          {
            // now verify the checksum
            checksum = oneWire.crc8 (tx_response, tx_response_ptr) ;
            tx_response_ptr-- ;     // remove the 'c' command
            if (rx_new_number == checksum)
            {
              rx_state = RX_CRLF ;  // all good - now get the CR LF before processing everything
            }
            else
            {
              rx_state = RX_INVALID ;    // treat as invalid - just get the cr/lf to complete the command
            }
          }
        }
      }
    }
    else if (rx_state == RX_CRLF)
    {
      if (rx_byte == 0x0A)
      {
        /*
        ***** reserved for communications protocol
        @ xx                  // device to read the command from computer (or =00 for all devices to read) or device responding to command
        c xxx                 // checksum for everything from @ to c
        CR                    // end of message
        LF                    // end of message
        
        *****  list of commands:
        t hh,mm,ss            // set the time
        d yy,MM,dd            // set the date (and update EEPROM)
        a                     // read or write analog values including analogWrite pcm
        A                     // read, write, set or clear Alarms
        i                     // read or write interrupt counters
        
        I                     // read or write digital input or output
        m                     // read the available free memory
        v                     // read the software version number
        q                     // read the loop time parameters
        L                     // set the LEDs
        e                     // read or write EEPROM values
        b                     // set the beeper
        T                     // read temperatures or read the DS18B20 address codes or clear the min/max values or read Arduino Temp/Volts
        R35                   // reset the arduino
        w xx:ABCD             // write ABCD to display position xx (0-83)
        N  1 or 0             // switch display to/from interactive mode
        K                     // acknowledge previous notify message or command
        
        ***** list of notify messages
        P                     // port changed pcode condition met
        k                     // key pressed on keypad
        A                     // alarm ringing
        T                     // temperature condition met (or humidity)
        a                     // analog condition met
        U                     // power up
        */
        
        switch (current_cmd)
        {
        case 'K' :
          // acknowledge of previously sent notify message
          if (tx_notify_head != tx_notify_tail && rx_recipient_id == device_id)
          {
            tx_notify_tail += 2 ;  
            if (tx_notify_tail >= TX_NOTIFY_MAX) tx_notify_tail = 0 ;
            tx_notify_attempt = 0 ;
          }
          break ;
        case 't' :
        case 'd' :
          if (current_cmd == 't')
          {
            // change the time - assume for the current date
            if (rx_num_colons)
            {
              setTime (current_value [0], current_value [1], current_value [2], day (), month (), year ()) ;
            } else
            {
              TimeElements time_elements ;
              breakTime( now (), time_elements) ; // fill in all the values
              printTime (time_elements) ;
              rx_state = RX_REPLY ;
            }
          }
          else 
          {
            // new date set - keep the time the same
            setTime (hour (), minute (), second (), current_value [2], current_value [1], 2000 + current_value [0]) ; 
            #if COMPILE_DS1307RTC
              RTC.set (now ()) ;
            #else 
              EEPROM.updateLong (eeMap::Time, now ()) ; // update the date/time in eeprom 
            #endif
          }
          #if COMPILE_DS1307RTC
            time_last_updated = now () ;
          #endif
          Alarms.newTimeSet () ; // this will restart only alarms that were set to go off before the new time
          break ;
        case 'A' :
          if (rx_num_colons == 0)
          {
            // print the alarm time 
            write (':') ;  
            if (Alarms.alarmTime [current_value [0]])
            {
              TimeElements alarm_time_elements ;
              breakTime( Alarms.alarmTime [current_value [0]], alarm_time_elements) ; // fill in all the values
              printTime (alarm_time_elements) ;
              /*
              print (alarm_time_elements.Day) ;
              write ('/') ;  
              print (alarm_time_elements.Month) ;
              write (' ') ;  
              print (alarm_time_elements.Hour) ;
              write (':') ;  
              print (alarm_time_elements.Minute) ;
              write (':') ;  
              print (alarm_time_elements.Second) ;
              */
            } else
            {
              write ('0') ;  // alarm not set
            }
            rx_state = RX_REPLY ;
          } else
          {
            if (!current_value [1])
            {
              Alarms.alarmTime [current_value [0]] = 0 ; // turn the alarm off
            } else
            {
              Alarms.setAlarm (current_value [0]) ;
            }
          }
          break ;
        case 'a' :
          if (rx_num_colons == 0)
          {
            if (current_value [0] < ANALOG_INPUTS_MAX)
            {
              // read any analogue input 0-7
              // NB this does not update the previous values read at the previous sample time
              write (':') ;
              if (current_value [0] < ARDUINO_VCC || current_value [0] > ARDUINO_TEMP)
              {
                print (AnalogInputs.analog_values [current_value [0]]) ; 
                write (',') ;
                print (AnalogInputs.analog_values_min [current_value [0]]) ; 
                write (',') ;
                print (AnalogInputs.analog_values_max [current_value [0]]) ; 
              } else
              {
                int value ;
                uint8_t fraction ;
                for (byte val_min_max = analogInputs::ARDUINO_VAL; val_min_max <= analogInputs::ARDUINO_MAX; val_min_max++)
                {
                  if (current_value [0] == ARDUINO_VCC)
                  {
                    AnalogInputs.readArduinoVcc (static_cast <analogInputs::ArduinoValMinMax> (val_min_max), value, fraction) ;
                  } else
                  {
                    AnalogInputs.readArduinoTempC (static_cast <analogInputs::ArduinoValMinMax> (val_min_max), value, fraction) ;
                  }                
                  print (value) ;
                  write ('.') ;
                  print (fraction) ;
                  if (val_min_max != analogInputs::ARDUINO_MAX) write (',') ;
                }
              }                
            } else
            {
              write (':') ;
              for (uint8_t i=0; i < ANALOG_INPUTS_MAX ; i++)   // i declared outside switch command
              {
                if (i > 0) write (',') ;
                print (AnalogInputs.analog_values [i]) ; // previously read values 
              }
            }
            rx_state = RX_REPLY ;
          } else
          {
            if (current_value [0])
            {
              analogWrite (current_value [0], current_value [1]) ; // for PWM on pins 3, 5, 6, 9, 10, and 11
            } else
            {
              // "a0:"
              AnalogInputs.clearMinMax () ;
            }
          }
          break ;
        case 'i' :
          if (rx_num_colons == 0)
          {
            // reads interrupt counter 0 or 1
            write (':') ;
            print ( current_value [0] ? interrupt_count1 : interrupt_count0 ) ;
            rx_state = RX_REPLY ;
          }  else
          {
            // sets the value of interrupt counter 0 or 1 (any value from 0 to 255)
            if (!current_value [0])
            {
              interrupt_count0 = (uint8_t) current_value [1] ;
            } else
            {
              interrupt_count1 = (uint8_t) current_value [1] ;
            }                    
          }                  
          break ;
        case 'I' :
          // I33      read all 8 bits from MCP GPIOB
          // I33,2    read only bit 2 from MCP GPIOB
          // I33:0    write 0 to all 8 bits of MCP GPIOB
          // I33:2:0  write 0 to bit 2 of MCP GPIOB
          if (rx_num_colons == 0)
          {
            // this is a read command
            write (':') ;
            // CHANGE should be same as p-code 
            if (current_value [0] <= 23)
            {
              // 'I0' to 'I13': read the bit from the arduino digital port; 'I14' to 'I21': read the bit from the arduino A0-A7 port
              print (digitalRead (current_value [0])) ; 
            } else
            if (current_value [0] == 24)
            {
              // 'I24'; read D0-D7
              print (PORTD) ; 
            } else
            if (current_value [0] == 25)
            {
              // 'I25': read D8-D13
              print (PORTB) ; 
            } else
            if (current_value [0] == 26)
            {
              // 'I26': read A0-A7 as digital
              print (PORTC) ; 
            } else
            {
              // 'I32' to 'I39': read mcp23017 
              uint8_t value = Mcp23017.read_gpio (current_value [0]) ; 
              if (current_value_ptr)
              {  
                // e.g. 'I33,0' read only a single bit (bit 0) from the MCP23017
                print ( (bool) (value & (B00000001 << current_value [1])) ) ; // just the single bit from the MCP port
              } else
              {
                // 'I33' is read
                print ( value ) ; // all 8 bits from the port
              }
            }
            rx_state = RX_REPLY ;
          } else
          {
            // this is a write command 
            if (current_value [0] <= 17)
            {
              // 0-13 write the bit to the arduino digital port D0-D13 ; 14-21 write the bit to the arduino A0-A3 port (as digital)
              digitalWrite (current_value [0], current_value [1]) ;
            } else
            /*
            if (current_value [0] == 24)
            {
              // 24 write 8 bits to D0-D7
              PORTD = current_value [1] ; 
            } else
            if (current_value [0] == 25)
            {
              // 24 write 6 bits to  D8-D13
              PORTB = current_value [1] ; 
            } else
            if (current_value [0] == 26)
            {
              // 26 write 8 bits to A0-A7 as digital
              PORTC = current_value [1] ; 
            } else
            */
            {
              // 'I32' to 'I39': read the current mcp23017 gpio value
              if (rx_num_colons == 1)
              {  
                // e.g. I33:0 write all 8 bits to the GPIO on MCP23017
                Mcp23017.write_gpio (current_value [0], current_value [1] ) ;  
              } else
              {
                // e.g. 'I33:0:0' is write only one bit to the MCP23017
                uint8_t value = Mcp23017.read_gpio (current_value [0]) ; // read_gpio 
                if (current_value [2])
                {
                  value |= B00000001 << current_value [1] ; // set only the requested bit high
                } else
                {
                  value &= ~ (B00000001 << current_value [1]) ; // set only the requested bit low
                }
                Mcp23017.write_gpio (current_value [0], value) ;  
              }
            } 
          }
          break ;
        case 'm' :
          // memory
          print (freeMemory()) ;
          rx_state = RX_REPLY ;
          break ;
        case 'v' :
          // version
          print (version_str) ;
          rx_state = RX_REPLY ;
          break ;
        case 'q' :
          // loop time
          print (loop_time) ;
          write (',') ;
          print (loop_count_fast) ;
          write (',') ;
          print (loop_count_total) ;
          write (',') ;
          
          for ( uint8_t i = LOOP_DELAY_MAX ; i ;)
          {
            print (loop_delays [--i]) ;
            loop_delays [i] = 0 ;  // reset all the loop delays
            write (',') ;
          }

          print (rx_input_overrun_ctr) ;
          write (',') ;
          print (tx_notify_overrun_ctr) ;
          write (',') ;
          print (tx_no_acknowledge_ctr) ;

          rx_input_overrun_ctr = 0 ;
          tx_notify_overrun_ctr = 0 ;
          tx_no_acknowledge_ctr = 0 ;
          rx_state = RX_REPLY ;
          break ;

        #if COMPILE_LEDSTRIP 
        case 'L' :
          // Lxx,yy,rrr,ggg,bbb or Lxx,yy,rgb
          // Write to LEDs
          if (rx_num_colons == 0)
          {
            if (current_value_ptr > 3)
            {
              LedStrip.setLedColour ( current_value [2], current_value [3], current_value [4] ) ;
            } else
            {
              LedStrip.setLedColourFromRGB ( current_value [2] ) ;
            }
            fill_solid ( &(LedStrip.leds[current_value [0]]), current_value [1], LedStrip.getLedColour () ) ;
            LedStrip.setLedMode (ledStrip::LedModeNormal) ;
          } else
          {
            // set led mode Lmm: or Lmm:rrr,ggg,bbb
            // L1:r,g,b = fast flash
            LedStrip.setLedMode (current_value [0]) ;
            if (current_value_ptr > 2)
            {
              LedStrip.setLedColour ( current_value [1], current_value [2], current_value [3]) ;
            } else
            {
              LedStrip.setLedColourFromRGB ( current_value [1] ) ;
            }
            LedStrip.setLedPosn (0) ;
            fill_solid ( &(LedStrip.leds[0]), LEDS_MAX, (current_value_ptr) ? LedStrip.getLedColour () : CRGB::Black ) ; // start with all leds off
          }
          LedStrip.show(); // Sends the values to the LEDs
          break ;
        #endif
        case 'e' :
          {
            /*
            Serial.print ("[0]=") ;
            Serial.print (current_value [0], DEC) ;
            Serial.print (",[1]=") ;
            Serial.print (current_value [1], DEC) ;
            Serial.print (",[2]=") ;
            Serial.print (current_value [2], DEC) ;
            */
            uint8_t current_value_unsigned = current_value [1] ;  // convert char (signed 8-bit) to unsigned 
            unsigned int eeprom_position =  current_value_unsigned ;
            /*
            Serial.print (", eeprom_position=") ;
            Serial.print (eeprom_position, DEC) ;
            */
            current_value_unsigned = current_value [0] ;  // convert char (signed 8-bit) to unsigned 
            eeprom_position = (eeprom_position << 8) + current_value_unsigned ;
            /*
            Serial.print (", eeprom_position=") ;
            Serial.print (eeprom_position, DEC) ;
            */
            if (rx_num_colons == 0)
            {
              // EEPROM read e32,1,8 read 8 bytes from EEPROM 288,289...
              write (':') ;
              for ( uint8_t i = 0; i < current_value [2]; i++) 
              {
                if (i != 0) write (',') ;
                print (EEPROM.read (eeprom_position++)) ;
              }
              rx_state = RX_REPLY ;
            } else
            {
              // EEPROM write e32:1:1,2,3,4 write 1,2,3,4 to EEPROM 288,289...
              if (current_value_ptr)
              {
                /*
                // test of EEPROM.updateLong
                if (eeprom_position == 64)
                {
                  unsigned long new_long = 0 ;
                  for ( uint8_t i = 2 ; ((i < current_value_ptr) && (i < 6)); i++ ) 
                  {
                    new_long = (new_long << 8) + (uint8_t) current_value [i] ;
                  }
                  EEPROM.updateLong (eeprom_position, new_long) ;
                } else
                {
                  */
                  for ( uint8_t i = 2 ; i < current_value_ptr ; i++) 
                  {
                    EEPROM.updateByte (eeprom_position++, current_value [i]) ; // warning 3.3ms per byte updated 
                  }
                // }
              }
            }
            /*
            Serial.println () ;
            */
          }
          break ;
        case 'b' :
          // beep for xx/2 short or (xx-50)/2 long beeps
          Beeper.setBeep (current_value [0]) ;
          break ;
        case 'T' :
          if (rx_num_colons == 0)
          {
            // show the temperature value/min/max of a DS18B20 device (at the last sample point)
            int value ;
            uint8_t fraction ;
            write (':') ;
            TempHumidity.readTemp (current_value [0], tempHumidity::TEMP_VAL, value, fraction) ;
            print (value) ;
            write ('.') ;
            print (fraction) ;
            write (',') ;
            TempHumidity.readTemp (current_value [0], tempHumidity::TEMP_MIN, value, fraction) ;
            print (value) ;
            write ('.') ;
            print (fraction) ;
            write (',') ;
            TempHumidity.readTemp (current_value [0], tempHumidity::TEMP_MAX, value, fraction) ;
            print (value) ;
            write ('.') ;
            print (fraction) ;
            rx_state = RX_REPLY ;
          } else
          {
            switch  (current_value [1])
            {
            case 2 :
              // 'Tx:0' show the actual One-Wire address of DS18B20 device x (or leave blank if not available)
              for ( uint8_t i = 0 ; i < 8 ; i++ ) // i declared outside switch
              {
                if (i) 
                {
                  write (',') ;
                } else
                {
                  write (':') ;
                }
                print (TempHumidity.ds18b20_address [current_value [0]][i]) ;
                rx_state = RX_REPLY ;
              }
              break ;
            case 1 :
              // 'Tx:1' will either requestTemperatures or read temps into temp_readings [] - only needed if EeMap TempSampleRate == 0
              TempHumidity.setTempHumidityStart () ;
              break ;
            default :
              // 'Tx:0' will reset the min and max values for all the temps
              TempHumidity.clearMinMax () ;
            }
          }
          break ;
        case 'R' :
          // reset the device
          if (current_value [0] == 35)
          {
            software_reset () ; // 35 is just for extra security
          }
          break ;  
        #if COMPILE_NOKIA_5110
        case 'w' :
          {
            Display.gotoCharPosition (current_value [0]) ;
            current_value [current_value_ptr] = 0 ; // put the End Of String marker at the end of the string
            Display.print ( &(current_value [1]) ) ; // prints all characters in the array current_value starting from [1] 
          }
          break ;
        case 'N' :
          // switch interactive mode on or off
          Display.setDisplayMode (current_value [0]) ;
          Display.clear () ;
          break ;
        #endif

        }
        if ((rx_state == RX_CRLF)) rx_state = RX_ACK ;
      }
      else if (rx_byte != 0x0D)  // expecting a CR character
      {
        rx_state = RX_INVALID ;  
      }

      if (rx_state == RX_REPLY)
      {
        printBuffChecksum () ; 
        flushBuff () ;  // send the response only once (the master can always resend the command if it receives no response or garbled response)
        // checksum has been received, now return to the waiting state 
        rx_state = RX_WAIT ; // return to the waiting state
      } else
      if (rx_state == RX_ACK && rx_recipient_id == device_id)
      {
        tx_response_ptr = 3 ;   // "@xx" for the device id
        write ('K') ;
        printBuffChecksum () ; 
        flushBuff () ;  // send the response only once (the master can always resend the command if it receives no response or garbled response)
        // checksum has been received, now return to the waiting state 
        rx_state = RX_WAIT ; // return to the waiting state
      }
      rx_millis_ready = millis () ; // can't send any notify messages to the master while a command is being received on RS485 lines
    }
    else
    {
      rx_state = RX_INVALID ; // invalid character received or command is not for this device
    }
  } else
  {
    // no character has been received in this loop cycle - check that the serial is not stuck
    if ((rx_state != RX_WAIT) && (millis () - rx_millis_ready > RX_MILLIS_MAX))
    {
      rx_state = RX_WAIT ;      // waiting too long for an input character to complete the command
    }
  }
}

serialComms SerialComms ; 

