/*
  CoCoMisc.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Misc CoCo support
*/


#include "CoCoMisc.h"


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

/** Whether or not we are in RGB mode */
byte _cocoRGBMode = 1;


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


void CoCoMiscPaletteFade(byte *rgbPalette, byte *cmpPalette, byte level, byte border) {
  byte *palette = (_cocoRGBMode) ? rgbPalette : cmpPalette;

  switch(level) {
  case 0:
	memset(cocoPaletteBaseReg, 0, COCO_NUM_PALETTE_REGISTERS);
	break;
  case 1:
	if (_cocoRGBMode)
	  for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
		cocoPaletteBaseReg[ii] = palette[ii] >> 3;
	else
	  for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
		cocoPaletteBaseReg[ii] = palette[ii] & 0x1f;
	break;
  case 2:
	if (_cocoRGBMode)
	  for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
		cocoPaletteBaseReg[ii] = palette[ii] & 0x38;
	else
	  for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
		cocoPaletteBaseReg[ii] = palette[ii] & 0x2f;
	break;
  case 3:
	for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
	  memcpy(cocoPaletteBaseReg, palette, COCO_NUM_PALETTE_REGISTERS);
	break;
  case 4:
	if (_cocoRGBMode)
	  for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
		cocoPaletteBaseReg[ii] = palette[ii] | 0x7;
	else
	  for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
		cocoPaletteBaseReg[ii] = palette[ii] | 0x10;
	break;
  case 5:
	if (_cocoRGBMode)
	  for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
		cocoPaletteBaseReg[ii] = 0x38 | palette[ii];
	else
	  for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
		cocoPaletteBaseReg[ii] = palette[ii] | 0x20;
	break;
  case 6:
	memset(cocoPaletteBaseReg, 0x3f, COCO_NUM_PALETTE_REGISTERS);
	break;
  }
  *cocoBorderRegister = cocoPaletteBaseReg[border];
}


void CoCoMiscDelay(unsigned int count) {
  for(int ii=0; ii<count; ii++);
}


void CoCoMiscFadeIn(byte *rgbPalette, byte *cmpPalette, unsigned int count, byte border) {
  for(byte ii=0; ii<4; ii++) {
    CoCoMiscPaletteFade(rgbPalette, cmpPalette, ii, border);
	CoCoMiscDelay(count);
  }
}


void CoCoMiscFadeOut(byte *rgbPalette, byte *cmpPalette, unsigned int count, byte border) {
  for(byte ii=4; ii>0; ii--) {
    CoCoMiscPaletteFade(rgbPalette, cmpPalette, ii-1, border);
	CoCoMiscDelay(count);
  }
}


void CoCoMiscSetRGBMode(byte mode) { _cocoRGBMode = mode; }


byte CoCoMiscGetRGBMode() { return _cocoRGBMode; }
