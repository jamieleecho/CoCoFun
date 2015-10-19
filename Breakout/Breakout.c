/*
  Breakout.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  The Breakout program was originally written in Assembly Language
  back around 1989 or so.
*/

#pragma org 0x3000
#include <cmoc.h>
#include <coco.h>

#include "Bricks.h"
#include "BreakoutBall.h"
#include "BreakoutScore.h"
#include "Blitter.h"
#include "Breakout.h"
#include "CoCoMisc.h"
#include "Sound.h"

#include "GrafxData.c"
#include "FontData.c"
#include "Bricks.c"
#include "BreakoutBall.c"
#include "BreakoutScore.c"
#include "Blitter.c"
#include "CoCoMisc.c"
#include "Sound.c"
#include "FixedPoint.c"


/** Loop delay for performing screen fades */
#define BREAKOUT_FADE_DELAY 3000

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

/** last score drawn by the breakout routine */
char breakoutLastDrawnScore[7];


int main() {
#if 1
  FixedPoint a, b, c, d, e, f;
  FixedPointMake(&a, 32, 0x8000);
  FixedPointMake(&b, 2, 0x0000);
  FixedPointMul(&c, &a, &b);
  printf("%x.%x\n", a.Whole, a.Fraction);
  printf("%x.%x\n", b.Whole, b.Fraction);
  printf("%x.%x\n\n", c.Whole, c.Fraction);

  FixedPointMake(&a, 0, 0x0);
  FixedPointMake(&b, 0, 0x1);
  FixedPointSub(&c, &a, &b);
  printf("%x.%x\n", a.Whole, a.Fraction);
  printf("%x.%x\n", b.Whole, b.Fraction);
  printf("%x.%x\n\n", c.Whole, c.Fraction);

  FixedPointMake(&a, -1, 0x0);
  FixedPointNegate(&b, &a);
  FixedPointNegate(&b, &b);
  FixedPointNegate(&b, &b);
  printf("%x.%x\n", a.Whole, a.Fraction);
  printf("%x.%x\n\n", b.Whole, b.Fraction);

  FixedPointMake(&a, -1, 0x0);
  FixedPointMake(&b, 32, 0x4000);
  FixedPointMul(&c, &a, &b);
  printf("%x.%x\n", a.Whole, a.Fraction);
  printf("%x.%x\n", b.Whole, b.Fraction);
  printf("%x.%x\n\n", c.Whole, c.Fraction);

  FixedPointMake(&a, -1, 0x0);
  FixedPointMake(&b, -1, 0xffff);
  FixedPointAdd(&c, &a, &b);
  FixedPointNegate(&d, &a);
  FixedPointNegate(&e, &b);
  FixedPointNegate(&f, &c);
  printf("%x.%x\n", a.Whole, a.Fraction);
  printf("%x.%x\n", b.Whole, b.Fraction);
  printf("%x.%x\n", c.Whole, c.Fraction);
  printf("%x.%x\n", d.Whole, d.Fraction);
  printf("%x.%x\n", e.Whole, e.Fraction);
  printf("%x.%x\n\n", f.Whole, f.Fraction);
  waitkey(0);

  FixedPointMake(&a, 0x7fff, 0x0);
  FixedPointMake(&b, 0x1, 0x0);
  FixedPointDiv(&c, &a, &b);
  printf("%x.%x\n", a.Whole, a.Fraction);
  printf("%x.%x\n", b.Whole, b.Fraction);
  printf("%x.%x\n", c.Whole, c.Fraction);
  waitkey(0);

#endif

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
  hscreen(2);
  SoundInit();
  BlitterInitGrafxDataNumberData(GrafxDataNumberData);

  // Draw and show the do you have an rgb monitor screen...
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
    BreakoutShowTitleScreen();
    BreakoutPlayGame();
  }
}


void BreakoutPlayGame() {
  // Clear previous paddle
  BlitterClearScreen(0);
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
		   15, 231, 1, 2, breakoutTitle);
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
		   2, 230, 0, 2, breakoutTitle);
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  3, 0, 247, 30, 2, "SCORE");
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  14, 0, 249, 60, 2, "LIVES");

  // Reset data structures
  memset(breakoutLastDrawnScore, 0, sizeof(breakoutLastDrawnScore));
  BreakoutLastDirection = BreakoutLastDirectionNone;
  BreakoutFirstDirection = BreakoutLastDirectionNone;
  BreakoutScoreReset(&breakoutScore);
  BreakoutBallReset();
  BricksReset();
  breakoutNumberOfBalls = 5;
  breakoutPaddlePosition = 72;

  // Draw the screen
  BreakoutDrawScore();  
  BreakoutBallDrawCount();
  BricksRefresh();
  BlitterDrawGraphics(GrafxDataPaddleData, 4, breakoutPaddlePosition);  

  // Display the screen
  CoCoMiscFadeIn(breakoutRGBColorPalette, breakoutCMPColorPalette,
		 BREAKOUT_FADE_DELAY, 0);

  // Play breakout until we run out of balls
  while(breakoutNumberOfBalls > 0) {
    BreakoutControlPaddle();
    BreakoutBallTick();
  }
  BlitterDrawGraphics(GrafxDataPaddleData, 4, breakoutPaddlePosition);

  // Tell the user that the game has ended
  BreakoutShowGameOver();

  // Make the screen go dark
  CoCoMiscFadeOut(breakoutRGBColorPalette, breakoutCMPColorPalette,
		  BREAKOUT_FADE_DELAY, 0);
}


void BreakoutControlPaddle() {
  // Look for 'p' to pause the game
  *CoCoMiscKeyboardScanOutput = 0xfe;
  byte pausePressed = ((*CoCoMiscKeyboardScanInput & 0x4) == 0x0);
  if (pausePressed)
    BreakoutPauseGame();
    
  // Look for up arrow
  *CoCoMiscKeyboardScanOutput = 0xf7;
  byte upPressed = ((*CoCoMiscKeyboardScanInput & 0x8) == 0x0);

  // Look for down arrow
  *CoCoMiscKeyboardScanOutput = 0xef;
  byte downPressed = ((*CoCoMiscKeyboardScanInput & 0x8) == 0x0);

  // Figure out the direction
  if (upPressed && downPressed) {
    if (BreakoutFirstDirection == BreakoutLastDirectionUp)
      BreakoutLastDirection = BreakoutLastDirectionDown;
    else if (BreakoutFirstDirection == BreakoutLastDirectionDown)
      BreakoutLastDirection = BreakoutLastDirectionUp;
    else
      BreakoutLastDirection = BreakoutLastDirectionNone; 
  } else if (upPressed) {
    BreakoutLastDirection = BreakoutLastDirectionUp;
    BreakoutFirstDirection = BreakoutLastDirectionUp;
  } else if (downPressed) {
    BreakoutLastDirection = BreakoutLastDirectionDown;
    BreakoutFirstDirection = BreakoutLastDirectionDown;
  } else {
    BreakoutLastDirection = BreakoutLastDirectionNone;
    BreakoutFirstDirection = BreakoutLastDirectionNone;
  }

  // Move the paddle
  if (BreakoutLastDirection == BreakoutLastDirectionUp) {
    if (breakoutPaddlePosition > 0) {
      BlitterFillRectangle(8, breakoutPaddlePosition + 36, 4, 3, 0);      
      breakoutPaddlePosition -= 3;
    }
  } else if (BreakoutLastDirection == BreakoutLastDirectionDown) {
    if (breakoutPaddlePosition < 148) {
      BlitterFillRectangle(8, breakoutPaddlePosition, 4, 3, 0);      
      breakoutPaddlePosition += 3;
    }
  }

  // Draw the paddle
  BlitterDrawGraphics(GrafxDataPaddleData, 4, breakoutPaddlePosition);
}


void BreakoutDrawScore() {
  char buffer[7], buffer2[7];
  BreakoutScoreFormat(&breakoutScore, buffer);
  memcpy(buffer2, buffer, sizeof(buffer));

  // Avoid drawing parts of the score that have not changed
  byte ii;
  for(ii=0; ii<sizeof(breakoutLastDrawnScore); ii++)
    if (breakoutLastDrawnScore[ii] != buffer2[ii])
      break;

  // Score has not changed, do not draw
  if (ii >= sizeof(breakoutLastDrawnScore))
    return;

  byte jj;
  for(jj=sizeof(breakoutLastDrawnScore)-1; jj > ii; jj--)
    if (breakoutLastDrawnScore[jj-1] != buffer2[jj-1])
      break;
  buffer2[jj] = 0;

  BlitterDrawNumericText(buffer2 + ii, 121 + ii*5, 40);
  memcpy(breakoutLastDrawnScore, buffer, sizeof(breakoutLastDrawnScore));
}


void BreakoutDrawBorderedScreen(byte f, byte b) {
  BreakoutDrawDialogBox(40, 24, f, b);
}


void BreakoutShowMonitorScreen() {
  byte b = 0;
  byte f = 14;
  unsigned y = 0;
  unsigned x = 0;

  // Draw the border
  BreakoutDrawBorderedScreen(7, b);

  // Draw the title
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
		   15, x + 122, y + 11, 2, breakoutTitle);
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
		   2, x + 121, y + 10, 2, breakoutTitle);
  
  // Draw the message
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  f, b, x + 28, y + 38, 1, "Press ENTER if the rainbow colors");
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  f, b, x + 28, y + 48, 1, "look correct. Otherwise press the ");
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  f, b, x + 28, y + 58, 1, "SPACEBAR until they look right.");

  // Draw the rainbow
  BlitterDrawGraphics(GrafxDataRainbowData, (byte)(x + 57), (byte)(y + 110));

  // Show the screen
  CoCoMiscFadeIn(breakoutRGBColorPalette, breakoutCMPColorPalette,
		 BREAKOUT_FADE_DELAY, b);
  
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
  
  CoCoMiscFadeOut(breakoutRGBColorPalette, breakoutCMPColorPalette,
		  BREAKOUT_FADE_DELAY, b);
}


void BreakoutPauseGame() {  
  // Location in character coordinates
  unsigned ww = 16, hh = 5;
  byte b = 0, f = 14;
  BreakoutDrawDialogBox(ww, hh, f, b);
  
  // Locations in pixel coordinates
  unsigned xx = (40 - ww)/2, yy = (24 - hh)/2;
  unsigned x = xx * 8, y = yy * 8;
  
  // Draw the game paused message
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  f, b, 112, y + 16, 1, "Game Paused");

  // Wait for the user to response
  waitkey(0);

  // Restore the screen
  BreakoutEraseDialogBox(ww, hh, b);
  BreakoutRefresh();
}


void BreakoutShowTitleScreen() {
  BlitterClearScreen(0);
  
  // Draw the border
  byte b = 0, f = 14;    
  BreakoutDrawBorderedScreen(7, b);

  // Draw the title
  unsigned int x = 0, y = 0;
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
		   15, x + 122, y + 11, 2, breakoutTitle);
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
		   2, x + 121, y + 10, 2, breakoutTitle);
  
  y = 2;
  BlitterFillRectangle(60, y + 25, 200, 131, f);
  BlitterFillRectangle(61, y + 26, 198, 129, b);
  BlitterDrawGraphics(GrafxDataPaddleData, 33, (byte)y + 70);
  BlitterDrawGraphics(GrafxDataBallData, 53, (byte)y + 89);
  for(int ii=0; ii<128; ii += 8) {
    int yy = y + 27 + ii;
    BlitterFillRectangle(156, yy, 4, 7, 7);
    BlitterFillRectangle(156 + 1, yy + 1, 2, 5, 1);

    BlitterFillRectangle(171, yy, 4, 7, 7);
    BlitterFillRectangle(171 + 1, yy + 1, 2, 5, 3);

    BlitterFillRectangle(186, yy, 4, 7, 7);
    BlitterFillRectangle(186 + 1, yy + 1, 2, 5, 9);

    BlitterFillRectangle(201, yy, 4, 7, 7);
    BlitterFillRectangle(201 + 1, yy + 1, 2, 5, 5);

    BlitterFillRectangle(216, yy, 4, 7, 7);
    BlitterFillRectangle(216 + 1, yy + 1, 2, 5, 14);

    BlitterFillRectangle(231, yy, 4, 7, 7);
    BlitterFillRectangle(231 + 1, yy + 1, 2, 5, 14);

    BlitterFillRectangle(246, yy, 4, 7, 7);
    BlitterFillRectangle(246 + 1, yy + 1, 2, 5, 10);
  }

  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  f, b, 74, 168, 1, "Press Any Key to Play");

  CoCoMiscFadeIn(breakoutRGBColorPalette, breakoutCMPColorPalette,
		 BREAKOUT_FADE_DELAY, 0);
  waitkey(0);

  CoCoMiscFadeOut(breakoutRGBColorPalette, breakoutCMPColorPalette,
		  BREAKOUT_FADE_DELAY, b);
}


void BreakoutDrawDialogBox(unsigned ww, unsigned hh, byte f, byte b) {
  unsigned xx = (40 - ww)/2, yy = (24 - hh)/2;

  // Locations in pixel coordinates
  unsigned x = xx * 8, y = yy * 8;
  unsigned w = ww * 8 - 8, h = hh * 8 - 8;
  byte f = 14, b = 0;

  // White out the selected area
  BlitterFillRectangle(x, y, w + 8, h + 8, b);

  // Draw corners
  BlitterDrawText(FontDataFontIndex, FontDataFontData, 7, b, x, y, 0, "{");
  BlitterDrawText(FontDataFontIndex, FontDataFontData, 7, b, x + w, y, 0, "}");
  BlitterDrawText(FontDataFontIndex, FontDataFontData, 7, b, x, y + h, 0, "[");
  BlitterDrawText(FontDataFontIndex, FontDataFontData, 7, b, x + w, y + h, 0, "]");

  // Draw Horizontal Edges
  for(unsigned ii = 8; ii < w; ii += 8) {
    BlitterDrawText(FontDataFontIndex, FontDataFontData,
		    7, b, ii + x, y, 0, "_");
    BlitterDrawText(FontDataFontIndex, FontDataFontData,
		    7, b, ii + x, y + h, 0, "_");
  }

  // Draw Vertical Edges
  for(byte ii=8; ii<h; ii+=8) {
    BlitterDrawText(FontDataFontIndex, FontDataFontData,
		    7, b, x, ii + y, 0, "~");
    BlitterDrawText(FontDataFontIndex, FontDataFontData,
		    7, b, x + w, ii + y, 0, "~");
  }
}


void BreakoutEraseDialogBox(unsigned ww, unsigned hh, byte b) {
  unsigned xx = (40 - ww)/2, yy = (24 - hh)/2;

  // Locations in pixel coordinates
  unsigned x = xx * 8, y = yy * 8;
  unsigned w = ww * 8 - 8, h = hh * 8 - 8;

  // White out the selected area
  BlitterFillRectangle(x, y, w + 8, h + 8, b);
}


void BreakoutShowGameOver() {
  // Location in character coordinates
  unsigned ww = 16, hh = 5;
  byte b = 0, f = 14;
  BreakoutDrawDialogBox(ww, hh, f, b);

  // Locations in pixel coordinates
  unsigned xx = (40 - ww)/2, yy = (24 - hh)/2;
  unsigned x = xx * 8, y = yy * 8;

  // Draw the game over message
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  f, b, 120, y + 16, 1, "Game Over");
    
  // Wait around a little so that the user does not make the game over disappear
  // too quickly
  CoCoMiscDelay(30000);

  waitkey(0);
}


void BreakoutRefresh() {
  BreakoutBallRefresh();
  BricksRefresh();
  BlitterDrawGraphics(GrafxDataPaddleData, 4, breakoutPaddlePosition);  
}
