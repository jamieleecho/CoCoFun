#ifndef _time_c
#define _time_c

#include "time.h"
#include "coco.h"

struct tm _lastLocalTime;
unsigned _timeHigh, _timeLow;


void timeTick() {
  unsigned timeLow = getTimer();
  if (timeLow < _timeLow) {
    _timeHigh++;
  }
}


void gettimeofday(struct timeval *now, struct timezone *tzp) {
}


struct tm *localtime (time_t *time) {
  return &_lastLocalTime;
}


#endif
