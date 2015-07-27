/*
  Breakout.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  The Breakout program was originally written in Assembly Language
  back around 1989 or so.
*/

#pragma org 0x3000
#include <cmoc.h>
#include <legacy.h>
#include <coco.h>

#include "Bricks.h"
#include "BreakoutBall.h"
#include "BreakoutScore.h"
#include "Blitter.h"
#include "Breakout.h"
#include "CoCoMisc.h"

#include "GrafxData.c"
#include "FontData.c"
#include "Bricks.c"
#include "BreakoutBall.c"
#include "BreakoutScore.c"
#include "Blitter.c"


/** Breakout palette colors */
byte breakoutColorPalette[COCO_NUM_PALETTE_REGISTERS] = {
  63, 36, 46, 52, 25, 18, 38, 56,
  7, 54, 40, 34, 10, 2, 9, 0
};
 

/** Position of the paddle */
byte breakoutPaddlePosition = 72;
    
/** current number of balls */
char breakoutNumberOfBalls;


int main() {
  BreakoutInit();
  BreakoutPlay();

  return 0;
}


void BreakoutInit() {
  // High speed mode
  initCoCoSupport();
  setHighSpeed(1);
  BlitterInit();
  

  // Black out the screen
  memset(cocoPaletteBaseReg, 0, COCO_NUM_PALETTE_REGISTERS);

  // Show the graphics screen
  hscreen(2);

  // Initialize objects
  BricksInit();
  BreakoutScoreInit(&breakoutScore);
  BreakoutBallInit();

  // Set final palette
  memcpy(cocoPaletteBaseReg, breakoutColorPalette, COCO_NUM_PALETTE_REGISTERS);
  *cocoBorderRegister = 0xff;

  // Draw some text
  while(1) {
	for(byte jj=0; jj<16; jj++) {
	  for(byte ii=0;ii<16; ii++) {
		BlitterBlitText(FontDataFontIndex, FontDataFontData,
						ii, jj, 0, 0, "0123456789 Hello World");
	  }
	}
  }
  while(1);
}


void BreakoutPlay() {
  while(1) {
    BreakoutPlayGame();
  }
}


void BreakoutPlayGame() {
  // Clear previous paddle
  hscreen(2);
  *cocoBorderRegister = 0xff;

  // Reset data structures
  BreakoutScoreReset(&breakoutScore);
  BreakoutBallReset();
  BricksReset();
  breakoutNumberOfBalls = 9;
  breakoutPaddlePosition = 72;

  // Draw the screen
  BreakoutDrawScore();  
  BreakoutBallDrawCount();
  BricksDrawBricks();

  // Play breakout until we run out of balls
  while(breakoutNumberOfBalls > 0) {
    BreakoutControlPaddle();
    BreakoutBallTick();
  }

  // Wait around before starting a new game
  waitkey(0);
}


void BreakoutControlPaddle() {
  byte joyX, joyY;
  readJoystick(&joyX, &joyY);
  if (joyY < 20) {
    if (breakoutPaddlePosition > 2)
      breakoutPaddlePosition -= 2;
  } else if (joyY > 40) {
    if (breakoutPaddlePosition < 146)
      breakoutPaddlePosition += 2;
  }
  blitGraphics2(GrafxDataPaddleData, 4, breakoutPaddlePosition);
}


void BreakoutDrawScore() {
  char buffer[7];
  BreakoutScoreFormat(&breakoutScore, buffer);
  blitNumericText(buffer, 0x78, 0x50);
}


