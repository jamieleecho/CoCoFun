#ifndef _TIME_H
#define _TIME_H

#include "c_stuff.h"

typedef unsigned long time_t;

struct tm {
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};

struct timezone {
};

struct timeval {
  unsigned tv_sec;
  unsigned tv_msec;
};

void gettimeofday(struct timeval *now, struct timezone *tzp);
struct tm *localtime (time_t *time);

#endif
