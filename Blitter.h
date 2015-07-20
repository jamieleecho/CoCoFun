/*
  Blitter.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Blitting functions
*/


#ifndef _Blitter_h
#define _Blitter_h


#define BREAKOUT_SCORE_NUM_BYTES 4


/** Blit graphics to the screen */
void blitGraphics();

/**
 * Blits graphics located at bitmap to (x, y)
 * @param bitmap[input] location of bitmap data
 * @param x[input] x byte location
 * @param y[input] y pixel location
 */
void blitGraphics2(byte *bitmap, byte x, byte y);


/**
 * Blits numeric text data to (x, y)
 * @param text[input] null terminated text to blit
 * @param x[input] x byte location
 * @param y[input] y pixel location
 */
void blitNumericText(char *text, byte x, byte y);


#endif
