/* 
  Alarms.h        Alarms library
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


#ifndef ALARMS_H
#define ALARMS_H

#include <Arduino.h>
#include <inttypes.h>
#include <EEPROM.h>

#include "EEpromMap.h"            // this defines the use of the EEPROM
#include "EEPROMAnything.h"

#include "Pcode.h"
#include "SerialComms.h"

#define ALARMS_MAX  16              // maximum number of alarms allowed
#define ALARM_REPEAT_BIT (1 << 15)  // bit 15 set if this alarm repeats continuously (otherwise run once until next set)

extern CEepromAnything EEPROM ;

class alarms
{
public:
  alarms () ;
  void begin () ;                           // make sure time() is set before calling this
  void checkAllAlarms () ;
  void setAlarm (uint8_t) ;
  void setAlarm (uint8_t, unsigned int) ;
  unsigned long calculateNextAlarm (unsigned int) ;
  void newTimeSet () ;                      // the correct time has been sent to the arduino; update all alarms that would have gone off
                                            // before this moment
  // void serialPrintTime (unsigned long) ; // testing only
  
  typedef enum AlarmsTypeLimits
  {
    AlarmOff =                    0,        // 0 = no alarm
    AlarmSecondsMax =             60 * 60,  // <3600 = timer mode in 1 second intervals up to one hour
    AlarmTenSecondsMax =          16384,    // timer mode in 10 second intervals from one hour to approx 30 hours
    AlarmOnSecondEveryMinuteMax = 60,       // alarm on the nth second of (every) minute (once per minute)
    AlarmOnMinuteEveryHourMax =   60,       // alarm on the 0th second of the nth minute of (every) hour (once per hour)
    AlarmOnMinuteEveryDayMax =    60 * 24,  // alarm on the 0th second of the nth minute of the nth hour daily (once per day)
    AlarmRepeatBit = ALARM_REPEAT_BIT
  } ;

  unsigned long alarmTime [ALARMS_MAX] ;    // the next unixtime for each of 16 alarms
  
private:
  unsigned long time_now ;                  // the time at the start of checking 
  void setNextAlarm (uint8_t, unsigned int) ;

} ;

extern alarms Alarms ; 


#endif    // ALARMS_H

/***** 
0,0      alarm off

e144:2:1,128      repeat every second
e144:2:60,128     repeat every minute
e144:2:14,144     repeat every hour



*****/


