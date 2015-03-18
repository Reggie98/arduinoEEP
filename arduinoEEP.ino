/*
  arduinoEEP.ino  Arduino EEP control software
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
 

#define VERSION             "1.00"
#define VERSION_DATE        20150318

#define DEVICE_ID           4       // only used in initial setup to put the value into the EEPROM
#define EEPROM_CLEAR_ALL    0       // clear all the values in the EEPROM to a default state
#define EEPROM_DUMP_ALL     0       // dump all the EEPROM values to serial after a delay of 5 seconds from power on

// Includes
#include "Limits.h"                 // system wide limits for code to compile on each device, and maximum allowed attachments (eg. LEDs)
                                    // and arduino pin assignments

#include <EEPROM.h>
#include <Time.h>

#if COMPILE_DS1307RTC
  #include <DS1307RTC.h>
  #define RTC_UPDATE_PERIOD 600     // synchronise the arduino clock from the RTC clock once every ten minutes
#endif

#define ONEWIRE_CRC8_TABLE  0       // don't use the fast CRC 256 bit table
#define ONEWIRE_CRC16       0       // no need for CRC16 code
#include <OneWire.h>                // for DS18B20 temperature sensors
#include <Wire.h>                   // I2C for relays, keypad, HTU21D temp humidity sensor

#include "EEPROMAnything.h"         // read or write anything to EEPROM, only write if new value is not the same as existing 
                                    // (each write is 3.5ms)
                                    // this is my adapted version of EEPROMAnything
#include "EEpromMap.h"              // this defines the use of the EEPROM


// #include <avr/wdt.h>             // UNSURE - don't trust this yet, need to know it will never hang up


#include "MCP23017.h"               // code for dealing with the MCP23017 16-bit GPIO extenders
#include "Beeper.h"    
#include "Keypad.h"
#include "AnalogInputs.h"
#include "TempHumidity.h"           // code for DS18B20 and HTU21D
#include "Pcode.h"
#include "SerialComms.h"
#include "Alarms.h"

#if COMPILE_LEDSTRIP
  #include <FastLED.h>
  #include "LedStrip.h"
#endif

#if COMPILE_NOKIA_5110
  #include <SPI.h>
  #include <PCD8544_SPI.h>
  #include "Display.h"
#endif


char version_str [] = VERSION ;

// GLOBALS copied from the EEPROM at startup
uint8_t device_id ;             // read from EEPROM at startup

uint8_t arduino_gpio [3] ;      // 0 is D0-D7, 1 is D8-D13, 2 is A0-A7
uint8_t arduino_gpio_last [3] ; // 0 is D0-D7, 1 is D8-D13, 2 is A0-A7; 
                                // these are the previous values of the arduino digital inputs 
                                // don't update until all checking is done

volatile unsigned long interrupt_count0 ;
volatile unsigned long interrupt_count1 ;

// Setup a oneWire instance to communicate with any OneWire devices (DS18B20)
OneWire oneWire (ONE_WIRE_PIN) ;                  

// Time
byte last_hour ;


const unsigned long GPIO_SAMPLE_RATE = 25 ;      // sample the GPIO every 25ms  
unsigned long gpio_millis_start ;

void read_gpio_check_cond ()
{
  if ((millis () - gpio_millis_start) >= GPIO_SAMPLE_RATE )
  {
    gpio_millis_start = millis () ;
    
    // read all the GPIO states into arrays - from the Arduino and the MCP23017s
    arduino_gpio [0] = PIND ;  // D0 to D7 (but we only check D2-D7)
    arduino_gpio [1] = PINB ;  // D8 to D13
    arduino_gpio [2] = PINC  ; // A0 to A7 (but we only check A0 to A3)

    // read all the GPIO states for the MCP23017 devices
    Mcp23017.read_gpio_all () ; // read all the gpio inputs and store in mcp_gpio_current []

    // now we can check all the pcodes for gpio
    Pcode.check_all_gpio_conditions () ;

    // copy current input states to the last state
    for (uint8_t i = 0; i < 3; i++)
    {
      arduino_gpio_last [i] = arduino_gpio [i] ; 
    }  

    Mcp23017.save_gpio_all () ; // save all the current gpio inputs states (mcp_gpio_current []) to mcp_gpio_last []
  }
}


uint8_t check_any_response ()
{
  // see if there is a command for this keypad 
  // CHANGE to allow several keypad states - e.g. interactive (control arduino) or menu (send keypad commands to master, instructions/options on display)
  if (Keypad.isKeyReady ())
  {
    uint8_t key_response = 0 ;
    uint8_t key_pressed = Keypad.getKeyPressed () ;
    
    if (IS_BEEP_KEY_SET)  // in EEpromMap, read the EEPROM to see whether to beep on keypad pressed
    {
      Beeper.setBeep (beeper::BeepOnce) ; // short beep on keypress
    }
    
    #if COMPILE_NOKIA_5110
      if (EEPROM.read (eeMap::PcodeKeysIndex + key_pressed) && !Display.isDisplayInteractive ())
      {
        // don't check for any responses if the Display is in interactive mode
        key_response = Pcode.change_ports ((eeMap::PcodeKeysIndex & 0xFF00) + EEPROM.read (eeMap::PcodeKeysIndex + key_pressed)) ;
      }
    #else
      if (EEPROM.read (eeMap::PcodeKeysIndex + key_pressed))
      {
        // don't check for any responses if the Display is in interactive mode
        key_response = Pcode.change_ports ((eeMap::PcodeKeysIndex & 0xFF00) + EEPROM.read (eeMap::PcodeKeysIndex + key_pressed)) ;
      }
    #endif
    
    // send an notify message to the master
    #if COMPILE_NOKIA_5110
      if (IS_NOTIFY_KEY_SET || Display.isDisplayInteractive ())  // read the EEPROM to see whether to send notify messages on keypad pressed
      {
        // always send a notify message if the display is in interactive mode
        SerialComms.set_notify_message (pcode::PcodeCmdKey + key_pressed, key_response) ;
      }
    #else
      if (IS_NOTIFY_KEY_SET)  // read the EEPROM to see whether to send notify messages on keypad pressed
      {
        // always send a notify message if the display is in interactive mode
        SerialComms.set_notify_message (pcode::PcodeCmdKey + key_pressed, key_response) ;
      }    
    #endif
    
    Keypad.setKeyProcessed () ;  // the keypad button has now been processed
    return 20 ;
  } 
  if (AnalogInputs.isAnalogReady ())
  {
    // All analog channels have now been read - check if any pcode conditions have been met
    for (byte analog_port = 0 ; analog_port < ANALOG_INPUTS_MAX ; analog_port++)
    {
      // now allows arduino voltage and temp to be tested for pcode conditions
      uint8_t has_analog_changed = Pcode.check_int_high_low (eeMap::PcodeAnalogIndex, analog_port, 
                                                     AnalogInputs.analog_values [analog_port], AnalogInputs.previous_checked ) ;
      if (has_analog_changed && IS_NOTIFY_ANALOG_SET)   // read the EEPROM to see whether to send notify messages on Analog changes
      {
        // put the temp command into the tx_int_buff 
        SerialComms.set_notify_message (pcode::PcodeCmdAnalog + analog_port, has_analog_changed) ;
      }
    }
    AnalogInputs.setAnalogWait () ;
    return 21 ;
  } 
  if (TempHumidity.isTempHumidityReady ())
  {
    // All DS18B20 and HTU21D channels have now been read - check if any pcode conditions have been met
    for (byte sensor = 0 ; sensor < TEMP_HUMIDITY_SENSORS_MAX ; sensor++)
    {
      uint8_t has_temp_changed = Pcode.check_int_high_low (eeMap::PcodeTempsIndex, sensor, 
                                                     TempHumidity.temp_values [sensor], TempHumidity.temp_previous_checked ) ;
      if (has_temp_changed && IS_NOTIFY_TEMP_SET)   // read the EEPROM to see whether to send notify messages on Temp changes
      {
        // put the temp command into the tx_int_buff 
        SerialComms.set_notify_message (pcode::PcodeCmdTemp + sensor, has_temp_changed) ;
      }

    }
    TempHumidity.setTempHumidityWait () ;
    return 22 ;
  }
  return 23 ;
}


// Interrupt Service Routine (ISR)
void interrupt_count0_rising ()
{
  interrupt_count0 ++ ;
}

void interrupt_count1_rising ()
{
  interrupt_count1 ++ ;
}


void software_reset()
{
/*
  wdt_enable(WDTO_15MS);   // DANGER - this might get into an eternal loop
  while(1)
  {
  }
*/
  /*
  Serial.println () ;
  Alarms.serialPrintTime (now ()) ;
  Serial.println () ;
  */
  // Serial.println ("RST") ;
  Serial.flush () ;  
  EEPROM.updateLong (eeMap::Time, now ()) ;  // could be 13.2ms (3.3ms / write byte)
  /*
  unsigned long read_time ;
  read_time = EEPROM.readLong (eeMap::Time) ;
  Alarms.serialPrintTime (now ()) ;
  Serial.println () ;
  */
  EEPROM.updateLong (eeMap::InterruptCount0, interrupt_count0) ;
  EEPROM.updateLong (eeMap::InterruptCount1, interrupt_count1) ;  
  delay (100) ; 
  // Restarts program from beginning but does not reset the peripherals and registers
  asm volatile ("  jmp 0") ;  
} 


int freeMemory () 
{
  extern int __heap_start, *__brkval ;
  int v ;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval) ;
}


#if COMPILE_DS1307RTC
  unsigned long time_last_updated ;
#endif


void setup()
{
  // make sure the RS485 is set to receive
  pinMode (TXE_PIN, OUTPUT) ;
  digitalWrite (TXE_PIN, RS485_TX_DISABLE);             // on prototype this only stops the transmission; receiving is disabled in hardware

  Serial.begin (115200) ;  

  #if EEPROM_CLEAR_ALL    
    int year = VERSION_DATE / 10000  ;                  // 20140320 => 2014
    int month = VERSION_DATE % 10000 ;                  // 20140320 => 320
    int day = month % 100 ;                             // 20140320 => 20
    month = month / 100 ;                               // 20140320 => 3
    setTime (0, 0, 0, day, month, year) ;               // assume midnight 

    // Serial.println ("clear") ;
    EEPROM.updateByte (eeMap::DeviceId, DEVICE_ID) ;    // set this device as #4
    EEPROM.updateByte (eeMap::BeeperTone, 20) ;

    EEPROM.updateLong (eeMap::Time, now ()) ;

    EEPROM.updateByte (eeMap::DisplayVop, 0xBC) ;       // 188
    EEPROM.updateByte (eeMap::DisplayTempcoef, 0x05) ;  // 5
    EEPROM.updateByte (eeMap::DisplayBias, 0x14) ;      // 20

    EEPROM.updateByte (eeMap::RxMillisDelay, 7) ;       // wait 7ms after the receipt of a serial char before deciding to send an notify message to the master

    EEPROM.updateByte (eeMap::PortDIO, B11110010) ;     // digital pins 0 to 7 set as INPUT (0) or OUTPUT (1) NOTE: not the same as on MCP (where Input = 1, Output = 0)
      // NOTES:
      //    0 is RX (must be input)
      //    1 is TX (must be output)
      //    2, 3 can be outputs (e.g. relays) or inputs (e.g. meter flash counters, flow meter counters) 
      //    4 is TXE_PIN (must be output)
      //    5 is BUZZER_PIN (if there is a buzzer, therefore output) or input
      //    6 is LED_PIN  
      //    7 is ONE_WIRE_PIN
      //
    EEPROM.updateByte (eeMap::PortDPullup, B00000000) ; // pullups enabled on digital pins 0 to 7 (probably all set as no pullups (0))
    EEPROM.updateByte (eeMap::PortBIO, B00000000) ;     // digital pins 8 to 13 set as INPUT (0) or OUTPUT (1) - NB: 14,15 are the crystal; Can't use any if display enabled
    EEPROM.updateByte (eeMap::PortBPullup, B00000000) ; // pullups enabled on digital pins 8 to 13 (probably all set as no pullups (0))
    EEPROM.updateByte (eeMap::PortCIO, B00001100) ;     // analogue pins 0 to 7 set as INPUT (0) or OUTPUT (1)
      // NOTES:
      //    4 is I2C SDA (set as output)
      //    5 is I2C SCL (must be output)
      //    6, 7 cannot be used as digital output
      // 
    EEPROM.updateByte (eeMap::PortCPullup, B00000000) ; // pullups enabled on digital pins 0 to 7 (probably all set as no pullups (0))

    // EEPROM.updateByte (eeMap::TempSensorResolution, 12) ; // DS18B20 resolution

    EEPROM.updateByte (eeMap::McpNumDevices, 1) ;       // single MCP device only
    
    for (byte i = 0; i < 8; i++)
    {
      EEPROM.updateByte (eeMap::McpIodir + i, 0xFF) ;   // set all as inputs
      EEPROM.updateByte (eeMap::McpGppu + i, 0x00) ;    // no pullups on any inputs
    }

    EEPROM.updateByte (eeMap::McpIodir, 0x0F) ;         // D7 - D4 = outputs, D3 - D0 = inputs for keypad
    EEPROM.updateByte (eeMap::McpIodir + 1, 0xF0) ;     // D7 - D4 = inputs, D3 - D0 = outputs for relay
    EEPROM.updateByte (eeMap::McpGppu, 0x0F) ;          // D7 - D4 = outputs (no pull-ups), D3 - D0 = inputs (use pullups) for keypad
    EEPROM.updateByte (eeMap::McpGppu + 1, 0xF0) ;      // D7 - D4 = inputs (use pullups), D3 - D0 = outputs (no pull-ups) for relay

    EEPROM.updateLong (eeMap::InterruptCount0, 0) ;
    EEPROM.updateLong (eeMap::InterruptCount1, 0) ;

    EEPROM.updateByte (eeMap::LedGroup, 1) ;            // number of LEDs in a group
    EEPROM.updateByte (eeMap::LedOnRgb, B00011000) ;    // green for LED on colour
    EEPROM.updateByte (eeMap::LedOffRgb, B11100000) ;   // red for LED off colour
    EEPROM.updateByte (eeMap::LedNumLeds, 50) ;         // 50 led strip

    EEPROM.updateByte (eeMap::OptionBeep, 0) ;          // all beeps disabled
    EEPROM.updateByte (eeMap::OptionNotify, 0xFF) ;     // notify on all conditions (key pressed, temp changed, io condition met)
    EEPROM.updateByte (eeMap::OptionGpio, 0x01) ;       // use 16-key keypad

    for (int i = 64 ; i < 1024 ; i++)
    {
      EEPROM.updateByte (i, 255) ;
    }
    
    for (byte i = 0 ; i < 16; i++)
    {
      // put all the pcode index bytes to zero
      EEPROM.updateByte (eeMap::PcodeKeysIndex + i, 0) ;
      EEPROM.updateByte (eeMap::PcodeTempsIndex + i, 0) ;
      EEPROM.updateByte (eeMap::PcodeAnalogIndex + i, 0) ;
      EEPROM.updateByte (eeMap::AnalogTypes + i, 0) ;
      EEPROM.updateByte (eeMap::PcodeAlarmsIndex + i, 0) ;
      EEPROM.updateByte (eeMap::PcodeInputsIndex + i, 0) ;
      EEPROM.updateByte (eeMap::DisplayMap + i, 0) ;
    }
    
    EEPROM.updateByte (eeMap::DisplayMap + 2, 84) ; // 
    // Serial.println ("clear done") ;

  #endif
  
  #if COMPILE_NOKIA_5110
    // read the initial state values from EEPROM and start the display
    Display.begin (false, EEPROM.read (eeMap::DisplayVop), EEPROM.read (eeMap::DisplayTempcoef), EEPROM.read (eeMap::DisplayBias)) ;
    Display.clear () ;
  #endif

  arduino_gpio [0] = EEPROM.read (eeMap::PortDIO) ;
  arduino_gpio [1] = EEPROM.read (eeMap::PortBIO) ;
  arduino_gpio [2] = EEPROM.read (eeMap::PortCIO) ;  
  
  // set relays on analog pins to output 
  PORTD |= EEPROM.read (eeMap::PortDPullup) & ~eeMap::PortDIOFixed ;   // if ee_pullup=1 and fixed=0 then enable the pullup else don't change
  DDRD &= arduino_gpio [0] | eeMap::PortDIOFixed ;  // if ee=0 and fixed=0 then set the DDRD to 0 (input) else don't change
  DDRD |= arduino_gpio [0] & ~eeMap::PortDIOFixed ;  // if ee=1 and fixed=0 then set the DDRD to 1 (output) else don't change
  PORTD |= arduino_gpio [0] & ~eeMap::PortDIOFixed ;   // if ee_io=1 (output) and fixed=0 then set output to 1
  
  PORTB |= EEPROM.read (eeMap::PortBPullup) & ~eeMap::PortBIOFixed ;   // if ee=1 and fixed=0 then enable the pullup else don't change
  DDRB &= arduino_gpio [1] | eeMap::PortBIOFixed ;  // if ee=0 and fixed=0 then set the DDRB to 0 (input) else don't change
  DDRB |= arduino_gpio [1] & ~eeMap::PortBIOFixed ;  // if ee=1 and fixed=0 then set the DDRB to 1 (output) else don't change
  PORTB |= arduino_gpio [1] & ~eeMap::PortBIOFixed ;   // if ee_io=1 (output) and fixed=0 then set output to 1

  PORTC |= EEPROM.read (eeMap::PortCPullup) & ~eeMap::PortBIOFixed ;   // if ee=1 and fixed=0 then enable the pullup else don't change
  DDRC |= arduino_gpio [2] & ~eeMap::PortCIOFixed ;  // if ee=1 and fixed=0 then set the DDRC to 1 (output) else don't change
  DDRC &= arduino_gpio [2] | eeMap::PortCIOFixed ;  // if ee=0 and fixed=0 then set the DDRD to 0 (input) else don't change
  PORTC |= arduino_gpio [2] & ~eeMap::PortCIOFixed ;   // if ee_io=1 (output) and fixed=0 then set output to 1
  
  pinMode (0, INPUT) ; // Just to be sure the RX pin remains as an input
  pinMode (1, OUTPUT) ; // Just to be sure the TX pin remains as an output

  device_id = EEPROM.read (eeMap::DeviceId) ;
  
  // NOTE: time has to be set up before some of the classes are initialised
  #if COMPILE_DS1307RTC
    setTime (RTC.get()) ;
    time_last_updated = now () ;
  #endif
  
  /*
  Serial.println () ;
  Alarms.serialPrintTime (now ()) ;
  Serial.println () ;
  */
  
  last_hour = hour () ; // update the time EEPROM hourly
  
  // Initiallise all the MCP devices (up to 4)
  Wire.begin (); // initialise the wire library and hardware
  Mcp23017.begin () ; 
  
  // start the beeper and the keypad
  Beeper.begin () ;
  Keypad.begin () ;
  
  SerialComms.begin () ;
  
  // read all the initial values of analog ports and set the min/max values
  AnalogInputs.begin () ;  

  // read all the initial values of temp/humidity sensors and set the min/max values
  TempHumidity.begin () ;
  
  #if COMPILE_LEDSTRIP
    LedStrip.addLeds<WS2811, LED_PIN, GRB>(LedStrip.leds, LEDS_MAX) ;
    LedStrip.clear () ;
    digitalWrite (LED_PIN, 0) ;  // needs to be 0 before sending first command
    delay (7) ;  // 7000uS
    LedStrip.show(); // Sends the values to the LEDs
  #endif
  // Serial.println ("start4") ;  

  // save the compiler options in EEPROM 
  EEPROM.updateByte (eeMap::OptionCompile, EE_COMPILE_NOKIA_5110_BIT | EE_COMPILE_LEDSTRIP_BIT) ;  
  
  // save the compiled limitations in EEPROM
  EEPROM.updateByte (eeMap::LedsMax, LEDS_MAX)  ;
  EEPROM.updateByte (eeMap::TempSensorsMax, TEMP_SENSORS_MAX) ;
  EEPROM.updateByte (eeMap::AnalogInputsMax, ANALOG_INPUTS_MAX) ;
  EEPROM.updateByte (eeMap::Mcp23017Max, MCP23017_MAX) ;

  // read the interrupt counters previous values
  interrupt_count0 = EEPROM.readLong (eeMap::InterruptCount0) ;  
  interrupt_count1 = EEPROM.readLong (eeMap::InterruptCount1) ;  

  attachInterrupt (0, interrupt_count0_rising, RISING) ;  
  attachInterrupt (1, interrupt_count1_rising, RISING) ;  
  
  // wdt_enable (WDTO_250MS) ;  // start the watchdog timer (250ms max per loop) e.g. eeprom write 16 bytes = 50ms
  
  #if EEPROM_DUMP_ALL
    // Serial.println ("dump") ;
    delay (2000) ;
    for (int i = 0; i < 1024 ; i++)
    {
      uint8_t ee = EEPROM.read (i) ;
      if ( (i & 0x000F) == 0 ) 
      {
        Serial.println () ;
        Serial.print ('e') ;
        Serial.print (i & 0x00FF) ; // low byte
        Serial.print (':') ;
        Serial.print (i >> 8) ; // high byte
        Serial.print (':') ;
      } else
      {
        Serial.print (',') ;
      }
      Serial.print (ee) ;
    }
    Serial.println () ;
  #endif
  
  Alarms.begin () ;

  SerialComms.set_notify_message (pcode::PcodeCmdPowerUp, 0) ; // send a power-up signal
  for ( uint8_t i = serialComms::LOOP_DELAY_MAX ; i ; ) 
  {
    SerialComms.loop_delays [--i] = 0 ;  // reset all the loop delays
  }
}


unsigned long loop_start_micros ;
unsigned long loop_start_millis ;
uint8_t loop_previous_secs = 0 ;
unsigned long loop_count_fast = 0 ;
unsigned long loop_count_total = 0 ;
unsigned long loop_time ;

unsigned long eeprom_save_long = 0 ;
byte eeprom_save_state = 0 ;

const unsigned long LOOP_MICROS_MAX = 250 ;  // don't check for actions if the previous checks have taken more than 250us

bool is_loop_time_OK ()
{
  return micros () - loop_start_micros <= LOOP_MICROS_MAX ;
}


void loop()
{
  loop_start_micros = micros () ;
  loop_start_millis = millis () ;
  uint8_t loop_last_delay = serialComms::LOOP_DELAY_NONE ;

  // wdt_reset () ;  // clear the watchdog timer for this loop cycle
  
  SerialComms.getSerialInput () ;
  
  SerialComms.checkCharAvailable () ;

  if (second () != loop_previous_secs)
  {
    loop_previous_secs = second () ;
    
    Alarms.checkAllAlarms () ;
  }

  if (is_loop_time_OK ())
  {
    Beeper.processBeep () ; // check if any beep set
  }  else if (!loop_last_delay)
  {
    loop_last_delay = serialComms::LOOP_DELAY_COMMS_ALARMS ;
  }
  
  
  if (SerialComms.isRxWaitState ())
  {
    if (is_loop_time_OK ())
    {
      SerialComms.check_notify_message () ;  // send out notification messages from any previous loop cycle
    }  else if (!loop_last_delay)
    {
      loop_last_delay = serialComms::LOOP_DELAY_BEEPER ;
    }

    uint8_t check_any_response_delay = 0 ;
    if (is_loop_time_OK ())
    {
      check_any_response_delay = check_any_response () ;  // respond to any key or gpio state change that was set in the previous loop cycle
    }  else if (!loop_last_delay)
    {
      loop_last_delay = serialComms::LOOP_DELAY_SEND_NOTIFY ;
    }
    
    // update the time EEPROM and interrupt counters hourly
    if (is_loop_time_OK ())
    {
      EEPROM.checkUpdate () ;      // checks if it is a new hour, and starts updates of hourly saved data (time, interrupt counters)
    }  else if (!loop_last_delay)
    {
      loop_last_delay = serialComms::LOOP_DELAY_CHECK_RESPONSE ;
    }

    #if COMPILE_DS1307RTC
      if (is_loop_time_OK ())
      {
        if (now () - time_last_updated > RTC_UPDATE_PERIOD )
        {
          time_last_updated = RTC.get() ;
          if (time_last_updated)
          {
            setTime (time_last_updated) ;
          } else
          {
            time_last_updated = now () ;
          }          
        }
      }  else if (!loop_last_delay)
      {
        loop_last_delay = serialComms::LOOP_DELAY_EEPROM ;
      }
    #endif
    
    #if COMPILE_NOKIA_5110
      if (is_loop_time_OK ())
      {
        Display.checkUpdate () ;
      }  else if (!loop_last_delay)
      {
        loop_last_delay = serialComms::LOOP_DELAY_RTC ;
      }
    #endif
    
    if (is_loop_time_OK ())
    {
      read_gpio_check_cond () ;  
    }  else if (!loop_last_delay)
    {
      loop_last_delay = serialComms::LOOP_DELAY_DISPLAY ;
    }
    
    if (is_loop_time_OK ())
    {
      Keypad.checkKeyPressed () ;
    }  else if (!loop_last_delay)
    {
      loop_last_delay = serialComms::LOOP_DELAY_GPIO_CONDS ;
    }
    
    if (is_loop_time_OK ())
    {
      AnalogInputs.checkAnalogInputs () ;
    }  else if (!loop_last_delay)
    {
      loop_last_delay = serialComms::LOOP_DELAY_KEYPAD ;
    }
    
    if (is_loop_time_OK ())
    {
      TempHumidity.checkTempHumidity () ;
    }  else if (!loop_last_delay)
    {
      loop_last_delay = serialComms::LOOP_DELAY_ANALOG ;
    }
    
    if (is_loop_time_OK ())
    {
      #if COMPILE_LEDSTRIP
        LedStrip.checkUpdate () ; // check if the leds need to be updated
        loop_last_delay = serialComms::LOOP_DELAY_LEDSTRIP ;
      #endif
    }  else if (!loop_last_delay)
    {
      loop_last_delay = serialComms::LOOP_DELAY_TEMP_HUMIDITY ;
    }
    
  }
  
  loop_time = micros () - loop_start_micros ;
  loop_count_total ++ ;
  if (loop_time < 200) loop_count_fast ++ ;
  if (loop_time < 10000000)  // every 429 seconds micros () goes back to zero
  {
    if (SerialComms.loop_delays [loop_last_delay] < loop_time )
    {
      SerialComms.loop_delays [loop_last_delay] = loop_time ;
    }
  }
}
