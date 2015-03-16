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

#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H

#include <Arduino.h>
#include <inttypes.h>
#include <Wire.h>

#include "Limits.h"
#include "EEpromMap.h"            // this defines the use of the EEPROM
#include "EEPROMAnything.h"

#if COMPILE_LEDSTRIP
  #include "LedStrip.h"
#endif

#include "MCP23017.h"              // code for dealing with the MCP23017 16-bit GPIO extenders
#include "Beeper.h"    
#include "Keypad.h"
#include "AnalogInputs.h"
#include "Pcode.h"
#include "Alarms.h"

#if COMPILE_NOKIA_5110
  #include <SPI.h>
  #include <PCD8544_SPI.h>
  #include "Display.h"
#endif

extern uint8_t device_id ;
extern OneWire oneWire ;

extern unsigned long loop_time ;
extern unsigned long loop_max ;
extern unsigned long loop_count_fast ;
extern unsigned long loop_count_total ;
extern unsigned long time_last_updated ;

extern int freeMemory () ;
extern void software_reset() ;

extern char version_str [] ;

extern CEepromAnything EEPROM ;

class analogInputs ;

extern analogInputs AnalogInputs ; 

#if COMPILE_DS1307RTC
  class DS1307RTC ;
  extern DS1307RTC RTC ;
#endif

// define RS485 receive states
#define BROADCAST_ID   0          // might need to change this

#define RX_MILLIS_MAX 2           // valid incoming characters must be received within 2ms of previous part of a command

#define TX_NOTIFY_MAX 16          // maximum size of notify message buffer (typically 2 bytes per message = 7 pending messages)
                                  // because buffer can never be full; must be 2^n

#define TX_NOTIFY_ATTEMPTS_MAX 4  // maximum number of attempts to send a notify message before giving up (no acknowledge received)

#define CURRENT_VALUE_PTR_MAX 30  // max number of characters ("xxx," *30 = 120)


class serialComms : public Print 
{
public :
  serialComms () ;
  void begin () ;
  void set_notify_message (uint8_t, uint8_t)  ;
  void check_notify_message () ;
  void checkCharAvailable () ;
  bool isRxWaitState () ;
  void getSerialInput () ;
  virtual size_t write (uint8_t) ;
  
  typedef enum
  {
    LOOP_DELAY_NONE,
    LOOP_DELAY_COMMS_ALARMS,            // on receipt of command (includes eeprom write) (every loop); alarms every second
    LOOP_DELAY_BEEPER,                  // once or twice in 250ms if beep on
    LOOP_DELAY_SEND_NOTIFY,             // as often as necessary
    LOOP_DELAY_CHECK_RESPONSE,          // twice per second (analog and temp/humidity)
    LOOP_DELAY_EEPROM,                  // once per hour
    LOOP_DELAY_RTC,                     // once every ten minutes
    LOOP_DELAY_DISPLAY,                 // on the dot, every second
    LOOP_DELAY_GPIO_CONDS,              // every 25ms
    LOOP_DELAY_KEYPAD,                  // every 40ms
    LOOP_DELAY_ANALOG,                  // once per second
    LOOP_DELAY_TEMP_HUMIDITY,            // once per second
    LOOP_DELAY_LEDSTRIP,                // update every 25ms 
    LOOP_DELAY_MAX
  } ;
  unsigned int loop_delays [LOOP_DELAY_MAX] ;

  
private :
  typedef enum RxState
  {
    RX_WAIT,          // waiting for the @xx start of message
    RX_RECIP,         // receiving the xx of @xx
    RX_CMD,           // waiting for the actual command
    RX_CMD_VALUE,     // waiting for any (optional) value associated with the command, terminated by cxxx or : or ,
    RX_RESPONSE,      // waiting for the response to the command after the ':' and terminated by cxxx or ,
    RX_CHKSUM,        // receiving the xxx of cxxx at the end of the message 
    RX_CRLF,          // receiving CR or LF
    RX_REPLY,         // command received and processed - send a reply value
    RX_ACK,           // command received and processed - send an acknowledge
    RX_INVALID        // invalid command or command is not for this device
  } ;
  RxState rx_state ;
  bool rx_get_number ;
  byte rx_recipient_id ;  // message intended for this recipient only (or all devices if id=0)
  long rx_new_number ;    // each number as it comes in
  byte rx_num_digits ;    // number of digits expected (2 for the address, 3 for the checksum, otherwise 
  byte rx_num_colons ;    // count the number of colons following a command e.g for @00dyy:mm:ddc000 rx_num_colons = 2

  // initial circular buffer for incoming 
  uint8_t rx_input [256] ; 
  uint8_t rx_input_head ;
  uint8_t rx_input_tail ;
  unsigned int rx_input_overrun_ctr ;
  
  // buffer for receipt/transmission of messages and calculating 
  uint8_t tx_response [128] ;  // store incoming and outgoing messages
  int tx_response_ptr ;

  // circular buffer for transmitting notify messages
  uint8_t tx_notify [TX_NOTIFY_MAX] ;
  uint8_t  tx_notify_head ;
  uint8_t  tx_notify_tail ;
  byte tx_notify_attempt ;    // number of attempts remaining to send current notify message (i.e. start at 4 attempts) 
  unsigned int tx_notify_overrun_ctr ;
  unsigned int tx_no_acknowledge_ctr ;

  uint8_t checksum ;
  byte current_cmd ;  // stored value of actual command
  char current_value [CURRENT_VALUE_PTR_MAX] ;  // stored value of the data for the command - now only 8 bits **** CAREFUL with negatives
  uint8_t current_value_ptr ;    //
  unsigned long rx_millis_ready ;  // the millis() at which a command has been received and successfully processed (including acknowledgement)
  // unsigned long rx_millis_start = 0 ;  // the millis() at which a serial character has been received

  void printTime (TimeElements &time_elements) ;
  void printBuffDeviceId () ;
  void printBuffChecksum () ;
  void flushBuff () ;
  
} ;

extern serialComms SerialComms ; 

#endif   // SERIALCOMMS_H

