/* xdaliclock - a melting digital clock
 * Copyright (c) 1991-2005 Jamie Zawinski <jwz@jwz.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 */

#ifndef _xdaliclock_h
#define _xdaliclock_h

#include "c_stuff.h"
#include "time.h"

extern char *progname;

struct render_state;

#define HHMMSS 0
#define HHMM 1
#define SS 2

#define MMDDYY 0
#define DDMMYY 1
#define YYMMDD 2

struct dali_config {
  byte                            time_mode;
  byte                            date_mode;
  int display_date_p;			     /* displaying time, or date */
  int twelve_hour_p;			     /* 12 hour time vs 24 hour time */
/*  int cycle_p;	*/			     /* color cycling */
/*  int transparent_p;	*/		     /* shaped window */
  time_t countdown;			     /* countdown to/from here */
  char test_hack;			     /* display this character once */

  int max_fps;				     /* desired frames per second */
  int max_cps;				     /* color cycle ticks per second */
  struct render_state *render_state;         /* internal to digital.c */
  unsigned char *bitmap;		     /* rendered output */
  unsigned int width, height;		     /* size of bitmap */
  unsigned int left_offset;
};

extern void render_init (struct dali_config *c);
extern void render_free (struct dali_config *c);
extern void render_once (struct dali_config *c);
extern void render_bitmap_size (struct dali_config *c,
                                unsigned int *w_ret, unsigned int *h_reti,
                                unsigned int *w2_ret, unsigned int *h2_ret);
#endif /* __DALICLOCK_H__ */
