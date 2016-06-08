/* xdaliclock - a melting digital clock
 * Copyright (c) 1991-2010 Jamie Zawinski <jwz@jwz.org>
 * CoCo Version - Copyright (c) 2016 Jamie Cho <jamieleecho@gmail.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 */


#include <coco.h>

/* CMOC does not support linking */
#include "digital.c"
#include "c_stuff.c"
#include "time.c"
#include "UInt32.c"


struct dali_config daliConfig;


int main () {
  daliConfig.width = 256;
  daliConfig.height = 192;
  daliConfig.bitmap = 0xe00;
  daliConfig.max_fps = 30; 
  daliConfig.max_cps = 30; 
  daliConfig.render_state = 0;
  daliConfig.time_mode = HHMMSS;
  daliConfig.date_mode = MMDDYY;
  daliConfig.display_date_p = 0;
  daliConfig.test_hack = 0;

  render_init(&daliConfig);
  memset(daliConfig.bitmap, 0, 6144);
  showGraphicsAddress(7);
  showPmode4(1);

  for (byte ii=32; ii<96; ii++) {
    draw_horizontal_line (&daliConfig, ii + 70 - 32, ii + 70, ii + 64, 1);
    draw_horizontal_line (&daliConfig, ii + 70 - 32 + 1, ii + 70 - 1, ii + 64, 0);
  }

  while(1)
    render_once(&daliConfig); 

  return 0;
}
