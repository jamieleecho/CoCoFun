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

#pragma org 0x2800

#include <cmoc.h>
#include "c_stuff.h"
#include "time.h"

#define extern
#include "xdaliclock.h"
#undef extern

#define DO_TEXTURE	/* Create LUMINANCE_ALPHA data instead of a bitmap */
#define BIGENDIAN	/* Bit ordering if creating a single-bit bitmap */

typedef unsigned byte POS;
typedef unsigned char BOOL;

/* static int use_builtin_font; */
struct raw_number {
  unsigned char *bits;
  POS width, height;
};

#define FONT(X)								\
 static struct raw_number numbers_ ## X [] = {			\
  { zero  ## X ## _bits,  zero ## X ## _width,  zero ## X ## _height }, \
  { one   ## X ## _bits,   one ## X ## _width,   one ## X ## _height }, \
  { two   ## X ## _bits,   two ## X ## _width,   two ## X ## _height }, \
  { three ## X ## _bits, three ## X ## _width, three ## X ## _height }, \
  { four  ## X ## _bits,  four ## X ## _width,  four ## X ## _height }, \
  { five  ## X ## _bits,  five ## X ## _width,  five ## X ## _height }, \
  { six   ## X ## _bits,   six ## X ## _width,   six ## X ## _height }, \
  { seven ## X ## _bits, seven ## X ## _width, seven ## X ## _height }, \
  { eight ## X ## _bits, eight ## X ## _width, eight ## X ## _height }, \
  { nine  ## X ## _bits,  nine ## X ## _width,  nine ## X ## _height }, \
  { colon ## X ## _bits, colon ## X ## _width, colon ## X ## _height }, \
  { slash ## X ## _bits, slash ## X ## _width, slash ## X ## _height }, \
  { 0, 0, 0 }								\
}

#define static

# include "zeroF.xbm"
# include "oneF.xbm"
# include "twoF.xbm"
# include "threeF.xbm"
# include "fourF.xbm"
# include "fiveF.xbm"
# include "sixF.xbm"
# include "sevenF.xbm"
# include "eightF.xbm"
# include "nineF.xbm"
# include "colonF.xbm"
# include "slashF.xbm"
FONT(F);

#undef static

struct raw_number * all_numbers[] = {
 numbers_F,
};

#undef countof
#define countof(x) (sizeof((x))/sizeof(*(x)))

/* Number of horizontal segments/line.  Enlarge this if you are trying
   to use a font that is too "curvy" for XDaliClock to cope with.
   This code was sent to me by Dan Wallach <c169-bg@auriga.berkeley.edu>.
   I'm highly opposed to ever using statically-sized arrays, but I don't
   really feel like hacking on this code enough to clean it up.
 */
#ifndef MAX_SEGS_PER_LINE
# define MAX_SEGS_PER_LINE 3
#endif

struct scanline {
  POS left[MAX_SEGS_PER_LINE], right[MAX_SEGS_PER_LINE];
};

struct frame {
  struct scanline scanlines [1]; /* scanlines are contiguous here */
};


/* The runtime settings (some initialized from system prefs, but changable.)
 */
struct render_state {
  UInt32 last_secs;
  unsigned int current_xsecs;
  int char_width, char_height, colon_width;
  struct frame *base_frames [12];	/* all digits */
  struct frame *orig_frames [8];	/* what was there */
  int           orig_digits [8];	/* what was there */
  struct frame *current_frames [8];	/* current intermediate animation */
  struct frame *target_frames [8];	/* where we are going */
  int           target_digits [8];	/* where we are going */
  struct frame *empty_frame;
  struct frame *empty_colon;
};


struct frame *
make_empty_frame (int width, int height)
{
  int size = sizeof (struct frame) + (sizeof (struct scanline) * height);
  struct frame *frame;
  int x, y;

  frame = (struct frame *) calloc (size, 1);
  if (!frame) {
    printf("out of mem!\n");
    abort();
  }
  for (y = 0; y < height; y++)
    for (x = 0; x < MAX_SEGS_PER_LINE; x++)
      frame->scanlines[y].left [x] = frame->scanlines[y].right [x] = (POS)(width / 2);
  return frame;
}


struct frame *
copy_frame (struct dali_config *c, struct frame *from)
{
  struct render_state *state = c->render_state;
  int height = state->char_height;
  int size = sizeof (struct frame) + (sizeof (struct scanline) * height);
  struct frame *to = (struct frame *) calloc (size, 1);
  int y;
  for (y = 0; y < height; y++) {
    memcpy(&(to->scanlines[y]), &(from->scanlines[y]), sizeof((to->scanlines[y])));  /* copies the whole struct */
   // for (int seg = 0; seg < MAX_SEGS_PER_LINE; seg++)
     // printf("(%d %d) ", from->scanlines[y].left[seg], from->scanlines[y].right[seg]);
  }
  //printf("\n");
  return to;
}


struct frame *
number_to_frame (unsigned char *bits, int width, int height)
{
  int x, y;
  struct frame *frame;
  POS *left, *right;

  frame = make_empty_frame (width, height);

  for (y = 0; y < height; y++)
    {
      int seg, end;
      x = 0;
# define GETBIT(bits,x,y) \
         (!! ((bits) [((y) * ((width+7) >> 3)) + ((x) >> 3)] \
              & (1 << ((x) & 7))))

      left = frame->scanlines[y].left;
      right = frame->scanlines[y].right;

      for (seg = 0; seg < MAX_SEGS_PER_LINE; seg++)
        left [seg] = right [seg] = (POS)(width / 2);

      for (seg = 0; seg < MAX_SEGS_PER_LINE; seg++)
        {
          for (; x < width; x++) {
            if (GETBIT (bits, x, y)) break;
          }
          if (x == width) break;
          left [seg] = (POS)x;
          for (; x < width; x++)
            if (! GETBIT (bits, x, y)) break;
          right [seg] = (POS)x;
          //printf("(%d %d) ", left[seg], right[seg]);
        }
        //printf("\n");

      for (; x < width; x++)
        if (GETBIT (bits, x, y))
          {
            printf ("%s: font is too curvy\n", progname);
            /* Increase MAX_SEGS_PER_LINE and recompile. */
            exit (-1);
          }

      /* If there were any segments on this line, then replicate the last
         one out to the end of the line.  If it's blank, leave it alone,
         meaning it will be a 0-pixel-wide line down the middle.
       */
      end = seg;
      if (end > 0)
        for (; seg < MAX_SEGS_PER_LINE; seg++)
          {
            left [seg] = left [end-1];
            right [seg] = right [end-1];
          }

# undef GETBIT
    }

  return frame;
}


int
pick_font_size (struct dali_config *c, unsigned int *w_ret, unsigned int *h_ret)
{
  int nn, cc;
  int f;
  unsigned int w, h;
  unsigned int ww = (w_ret ? *w_ret : c->width);
  unsigned int hh = (h_ret ? *h_ret : c->height);

  switch (c->time_mode)
    {
    case SS:     nn = 2; cc = 0; break;
    case HHMM:   nn = 4; cc = 1; break;
    case HHMMSS: nn = 6; cc = 2; break;
    default:   
      printf("Bad time mode\n");
      abort();
      break;
    }

  for (f = 0; f < countof(all_numbers); f++)
    {
      w = ((all_numbers[f][0].width * nn) +
           (all_numbers[f][10].width * cc));
      h = all_numbers[f][0].height;
      if (w <= ww && h <= hh) break;
    }
  if (f >= countof(all_numbers))
    f = countof(all_numbers)-1;

  w += 2;  /* Leave a single pixel margin in the overall bitmap */
  h += 2;

  w = ((w + 7) / 8) * 8;  /* round up to byte */

  if (w_ret) *w_ret = w;
  if (h_ret) *h_ret = h;
  return f;
}


void
init_numbers (struct dali_config *c)
{
  struct render_state *state = c->render_state;
  int i;
  struct raw_number *raw;

  int size = pick_font_size (c, (unsigned int *)NULL, (unsigned int *)NULL);
  if (size >= countof(all_numbers)) {
    printf("bad size\n");
    abort();
  }
  raw = all_numbers[size];

  state->char_width  = raw[0].width;
  state->char_height = raw[0].height;
  state->colon_width = raw[10].width;

  state->empty_frame = make_empty_frame (raw[0].width,  raw[0].height);
  state->empty_colon = make_empty_frame (raw[10].width, raw[10].height);

  for (i = 0; i < countof(state->base_frames); i++) {
    // printf("*** %d ***\n", i);
    state->base_frames [i] =
      number_to_frame (raw[i].bits, raw[i].width, raw[i].height);
    // waitkey(0);
  }

  memset (state->orig_frames,    0, sizeof(state->orig_frames));
  memset (state->current_frames, 0, sizeof(state->current_frames));
  memset (state->target_frames,  0, sizeof(state->target_frames));

  for (i = 0; i < countof(state->current_frames); i++) {
    int colonic_p = (i == 2 || i == 5);
    int cw = raw[colonic_p ? 10 : 0].width;
    int ch = raw[0].height;
    state->orig_frames[i]    = make_empty_frame (cw, ch);
    state->current_frames[i] = make_empty_frame (cw, ch);
    state->target_frames[i]  = make_empty_frame (cw, ch);
  }

  for (i = 0; i < countof(state->target_digits); i++)
    state->orig_digits[i] = state->target_digits[i] = -1;

  if (! c->bitmap)
    c->bitmap = (unsigned char *)calloc (1, c->height * (c->width << 3));

  if (! c->bitmap) {
    printf("Out of memory\n");
    abort();
  }
}


void
free_numbers (struct dali_config *c)
{
  struct render_state *state = c->render_state;
  int i;
# define FREEIF(x) do { if ((x)) { free((x)); (x) = 0; } } while (0)
# define FREELOOP(x) do { \
    for (i = 0; i < countof ((x)); i++) FREEIF ((x)[i]); } while (0)

  FREELOOP (state->base_frames);
  FREELOOP (state->orig_frames);
  FREELOOP (state->current_frames);
  FREELOOP (state->target_frames);
  FREEIF (state->empty_frame);
  FREEIF (state->empty_colon);

# undef FREELOOP
# undef FREEIF
}


void sub32(unsigned *ahi, unsigned *alo, unsigned bhi, unsigned blo,
           unsigned chi, unsigned clo) {
  asm {
    leax clo
    neg 3,x
    bcs sub32Com2
    neg 2,x
    bcs sub32Com1
    neg 1,x
    bcs sub32Com0
    neg ,x
    bra sub32Done

sub32Com2:
    com 2,x
sub32Com1:
    com 1,x
sub32Com0:
    com ,x
sub32Done:

     
  }
}
              

void
fill_target_digits (struct dali_config *c, UInt32 *time)
{
  struct render_state *state = c->render_state;
  struct tm *tm = localtime (time);

  int i;
  int h = tm->tm_hour;
  int m = tm->tm_min;
  int s = tm->tm_sec;
  int D = tm->tm_mday;
  int M = tm->tm_mon + 1;
  int Y = tm->tm_year % 100;

  int twelve_p = c->twelve_hour_p;

  if (!(c->countdown.Hi || c->countdown.Lo))
    {
      UInt32 delta, div, ss;
      UInt32 zero = UInt32Init(0, 0);
      UInt32 sixty = UInt32Init(0, 60);
      UInt32 hundred = UInt32Init(0, 100);
      UInt32 thirtysixhundred = UInt32Init(0, 60 * 60);
      UInt32 mm, hh, tt;
      UInt32Sub(&delta, &(c->countdown), time);
      if (UInt32LessThan(&delta, &zero)) UInt32Sub(&delta, &zero,  &delta);
      UInt32Mod(&div, &ss, &delta, &sixty);
      UInt32Div(&mm, &delta, &sixty);
      UInt32Mod(&hh, &mm, &mm, &sixty);

      UInt32Div(&hh, &delta, &thirtysixhundred);
      UInt32Mod(&tt, &hh, &hh, &hundred);
      twelve_p = 0;
    }

  if (twelve_p) 
    {
      if (h > 12) { h -= 12; }
      else if (h == 0) { h = 12; }
    }

  for (i = 0; i < countof(state->target_digits); i++)
    state->target_digits[i] = -1;

  if (c->test_hack)
    {
      int a = (c->test_hack >= '0' && c->test_hack <= '9'
               ? c->test_hack - '0'
               : -1);
      state->target_digits [0] = a;
      state->target_digits [1] = a;
      state->target_digits [2] = 10;
      state->target_digits [3] = a;
      state->target_digits [4] = a;
      state->target_digits [5] = 10;
      state->target_digits [6] = a;
      state->target_digits [7] = a;
      c->test_hack = 0;
    }
  else if (!c->display_date_p)
    {
      switch (c->time_mode)
        {
        case SS:
          state->target_digits[0] = (s / 10);
          state->target_digits[1] = (s % 10);
          break;
        case HHMM:
          state->target_digits[0] = (h / 10);
          state->target_digits[1] = (h % 10);
          state->target_digits[2] = 10;		/* colon */
          state->target_digits[3] = (m / 10);
          state->target_digits[4] = (m % 10);
          if (twelve_p && state->target_digits[0] == 0)
            state->target_digits[0] = -1;
          break;
        case HHMMSS:
          state->target_digits[0] = (h / 10);
          state->target_digits[1] = (h % 10);
          state->target_digits[2] = 10;		/* colon */
          state->target_digits[3] = (m / 10);
          state->target_digits[4] = (m % 10);
          state->target_digits[5] = 10;		/* colon */
          state->target_digits[6] = (s / 10);
          state->target_digits[7] = (s % 10);
          if (twelve_p && state->target_digits[0] == 0)
            state->target_digits[0] = -1;
          break;
        default: 
          printf("Bad format\n");
          abort();
        }
    }
  else	/* date mode */
    {
      switch (c->date_mode) 
        {
        case MMDDYY:
          switch (c->time_mode) {
          case SS:
            state->target_digits[0] = (D / 10);
            state->target_digits[1] = (D % 10);
            break;
          case HHMM:
            state->target_digits[0] = (M / 10);
            state->target_digits[1] = (M % 10);
            state->target_digits[2] = 11;		/* dash */
            state->target_digits[3] = (D / 10);
            state->target_digits[4] = (D % 10);
            break;
          case HHMMSS:
            state->target_digits[0] = (M / 10);
            state->target_digits[1] = (M % 10);
            state->target_digits[2] = 11;		/* dash */
            state->target_digits[3] = (D / 10);
            state->target_digits[4] = (D % 10);
            state->target_digits[5] = 11;		/* dash */
            state->target_digits[6] = (Y / 10);
            state->target_digits[7] = (Y % 10);
            break;
          default:
            printf("Bad format\n");
            abort();
          }
          break;
        case DDMMYY:
          switch (c->time_mode) {
          case SS:
            state->target_digits[0] = (D / 10);
            state->target_digits[1] = (D % 10);
            break;
          case HHMM:
            state->target_digits[0] = (D / 10);
            state->target_digits[1] = (D % 10);
            state->target_digits[2] = 11;		/* dash */
            state->target_digits[3] = (M / 10);
            state->target_digits[4] = (M % 10);
            break;
          case HHMMSS:
            state->target_digits[0] = (D / 10);
            state->target_digits[1] = (D % 10);
            state->target_digits[2] = 11;		/* dash */
            state->target_digits[3] = (M / 10);
            state->target_digits[4] = (M % 10);
            state->target_digits[5] = 11;		/* dash */
            state->target_digits[6] = (Y / 10);
            state->target_digits[7] = (Y % 10);
            break;
          default:
            printf("Bad format\n");
            abort();
          }
          break;
        case YYMMDD:
          switch (c->time_mode) {
          case SS:
            state->target_digits[0] = (D / 10);
            state->target_digits[1] = (D % 10);
            break;
          case HHMM:
            state->target_digits[0] = (M / 10);
            state->target_digits[1] = (M % 10);
            state->target_digits[2] = 11;		/* dash */
            state->target_digits[3] = (D / 10);
            state->target_digits[4] = (D % 10);
            break;
          case HHMMSS:
            state->target_digits[0] = (Y / 10);
            state->target_digits[1] = (Y % 10);
            state->target_digits[2] = 11;		/* dash */
            state->target_digits[3] = (M / 10);
            state->target_digits[4] = (M % 10);
            state->target_digits[5] = 11;		/* dash */
            state->target_digits[6] = (D / 10);
            state->target_digits[7] = (D % 10);
            break;
          default:
            printf("Bad format\n");
            abort();
          }
          break;
        default:
          printf("Bad format\n");
          abort();
        }
    }
}

void
draw_horizontal_line (struct dali_config *c, byte x1, byte x2, byte y, BOOL black_p)
{
  unsigned char *scanline;

  if (x1 == x2) return;
  if (y > c->height) return;
  if (x1 > c->width) x1 = (byte)c->width;
  if (x2 > c->width) x2 = (byte)c->width;
  if (x1 > x2)
    {
      byte swap = x1;
      x1 = x2;
      x2 = swap;
    }

  scanline = c->bitmap + (y * (c->width >> 3));
  byte xx1 = x1 & 7;
  byte xx2 = x2 & 0xf8;
  if ((x1 & 0xf8) + 8 <= x2) {
    if (xx1) {
      if (black_p) {
        scanline[x1>>3] |= (byte)(0xff >> xx1);
      } else {
        scanline[x1>>3] &= ~(byte)(0xff >> xx1);
      }
    }

    x1 = x1 + (xx1 ? (8 - xx1) : 0);
    if (black_p)
      for (; x1 < xx2; x1+=8) {
        scanline[x1>>3] = 0xff;
      }
    else
      for (; x1 < xx2; x1+=8) {
        scanline[x1>>3] = 0x00;
      }

    byte delta = (8 - (x2 - xx2)) & 0x7;
    byte xval = 0;
    if (delta) {
      if (black_p) {
        scanline[x1>>3] |= (byte)(0xff << delta);
      } else {
        byte val = ~(byte)(0xff << delta);
        scanline[x1>>3] &= val;
      }
    }
  } else {
    byte delta = (8 - (x2 - xx2)) & 0x7;
    byte val; 
    if (black_p) {
      val = scanline[x1>>3] | (byte)(0xff >> xx1);
      scanline[x1>>3] = val & (byte)(0xff << delta);
    } else {
      val = ~((byte)(0xff >> xx1) & (byte)(0xff << delta));
      scanline[x1>>3] &= val;
    }
  }
}


byte
draw_frame (struct dali_config *c, struct frame *frame, byte x, byte y, int colonic_p)
{
  struct render_state *state = c->render_state;
  byte px, py;
  byte cw = (byte)(colonic_p ? state->colon_width : state->char_width);

  for (py = 0; py < state->char_height; py++)
    {
      struct scanline *line = &frame->scanlines [py];
      byte last_right = 0;

      for (px = 0; px < MAX_SEGS_PER_LINE; px++)
        {
          if (px > 0 &&
              (line->left[px] == line->right[px] ||
               (line->left [px] == line->left [px-1] &&
                line->right[px] == line->right[px-1])))
            continue;

          /* Erase the line between the last segment and this segment.
           */
          draw_horizontal_line (c,
                                x + last_right,
                                x + line->left [px],
                                y + py,
                                0);

          /* Draw the line of this segment.
           */
          draw_horizontal_line (c,
                                x + line->left [px],
                                x + line->right[px],
                                y + py,
                                1);

          last_right = line->right[px];
        }

      /* Erase the line between the last segment and the right edge.
       */
      draw_horizontal_line (c,
                            x + last_right,
                            x + cw,
                            y + py,
                            0);
    }
  return cw;
}


void draw_clock (struct dali_config *c);

void
start_sequence (struct dali_config *c, UInt32 *time)
{
  struct render_state *state = c->render_state;
  int i;

  /* Move the (old) current_frames into the (new) orig_frames,
     since that's what's on the screen now. 
     Frames are freed as they expire out of orig_frames.
   */
  for (i = 0; i < countof (state->current_frames); i++)
    {
      if (state->orig_frames[i]) 
        free (state->orig_frames[i]);
      state->orig_frames[i]    = state->current_frames[i];
      state->current_frames[i] = state->target_frames[i];
      state->target_frames[i]  = 0;
      state->orig_digits[i]    = state->target_digits[i];
    }

  /* generate new target_digits */
  fill_target_digits (c, time);

  /* Fill the (new) target_frames from the (new) target_digits. */
  for (i = 0; i < countof (state->target_frames); i++)
    {
      int colonic_p = (i == 2 || i == 5);
      state->target_frames[i] =
        copy_frame (c,
                    (state->target_digits[i] == -1
                     ? (colonic_p ? state->empty_colon : state->empty_frame)
                     : state->base_frames[state->target_digits[i]]));
    }

  /* Render the current frame. */
  draw_clock (c);
}


void
one_step (struct dali_config *c,
          struct frame *orig_frame,
          struct frame *current_frame,
          struct frame *target_frame,
          unsigned int xsecs)
{
  struct render_state *state = c->render_state;
  struct scanline *orig   =    &orig_frame->scanlines [0];
  struct scanline *curr   = &current_frame->scanlines [0];
  struct scanline *target =  &target_frame->scanlines [0];
  int i = 0, x;
  for (i = 0; i < state->char_height; i++)
    {
# define STEP(field) \
         (curr->field = (POS)(orig->field \
                         + (((int) (target->field - orig->field)) \
                            * (int) xsecs / 60)))

      for (x = 0; x < MAX_SEGS_PER_LINE; x++)
        {
          STEP (left [x]);
          STEP (right[x]);
          //curr->left[x] = target->left[x];
          //curr->right[x] = target->right[x];
        }
      orig++;
      curr++;
      target++;
# undef STEP
    }
}


void
tick_sequence (struct dali_config *c)
{
  struct render_state *state = c->render_state;
  int i;

  struct timeval now;
  struct timezone tzp;
  gettimeofday (&now, &tzp);
  UInt32 secs;
  memcpy(&secs, &(now.tv_sec), sizeof(secs));
  unsigned long xsecs = now.tv_xsec;

  if (!state->last_secs.Hi || !state->last_secs.Lo) {
    memcpy(&(state->last_secs), &secs, sizeof(&(state->last_secs)));
  }
  else //  if (UInt32Equals(&secs, &(state->last_secs))) xxx
    {
      /* End of the animation sequence; fill target_frames with the
         digits of the current time. */
      start_sequence (c, &secs);
      memcpy(&(state->last_secs), &secs, sizeof(&(state->last_secs)));
    }

  /* Linger for about 1/10th second at the end of each cycle. */
  state->current_xsecs = xsecs;
  xsecs *= 12;
  xsecs /= 10;
  if (xsecs > 60) xsecs = 60;

  /* Construct current_frames by interpolating between
     orig_frames and target_frames. */
  for (i = 0; i < countof (state->current_frames); i++)
    one_step (c,
              state->orig_frames[i],
              state->current_frames[i],
              state->target_frames[i],
              (unsigned int) xsecs);
  state->current_xsecs = (state->current_xsecs + 1) % 60; // (unsigned int) xsecs;
}


void
compute_left_offset (struct dali_config *c)
{
  struct render_state *state = c->render_state;

  /* left_offset is so that the clock can be centered in the window
     when the leftmost digit is hidden (in 12-hour mode when the hour
     is 1-9).  When the hour rolls over from 9 to 10, or from 12 to 1,
     we animate the transition to keep the digits centered.
   */
  if (state->target_digits[0] == -1 &&		/* Fading in to no digit */
      state->orig_digits[1] == -1)
    c->left_offset = state->char_width / 2;
  else if (state->target_digits[0] != -1 &&	/* Fading in to a digit */
           state->orig_digits[1] == -1)
    c->left_offset = 0;
  else if (state->orig_digits[0] != -1 &&	/* Fading out from digit */
           state->target_digits[1] == -1)
    c->left_offset = 0;
  else if (state->orig_digits[0] != -1 &&	/* Fading out from no digit */
           state->target_digits[1] == -1)
    c->left_offset = state->char_width / 2;
  else if (state->orig_digits[0] == -1 &&	/* Anim no digit to digit. */
           state->target_digits[0] != -1)
    c->left_offset = state->char_width * (60 - state->current_xsecs) / 120;
  else if (state->orig_digits[0] != -1 &&	/* Anim digit to no digit. */
           state->target_digits[0] == -1)
    c->left_offset = state->char_width * state->current_xsecs / 120;
  else if (state->target_digits[0] == -1)	/* No anim, no digit. */
    c->left_offset = state->char_width / 2;
  else						/* No anim, digit. */
    c->left_offset = 0;
}


void
draw_clock (struct dali_config *c)
{
  struct render_state *state = c->render_state;
  byte x, y, i, nn, cc;

  compute_left_offset (c);

  switch (c->time_mode)
    {
    case SS:     nn = 2; cc = 0; break;
    case HHMM:   nn = 4; cc = 1; break;
    case HHMMSS: nn = 6; cc = 2; break;
    default: 
      printf("Bad format\n");
      abort();
      break;
    }

  x = y = 0;
  for (i = 0; i < nn+cc; i++) 
    {
      byte colonic_p = (i == 2 || i == 5);
      x += draw_frame (c, state->current_frames[i], x, y, colonic_p);
    }
}


void
render_init (struct dali_config *c)
{
  if (c->render_state) {
    printf("bad render state\n");
    abort();
  }
  c->render_state = (struct render_state *)
    calloc (1, sizeof (struct render_state));
  init_numbers (c);
}

void
render_free (struct dali_config *c)
{
  free_numbers (c);
  if (c->render_state) free (c->render_state);
  if (c->bitmap) free (c->bitmap);
  c->render_state = 0;
  c->bitmap = 0;
}

void
render_once (struct dali_config *c)
{
  if (! c->render_state) {
    printf("bad render_state\n");
    abort();
  }
  if (! c->bitmap) {
    printf("bad bitmap\n");
    abort();
  }
  tick_sequence (c);
  draw_clock (c);
}


void
render_bitmap_size (struct dali_config *c, 
                    unsigned int *w_ret, unsigned int *h_ret,
                    unsigned int *w2_ret, unsigned int *h2_ret)
{
  pick_font_size (c, w_ret, h_ret);
  if (w2_ret) *w2_ret = *w_ret;
  if (w2_ret) *h2_ret = *h_ret;
}
