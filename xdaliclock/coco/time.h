#ifndef _time_h
#define _time_h

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
  unsigned tv_sec_hi;
  unsigned tv_sec_lo;
  unsigned tv_xsec;
};

void gettimeofday(struct timeval *now, struct timezone *tzp);
struct tm *localtime (time_t *time);

#endif
