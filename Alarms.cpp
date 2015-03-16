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


#include "Alarms.h"


alarms::alarms ()    // constructor - nothing to do here
{
}


void alarms::begin ()
{
  for (uint8_t alarm_num = 0 ; alarm_num < ALARMS_MAX ; alarm_num++)
  {
    alarmTime [alarm_num] = now () ;  // unused alarms are switched off
  }
  newTimeSet () ;
}


void alarms::newTimeSet ()
{
  for (uint8_t alarm_num = 0 ; alarm_num < ALARMS_MAX ; alarm_num++)
  {
    unsigned int eeprom_position = EEPROM.read (eeMap::PcodeAlarmsIndex + alarm_num) ;
    // Serial.print ("eeprom_position=") ;
    // Serial.print (eeprom_position, DEC) ;

    if (eeprom_position)
    {
      eeprom_position += (eeMap::PcodeAlarmsIndex & 0xFF00) ; // 2 bytes for the alarm_repeat_command
      // Serial.print (", eeprom_position=") ;
      // Serial.print (eeprom_position, DEC) ;
      unsigned int alarm_repeat_command = EEPROM.read (eeprom_position++) ;
      alarm_repeat_command += EEPROM.read (eeprom_position++) * 256 ;
      // Serial.print (" = ") ;
      // Serial.println (alarm_repeat_command, DEC) ;
      if (alarmTime [alarm_num] && alarmTime [alarm_num] <= now ())
      {
        // only set the alarm if it was either not running (=0), or was set to go off before the new time now()
        alarmTime [alarm_num] = 0 ; // make sure the alarm is initially off
        setAlarm (alarm_num, alarm_repeat_command) ;
      }
    } else
    {
      alarmTime [alarm_num] = 0 ;  // unused alarms are switched off
    }
  }
}


void alarms::setAlarm (uint8_t alarm_num)
{
  unsigned int eeprom_position = EEPROM.read (eeMap::PcodeAlarmsIndex + alarm_num) ;
  if (eeprom_position)
  {
    eeprom_position += (eeMap::PcodeAlarmsIndex & 0xFF00) ; // 2 bytes (int) for the 
    // Serial.print (", eeprom_position=") ;
    // Serial.print (eeprom_position, DEC) ;
    unsigned int alarm_repeat_command = EEPROM.read (eeprom_position++) ;
    alarm_repeat_command += EEPROM.read (eeprom_position++) * 256 ;
    setAlarm (alarm_num, alarm_repeat_command) ;
  }
}


/*
void alarms::serialPrintTime (unsigned long time_now)
{
  TimeElements time_now_elements ;
  breakTime (time_now, time_now_elements) ; 
  Serial.print (time_now_elements.Day) ;  
  Serial.print ('/') ;
  Serial.print (time_now_elements.Month) ;  
  Serial.print ('/') ;
  Serial.print (time_now_elements.Year - 30) ;  
  Serial.print (' ') ;
  Serial.print (time_now_elements.Hour) ;  
  Serial.print (':') ;
  Serial.print (time_now_elements.Minute) ;  
  Serial.print (':') ;
  Serial.print (time_now_elements.Second) ;
}
*/


void alarms::setAlarm (uint8_t alarm_num, unsigned int alarm_repeat_command)
{
  alarmTime [alarm_num] = calculateNextAlarm (alarm_repeat_command) ;
}  
  
  
unsigned long alarms::calculateNextAlarm (unsigned int alarm_repeat_command)
{
  unsigned long alarm_next ;
  
  if (!alarm_repeat_command)
  {
    // clear the alarm
    return 0 ;
  }
  alarm_repeat_command &= ~AlarmRepeatBit ; // clear the repeat bit
  if (alarm_next == 0) 
  {
    alarm_next = now () ;
  }
  if (alarm_repeat_command <= AlarmSecondsMax)
  {
    // alarm set to between 1 second from now to 1 hour from now
    // Serial.print (", repeat within hour");
    alarm_next += alarm_repeat_command ;
  } else
  if (alarm_repeat_command < AlarmTenSecondsMax)
  {
    // alarm set to between 1 hour from now to about 24 hours from now
    // Serial.print (", repeat >1hr within day");
    alarm_next += (alarm_repeat_command - AlarmSecondsMax) * 10 ;
  } else
  {
    alarm_repeat_command -= AlarmTenSecondsMax ; 
    TimeElements time_now_elements ;
    breakTime(alarm_next, time_now_elements) ; // fill in all the values to time now
    if (alarm_repeat_command < AlarmOnSecondEveryMinuteMax)
    {
      // Serial.print (", 1 sec every minute");
      time_now_elements.Second = alarm_repeat_command ;
      time_now_elements.Minute ++ ;  // it can cope with 61
      alarm_next = makeTime (time_now_elements) ;
    } else 
    {
      alarm_repeat_command -= AlarmOnSecondEveryMinuteMax ; 
      if (alarm_repeat_command < AlarmOnMinuteEveryHourMax)
      {
        // Serial.print (", 1 min hourly");
        time_now_elements.Second = 0 ;
        time_now_elements.Minute = alarm_repeat_command ;  
        time_now_elements.Hour ++ ;  // it can cope with 25
        alarm_next = makeTime (time_now_elements) ;
      } else 
      {
        // Serial.print (", 1 min daily");
        alarm_repeat_command -= AlarmOnMinuteEveryHourMax ; 
        if (alarm_repeat_command < AlarmOnMinuteEveryDayMax)
        {
          time_now_elements.Second = 0 ;
          time_now_elements.Minute = alarm_repeat_command % 60 ;  
          time_now_elements.Hour = alarm_repeat_command / 60 ;
          alarm_next = makeTime (time_now_elements) ;  // set the daily alarm to today
          if (alarm_next <= now ())
          {
            time_now_elements.Day ++ ;  // it can cope with 32
            alarm_next = makeTime (time_now_elements) ;
          }
        } else
        {
          // could write more for day of week if desired
          // alarm_repeat_command -= AlarmOnMinuteEveryDayMax ; 
          alarm_next = 0 ; 
        }
      }
    }
  }
  return alarm_next ;
  /*
  Serial.print (", now=") ;
  serialPrintTime (now()) ;
  Serial.print (", alarmTime=") ;
  serialPrintTime (alarmTime [alarm_num]) ;  
  Serial.println () ;
  */
}


void alarms::setNextAlarm (uint8_t alarm_num, unsigned int alarm_repeat_command)
{
  // an alarm has just run - set the next one if required
  if (alarm_repeat_command && (alarm_repeat_command & AlarmRepeatBit))
  {
    setAlarm (alarm_num, alarm_repeat_command) ;
  } else
  {
    alarmTime [alarm_num] = 0 ;
  }
}


void alarms::checkAllAlarms () 
{
  time_now = now () ;
  for (uint8_t alarm_num = 0 ; alarm_num < ALARMS_MAX ; alarm_num++)
  {
    if (alarmTime [alarm_num] && time_now >= alarmTime [alarm_num])
    {
      unsigned int eeprom_position = EEPROM.read (eeMap::PcodeAlarmsIndex + alarm_num) ;
      /*
      Serial.print ("checkAlarm ") ;
      Serial.print (alarm_num, DEC) ;
      Serial.print (", eeprom_position=") ;
      Serial.print (eeprom_position, DEC) ;
      */
      if (eeprom_position)
      {
        eeprom_position += (eeMap::PcodeAlarmsIndex & 0xFF00) ; // 2 bytes (int) for the 
        // Serial.print (", eeprom_position=") ;
        // Serial.print (eeprom_position, DEC) ;
        unsigned int alarm_repeat_command = EEPROM.read (eeprom_position++) ;
        alarm_repeat_command += EEPROM.read (eeprom_position++) * 256 ;
        // Serial.print (", alarm_repeat_command=") ;
        // Serial.println (alarm_repeat_command, DEC) ;
        setNextAlarm (alarm_num, alarm_repeat_command) ;
        uint8_t alarm_response = Pcode.change_ports (eeprom_position) ;
        // send an notify message to the master
        if (IS_NOTIFY_ALARM_SET)  // read the EEPROM to see whether to send notify messages on alarms
        {
          // always send a notify message if the display is in interactive mode
          SerialComms.set_notify_message (pcode::PcodeCmdAlarm + alarm_num, alarm_response) ;
        }        
      } else
      {
        alarmTime [alarm_num] = 0 ; // just clear the alarm
      }
    }
  }
}

alarms Alarms ; 

