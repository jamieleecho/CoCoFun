/*
  CoCoMisc.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Misc CoCo support
*/


#ifndef _CoCoMisc_h
#define _CoCoMisc_h


/**
 * @param value[in] largest number random() will return
 * @return a random value on [1, value]
 */
int random(int value);


/**
 * Reads the joystick.
 *
 * @param x[out] x position
 * @param y[out] y position
 */
void readJoystick(byte *x, byte *y);


/**
 * Sets the CoCo Palette, either making it darker or brighter.
 * @param rgbPalette[in] Base rgb palette
 * @param cmpPalette[in] Base cmp palette
 * @param level[in] 0->2 means darker where 0 is darkest
 *                     3 means no change
 *                  4->6 means lighter where 1 is brighest
 * @param border[in] [0, 15] slot to set border to
 */
void CoCoMiscPaletteFade(byte *rgbPalette, byte *cmpPalette, byte level, byte border);


/**
 * Delay loop.
 * @param count[in] number of times to loop.
 */
void CoCoMiscDelay(unsigned int count);


/**
 * Fade into the palette
 * @param rgbPalette[in] rgb palette colors
 * @param cmpPalette[in] cmp palette colors
 * @param count[in] delay time
 * @param border[in] border color slot
 */
void CoCoMiscFadeIn(byte *rgbPalette, byte *cmpPalette, unsigned int count, byte border);


/**
 * Fade out of the palette
 * @param rgbPalette[in] rgb palette colors
 * @param cmpPalette[in] cmp palette colors
 * @param count[in] delay time
 * @param border[in] border color slot
 */
void CoCoMiscFadeOut(byte *rgbPalette, byte *cmpPalette, unsigned int count, byte border);


/**
 * Set the monitor mode for the software
 * @param mode[in] 0 for CMP and != 0 for RGB mode
 */
void CoCoMiscSetRGBMode(byte mode);


/**
 * @return monitor mode
 */
byte CoCoMiscGetRGBMode();


#endif
