#ifndef _time_c
#define _time_c

#include "time.h"
#include "coco.h"

struct tm _lastLocalTime;
UInt32 _lastTime;


void timeTick() {
  unsigned timeLow = getTimer();
  if (timeLow < _lastTime.Lo) {
    _lastTime.Hi++;
  }
  _lastTime.Lo = timeLow;
}


void gettimeofday(struct timeval *now, struct timezone *tzp) {
}


struct tm *localtime (time_t *time) {
  //UInt32 sixty = UInt32Init(0, 60);
  //UInt32Mod(&minutes, &seconds, time, &sixty);
  //UInt32 twentyfour = UInt32Init(0, 24);
  //UInt32 threesixtyfive = UInt32Init(0, 24);
  //UInt32Mod(&hours, &minutes, &minutes, &sixty);
  //UInt32Mod(&days, &hours, &hours, &twentyfour);

  _lastLocalTime.tm_hour = 8;
  _lastLocalTime.tm_min = 30;
  _lastLocalTime.tm_sec = 21;
  _lastLocalTime.tm_mday = 6;
  _lastLocalTime.tm_mon = 12;
  _lastLocalTime.tm_year = 2016;

  return &_lastLocalTime;
}


#endif
