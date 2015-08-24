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
#include "CoCoMisc.c"


/** Loop delay for performing screen fades */
#define BREAKOUT_FADE_DELAY 2500

/** Breakout RGB palette colors */
byte breakoutRGBColorPalette[COCO_NUM_PALETTE_REGISTERS] = {
  63, 36, 46, 52, 25, 18, 38, 56, 7, 54, 40, 34, 11, 41, 9, 0
};
 
/** Breakout CMP palette colors */
byte breakoutCMPColorPalette[COCO_NUM_PALETTE_REGISTERS] = {
  48, 23, 40, 37, 46, 18, 37, 32, 16, 52, 9, 5, 28, 26, 12, 0
};


/** Game title */
char *breakoutTitle = "Breakout";

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
  CoCoMiscPaletteFade(breakoutRGBColorPalette, breakoutCMPColorPalette, 0, 0);

  // Draw and show the do you have an rgb monitor screen...
  hscreen(2);
  BreakoutShowMonitorScreen();  
  
  // Black out the screen
  CoCoMiscPaletteFade(breakoutRGBColorPalette, breakoutCMPColorPalette, 0, 0);
  
  // Initialize objects
  BricksInit();
  BreakoutScoreInit(&breakoutScore);
  BreakoutBallInit();
}


void BreakoutPlay() {
  while(1) {
    BreakoutPlayGame();
  }
}


void BreakoutPlayGame() {
  // Clear previous paddle
  hscreen(2);
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
				  1, 0, 235, 0, 1, breakoutTitle);
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
				  3, 0, 250, 30, 1, "SCORE");
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
				  14, 0, 252, 60, 1, "LIVES");

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
  BlitterDrawGraphics(GrafxDataPaddleData, 4, breakoutPaddlePosition);  

  // Display the screen
  CoCoMiscFadeIn(breakoutRGBColorPalette, breakoutCMPColorPalette, BREAKOUT_FADE_DELAY, 0);

  // Play breakout until we run out of balls
  while(breakoutNumberOfBalls > 0) {
    BreakoutControlPaddle();
    BreakoutBallTick();
  }
  BlitterDrawGraphics(GrafxDataPaddleData, 4, breakoutPaddlePosition);

  // Wait around before starting a new game
  waitkey(0);

  // Make the screen go dark
  CoCoMiscFadeOut(breakoutRGBColorPalette, breakoutCMPColorPalette, BREAKOUT_FADE_DELAY, 0);
}


void BreakoutControlPaddle() {
  byte k = inkey();
  if ((k == 'p') || (k == 'P') || (k ==' '))
	BreakoutPauseGame();

  byte joyX, joyY;
  readJoystick(&joyX, &joyY);
  if (joyY < 21) {
    if (breakoutPaddlePosition > 2)
      breakoutPaddlePosition -= 2;
  } else if (joyY > 42) {
    if (breakoutPaddlePosition < 146)
      breakoutPaddlePosition += 2;
  }
  BlitterDrawGraphics(GrafxDataPaddleData, 4, breakoutPaddlePosition);
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
				  3, 0, 245, 40, 1, buffer);
}


void BreakoutShowMonitorScreen() {
  byte b = 12;
  byte f = 0;
  unsigned y = 0;
  unsigned x = 0;
  BlitterClearScreen(b);

  // Draw the border
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
				  f, b, x, y + 0, 0,
				  "{______________________________________}");

  for(byte ii=8; ii<184; ii+=8) {
	BlitterDrawText(FontDataFontIndex, FontDataFontData,
					f, b, x, y + ii, 0,
					"~");
	BlitterDrawText(FontDataFontIndex, FontDataFontData,
					f, b, 312, y + ii, 0,
					"~");
  }
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
				  f, b, x, y + 184, 0,
				  "[______________________________________]"); 

  // Draw the title
  char buffer[2];
  buffer[1] = 0;
  for(unsigned jj=0; jj<8; jj++) {
	buffer[0] = breakoutTitle[jj];
	unsigned xx = x + (jj * 10) + 120;
	BlitterDrawText2(FontDataFontIndex, FontDataFontData,
					15, xx, y + 11, 2, buffer);
	BlitterDrawText2(FontDataFontIndex, FontDataFontData,
					2, xx-1, y + 10, 2, buffer);
  }
  
  // Draw the message
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
				   15, x + 16, y + 30, 1, "Press ENTER if the rainbow colors");
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
				   15, x + 16, y + 40, 1, "look correct. Otherwise press the ");
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
				   15, x + 16, y + 50, 1, "SPACEBAR until they look right.");

  // Draw the rainbow
  BlitterDrawGraphics(GrafxDataRainbowData, (byte)(x + 57), (byte)(y + 90));

  // Show the screen
  CoCoMiscFadeIn(breakoutRGBColorPalette, breakoutCMPColorPalette, BREAKOUT_FADE_DELAY, b);
  
  for(byte key = waitkey(0); key != 0; key = waitkey(0)) {
	// RGB or Yes we do not have an RGB monitor
	if ((key == 'r') || (key == 'R') || (key == 'y') || (key == 'Y')) {
	  CoCoMiscSetRGBMode(1);
	  break;
	} 

	// Composite or No we do not have an RGB monitor
	if ((key == 'c') || (key == 'C') || (key == 'n') || (key == 'N')) {
	  CoCoMiscSetRGBMode(0);
	  break;
	} 

	// Swap colors
	if (key == ' ') {
	  CoCoMiscSetRGBMode(!CoCoMiscGetRGBMode());
	  CoCoMiscPaletteFade(breakoutRGBColorPalette, breakoutCMPColorPalette, 3, b);
	} 

	// Enter pressed
	if (key == 13) 
	  break;
  }

  CoCoMiscPaletteFade(breakoutRGBColorPalette, breakoutCMPColorPalette, 3, b);
}


void BreakoutPauseGame() {  
  CoCoMiscPaletteFade(breakoutRGBColorPalette, breakoutCMPColorPalette, 2, 0);
  waitkey(0);
  CoCoMiscPaletteFade(breakoutRGBColorPalette, breakoutCMPColorPalette, 3, 0);
}
