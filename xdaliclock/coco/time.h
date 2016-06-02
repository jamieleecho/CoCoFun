#ifndef _time_h
#define _time_h

#include "c_stuff.h"
#include "UInt32.h"

typedef UInt32 time_t;

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
  UInt32 tv_sec;
  unsigned tv_xsec;
};

void gettimeofday(struct timeval *now, struct timezone *tzp);
struct tm *localtime (time_t *time);

#endif
