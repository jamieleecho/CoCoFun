/*
  Blitter.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Blitting functions
*/


#ifndef _Blitter_h
#define _Blitter_h


#define BREAKOUT_SCORE_NUM_BYTES 4


/** Must be invoked before other Blitter routines */
void BlitterInit();


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


/**
 * @param fontIndex[in] index that maps ASCII [32, 127] to offsets into fontData
 * @param fontData[in] array containing glyph data. Each glyph consists of
 *                     width and height bytes that specify the glyph size in bits.
 *                     The remaining data is the 1-bit data for the font packed in
 *                     bytes.
 * @param foreground[in] Foreground color on [0, 15]
 * @param background[in] Background color on [0, 15]
 * @param x[in] X location in pixels
 * @param y[in] Y location in pixels
 * @param glyphSpacing[in] spacing between glyphs in pixels
 * @param text[in] Text to output
 */
void BlitterDrawText(int *fontIndex, byte *fontData,
					 byte foreground, byte background,
					 int x, int y,
					 byte glyphSpacing,
					 char *text);


#endif
