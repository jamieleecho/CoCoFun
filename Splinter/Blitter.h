/*
  Blitter.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Blitting functions
*/


#ifndef _Blitter_h
#define _Blitter_h


#include <coco.h>


/** Must be invoked before other Blitter routines */
void BlitterInit();


/**
 * Blits graphics located at bitmap to (x, y)
 * @param bitmap[input] location of bitmap data
 * @param x[input] x byte location
 * @param y[input] y pixel location
 */
void BlitterDrawGraphics(byte *bitmap, byte x, byte y);


/**
 * Blits numeric text data to (x, y)
 * @param text[input] null terminated text to blit
 * @param x[input] x byte location
 * @param y[input] y pixel location
 */
void BlitterDrawNumericText(const char *text, byte x, byte y);


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
void BlitterDrawText(unsigned *fontIndex, byte *fontData,
		     byte foreground, byte background,
		     unsigned x, unsigned y,
		     byte glyphSpacing,
		     const char *text);

/**
 * @param fontIndex[in] index that maps ASCII [32, 127] to offsets into fontData
 * @param fontData[in] array containing glyph data. Each glyph consists of
 *                     width and height bytes that specify the glyph size in bits.
 *                     The remaining data is the 1-bit data for the font packed in
 *                     bytes.
 * @param foreground[in] Foreground color on [0, 15]
 * @param x[in] X location in pixels
 * @param y[in] Y location in pixels
 * @param glyphSpacing[in] spacing between glyphs in pixels
 * @param text[in] Text to output
 */
void BlitterDrawText2(unsigned *fontIndex, byte *fontData,
		      byte foreground, unsigned x, unsigned y,
		      byte glyphSpacing, const char *text);

/**
 * Clears the screen to the given color
 * @param color[in] color to set
 */
void BlitterClearScreen(byte color);


/**
 * Fills a rectangle with the given color
 * @param x[in] starting x coordinate
 * @param y[in] starting y coordinate
 * @param width[in] width in pixels
 * @param height[in] height in pixels
 */
void BlitterFillRectangle(unsigned x, unsigned y, unsigned width, unsigned height,
			  byte color);


/**
 * Gets rectanguler bitmap data on screen from (x, y)->(x+width-1, y+height-1)
 * in a format suitable BlitterDrawGraphics.
 *
 * @param buffer[out] buffer that has at least 1 + ((x + 1) * y) bytes.
 * @param x[in] x coordinate in bytes
 * @param y[in] y coordinate in pixels
 * @param width[in] width in bytes
 * @param height[in] height in pixels
 */
void BlitterGet(byte *buffer, byte x, byte y, byte width, byte height);


/**
 * Initializes GrafxDataNumberData
 * param buffer[in] pointer to GrafxDataNumberData
 */
void BlitterInitGrafxDataNumberData(byte *buffer);

#endif
