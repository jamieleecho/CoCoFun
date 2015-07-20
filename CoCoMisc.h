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


#endif
