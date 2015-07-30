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
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
				  1, 0, 235, 0, "BREAKOUT");
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
				  3, 0, 250, 30, "SCORE");
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
				  14, 0, 252, 60, "LIVES");

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
  blitGraphics2(GrafxDataPaddleData, 4, breakoutPaddlePosition);

  // Wait around before starting a new game
  waitkey(0);
}


void BreakoutControlPaddle() {
  byte joyX, joyY;
  readJoystick(&joyX, &joyY);
  if (joyY < 21) {
    if (breakoutPaddlePosition > 2)
      breakoutPaddlePosition -= 2;
  } else if (joyY > 42) {
    if (breakoutPaddlePosition < 146)
      breakoutPaddlePosition += 2;
  }
  blitGraphics2(GrafxDataPaddleData, 4, breakoutPaddlePosition);
}


void BreakoutDrawScore() {
  char buffer[13];
  BreakoutScoreFormat(&breakoutScore, buffer);

  // Hack - add white space to compensate for "1"s that are
  // smaller than the other numbers
  byte ii;
  byte jj = 6;
  for(byte ii=0; ii<6; ii++)
	if (buffer[ii] == '1')
		buffer[jj++] = ' ';
  buffer[jj] = 0;
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
				  3, 0, 245, 40, buffer);
}


