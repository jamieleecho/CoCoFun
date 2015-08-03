/*
  CoCoMisc.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Misc CoCo support
*/


#ifndef _CoCoMisc_h
#define _CoCoMisc_h


/** CoCo base palette register */
byte *cocoPaletteBaseReg = (byte *)0xffb0;

/** Number of palette registers */
#define COCO_NUM_PALETTE_REGISTERS 16

/** CoCo high speed control register */
byte *cocoBorderRegister = (byte *)0xff9a;

/** CoCo slow speed control register */
byte *cocoSlowSpeedReg = (byte *)0xffd8;

/** CoCo high speed control register */
byte *cocoHighSpeedReg = (byte *)0xffd9;


/**
 * @param value[in] largest number random() will return
 * @return a random value on [1, value]
 */
int random(int value) {
  asm {
    pshs u
    ldd value
    jsr $B4F4
    jsr $BF1F
    jsr $B3ED
    std value
    puls u
  }
  return value;
}


/**
 * Reads the joystick.
 *
 * @param x[out] x position
 * @param y[out] y position
 */
void readJoystick(byte *x, byte *y) {
  asm {
    PSHS U
    JSR	[$A00A]
    PULS U
    LDD	$15a
    ldx x
    ldy y
    sta ,x
    stb ,y
  }
}


/**
 * Sets the CoCo Palette, either making it darker or brighter.
 * @param palette[in] Base palette
 * @param level[in] 0->2 means darker where 0 is darkest
 *                     3 means no change
 *                  4->6 means lighter where 1 is brighest
 * @param border[in] [0, 15] slot to set border to
 */
void CoCoMiscPaletteFade(byte *palette, byte level, byte border) {
  switch(level) {
  case 0:
	memset(cocoPaletteBaseReg, 0, COCO_NUM_PALETTE_REGISTERS);
	break;
  case 1:
	for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
	  cocoPaletteBaseReg[ii] = palette[ii] >> 3;
	break;
  case 2:
	for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
	  cocoPaletteBaseReg[ii] = palette[ii] & 0x38;
	break;
  case 3:
	memcpy(cocoPaletteBaseReg, palette, COCO_NUM_PALETTE_REGISTERS);
	break;
  case 4:
	for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
	  cocoPaletteBaseReg[ii] = palette[ii] | 0x7;
	break;
  case 5:
	for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
	  cocoPaletteBaseReg[ii] = 0x38 | palette[ii];
	break;
  case 6:
	memset(cocoPaletteBaseReg, 0x3f, COCO_NUM_PALETTE_REGISTERS);
	break;
  }
  *cocoBorderRegister = cocoPaletteBaseReg[border];
}


/**
 * Delay loop.
 * @param count[in] number of times to loop.
 */
void CoCoMiscDelay(unsigned int count) {
  for(int ii=0; ii<count; ii++);
}


/**
 * Fade into the palette
 * @param palette[in] palette colors
 * @param count[in] delay time
 * @param border[in] border color slot
 */
void CoCoMiscFadeIn(byte *palette, unsigned int count, byte border) {
  for(byte ii=0; ii<4; ii++) {
    CoCoMiscPaletteFade(palette, ii, border);
	CoCoMiscDelay(count);
  }
}


/**
 * Fade out of the palette
 * @param palette[in] palette colors
 * @param count[in] delay time
 * @param border[in] border color slot
 */
void CoCoMiscFadeOut(byte *palette, unsigned int count, byte border) {
  for(byte ii=4; ii>0; ii--) {
    CoCoMiscPaletteFade(palette, ii-1, border);
	CoCoMiscDelay(count);
  }
}


#endif
