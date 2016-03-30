/*
  CoCoMisc.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Misc CoCo support
*/


#ifndef _CoCoMisc_h
#define _CoCoMisc_h

#include <coco.h>


/** CoCo base palette register */
#define cocoPaletteBaseReg ((byte *)0xffb0)

/** CoCo high speed control register */
#define cocoBorderRegister ((byte *)0xff9a)

/** CoCo slow speed control register */
#define cocoSlowSpeedReg ((byte *)0xffd8)

/** CoCo high speed control register */
#define cocoHighSpeedReg ((byte *)0xffd9)

/** Register to write when reading the keyboard */
#define CoCoMiscKeyboardScanOutput ((byte *)0xff02)

/** Register to read when reading the keyboard */
#define CoCoMiscKeyboardScanInput ((byte *)0xff00)

/** PIA2 Control Register A*/
#define CoCoMiscPIA2ControlRegisterA ((byte *)0xff21)

/** PIA2 Data Register A*/
#define CoCoMiscPIA2DataRegisterA ((byte *)0xff20)

/** PIA2 Control Register B*/
#define CoCoMiscPIA2ControlRegisterB ((byte *)0xff23)

/** PIA2 Data Register B*/
#define CoCoMiscPIA2DataRegisterB ((byte *)0xff22)

/** PIA1 Control Register A*/
#define CoCoMiscPIA1ControlRegisterA ((byte *)0xff01)

/** PIA1 Data Register A*/
#define CoCoMiscPIA1DataRegisterA ((byte *)0xff00)

/** PIA1 Control Register B*/
#define CoCoMiscPIA1ControlRegisterB ((byte *)0xff03)

/** PIA1 Data Register B*/
#define CoCoMiscPIA1DataRegisterB ((byte *)0xff02)

/** IRQ Jump Vector address */
#define CoCoMiscIRQJumpVector ((byte *)0x10c)

/** FIRQ Jump Vector address */
#define CoCoMiscFIRQJumpVector ((byte *)0x10f)

/** GIME init 0 */
#define CoCoMiscGIMEInit0 ((byte *)0xff90)

/** GIME init 1*/
#define CoCoMiscGIMEInit1 ((byte *)0xff91)

/** GIME IRQ enable register */
#define CoCoMiscGIMEIRQEnable ((byte *)0xff92)

/** GIME FIRQ enable register */
#define CoCoMiscGIMEFIRQEnable ((byte *)0xff93)

/** GIME Timer */
#define CoCoMiscGIMETimer ((unsigned *)0xff94)

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


/**
 * Reads the given joystick axis. Assumes hi-speed mode is active.
 * @param axis[in] 0 = Right X, 1 = Left Y, 2 = Left X, 3 = Left Y
 * @param scale[in] Meaning depends on whether the X or Y axis is being read.
 *                  For X: 1 = 256, 2 = 320, 3 = 640
 *                  For Y: 1 = 192, 2 = 200, 3 = 225
 */
int CoCoMiscReadJoystick(byte axis, byte scale);

#endif
