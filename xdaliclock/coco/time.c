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
  return &_lastLocalTime;
}


#endif
