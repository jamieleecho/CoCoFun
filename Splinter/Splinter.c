/*
  Splinter.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  The Splinter program was originally written in Assembly Language
  back around 1989 or so.
*/

#define VERSION "Splinter 1.2"

#include <coco.h>
#include <cmoc.h>

#include "Blitter.h"
#include "Bricks.h"
#include "CoCoMisc.h"
#include "Sound.h"
#include "Splinter.h"
#include "SplinterBall.h"
#include "SplinterScore.h"
#include "Vector2d.h"

#include "FontData.h"
#include "GrafxData.h"


/** Loop delay for performing screen fades */
#define SPLINTER_FADE_DELAY 3000


/** Last direction of the paddle */
byte SplinterLastDirection;

/** First direction of the paddle when both keys are detected */
byte SplinterFirstDirection;

/** Score during a splinter game */
SplinterScore splinterScore;

/** Splinter RGB palette colors */
byte splinterRGBColorPalette[COCO_NUM_PALETTE_REGISTERS] = {
  63, 36, 46, 52, 25, 18, 38, 56, 7, 54, 40, 34, 11, 41, 9, 0
};
 
/** Splinter CMP palette colors */
byte splinterCMPColorPalette[COCO_NUM_PALETTE_REGISTERS] = {
  48, 23, 40, 37, 46, 18, 37, 32, 16, 52, 9, 5, 28, 26, 12, 0
};

/** Game title */
const char *splinterTitle = "Splinter";

/** last score drawn by the splinter routine */
char splinterLastDrawnScore[7];


int main() {
  SplinterInit();
  SplinterPlay();

  return 0;
}


void SplinterInit() {
  // High speed mode
  initCoCoSupport();
  setHighSpeed(1);
  BlitterInit();

  //while(1) {
  //  printf("%d %d\n", CoCoMiscReadJoystick(0, 2), CoCoMiscReadJoystick(1, 2));
  //}

  // Black out the screen
  CoCoMiscPaletteFade(splinterRGBColorPalette, splinterCMPColorPalette, 0, 0);
  hscreen(2);
  SoundInit();
  BlitterInitGrafxDataNumberData(GrafxDataNumberData);
  FixedPointInitialize();

  // Draw and show the do you have an rgb monitor screen...
  SplinterShowMonitorScreen();  
  
  // Black out the screen
  CoCoMiscPaletteFade(splinterRGBColorPalette, splinterCMPColorPalette, 0, 0);
  
  // Initialize objects
  BricksInit();
  SplinterScoreInit(&splinterScore);
  SplinterBallInit();
}


void SplinterPlay() {
  while(1) {
    SplinterShowTitleScreen();
    SplinterPlayGame();
  }
}


void SplinterPlayGame() {
  // Clear previous paddle
  BlitterClearScreen(0);
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
		   15, 235, 1, 2, splinterTitle);
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
		   2, 234, 0, 2, splinterTitle);
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  3, 0, 247, 30, 2, "SCORE");
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  14, 0, 249, 60, 2, "BALLS");

  // Reset data structures
  memset(splinterLastDrawnScore, 0, sizeof(splinterLastDrawnScore));
  SplinterLastDirection = SplinterLastDirectionNone;
  SplinterFirstDirection = SplinterLastDirectionNone;
  SplinterScoreReset(&splinterScore);
  SplinterBallReset(TRUE);
  BricksReset();
  splinterNumberOfBalls = 5;
  splinterPaddlePosition = 72;

  // Draw the screen
  SplinterDrawScore();  
  SplinterBallDrawCount();
  BricksRefresh();
  BlitterDrawGraphics(GrafxDataPaddleData, 4, splinterPaddlePosition);  

  // Display the screen
  CoCoMiscFadeIn(splinterRGBColorPalette, splinterCMPColorPalette,
		 SPLINTER_FADE_DELAY, 0);

  // Play splinter until we run out of balls
  while(splinterNumberOfBalls > 0) {
    if (SplinterControlPaddle())
      break;
    SplinterBallTick();
  }
  BlitterDrawGraphics(GrafxDataPaddleData, 4, splinterPaddlePosition);

  // Tell the user that the game has ended
  SplinterShowGameOver();

  // Make the screen go dark
  CoCoMiscFadeOut(splinterRGBColorPalette, splinterCMPColorPalette,
		  SPLINTER_FADE_DELAY, 0);
}


byte SplinterControlPaddle() {
  // Look for 'p' to pause the game
  *CoCoMiscKeyboardScanOutput = 0xfe;
  byte pausePressed = ((*CoCoMiscKeyboardScanInput & 0x4) == 0x0);
  if (pausePressed)
    SplinterPauseGame();
    
  // Look for the 'break' to quit the game
  *CoCoMiscKeyboardScanOutput = 0xfb;
  byte breakPressed = ((*CoCoMiscKeyboardScanInput & 0x40) == 0x0);
  if (breakPressed && SplinterQuitGame())
    return TRUE;

  // Look for up arrow
  *CoCoMiscKeyboardScanOutput = 0xf7;
  byte upPressed = ((*CoCoMiscKeyboardScanInput & 0x8) == 0x0);

  // Look for down arrow
  *CoCoMiscKeyboardScanOutput = 0xef;
  byte downPressed = ((*CoCoMiscKeyboardScanInput & 0x8) == 0x0);

  // Figure out the direction
  if (upPressed && downPressed) {
    if (SplinterFirstDirection == SplinterLastDirectionUp)
      SplinterLastDirection = SplinterLastDirectionDown;
    else if (SplinterFirstDirection == SplinterLastDirectionDown)
      SplinterLastDirection = SplinterLastDirectionUp;
    else
      SplinterLastDirection = SplinterLastDirectionNone; 
  } else if (upPressed) {
    SplinterLastDirection = SplinterLastDirectionUp;
    SplinterFirstDirection = SplinterLastDirectionUp;
  } else if (downPressed) {
    SplinterLastDirection = SplinterLastDirectionDown;
    SplinterFirstDirection = SplinterLastDirectionDown;
  } else {
    SplinterLastDirection = SplinterLastDirectionNone;
    SplinterFirstDirection = SplinterLastDirectionNone;
  }

  // Move the paddle
  if (SplinterLastDirection == SplinterLastDirectionUp) {
    if (splinterPaddlePosition > 0) {
      BlitterFillRectangle(8, splinterPaddlePosition + 36, 4, 3, 0);      
      splinterPaddlePosition -= 3;
    }
  } else if (SplinterLastDirection == SplinterLastDirectionDown) {
    if (splinterPaddlePosition < 148) {
      BlitterFillRectangle(8, splinterPaddlePosition, 4, 3, 0);      
      splinterPaddlePosition += 3;
    }
  }

  // Draw the paddle
  BlitterDrawGraphics(GrafxDataPaddleData, 4, splinterPaddlePosition);

  return FALSE;
}


void SplinterDrawScore() {
  char buffer[7], buffer2[7];
  SplinterScoreFormat(&splinterScore, buffer);
  memcpy(buffer2, buffer, sizeof(buffer));

  // Avoid drawing parts of the score that have not changed
  byte ii;
  for(ii=0; ii<sizeof(splinterLastDrawnScore); ii++)
    if (splinterLastDrawnScore[ii] != buffer2[ii])
      break;

  // Score has not changed, do not draw
  if (ii >= sizeof(splinterLastDrawnScore))
    return;

  byte jj;
  for(jj=sizeof(splinterLastDrawnScore)-1; jj > ii; jj--)
    if (splinterLastDrawnScore[jj-1] != buffer2[jj-1])
      break;
  buffer2[jj] = 0;

  BlitterDrawNumericText(buffer2 + ii, 121 + ii*5, 40);
  memcpy(splinterLastDrawnScore, buffer, sizeof(splinterLastDrawnScore));
}


void SplinterDrawBorderedScreen(byte f, byte b) {
  SplinterDrawDialogBox(40, 24, f, b);
}


void SplinterShowMonitorScreen() {
  byte b = 0;
  byte f = 14;
  unsigned y = 0;
  unsigned x = 0;

  // Draw the border
  SplinterDrawBorderedScreen(7, b);

  // Draw the title
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
		   15, x + 126, y + 11, 2, splinterTitle);
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
		   2, x + 125, y + 10, 2, splinterTitle);
  
  // Draw the message
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  f, b, x + 23, y + 38, 1, "Press ENTER if the rainbow colors");
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  f, b, x + 23, y + 48, 1, "look correct. Otherwise press the ");
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  f, b, x + 23, y + 58, 1, "SPACEBAR until they look right.");

  // Draw the rainbow
  BlitterDrawGraphics(GrafxDataRainbowData, (byte)(x + 57), (byte)(y + 110));

  // Show the screen
  CoCoMiscFadeIn(splinterRGBColorPalette, splinterCMPColorPalette,
		 SPLINTER_FADE_DELAY, b);
  
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
      CoCoMiscPaletteFade(splinterRGBColorPalette, splinterCMPColorPalette, 3, b);
    } 
    
    // Enter pressed
    if (key == 13) 
      break;
  }
  
  CoCoMiscFadeOut(splinterRGBColorPalette, splinterCMPColorPalette,
		  SPLINTER_FADE_DELAY, b);
}


void SplinterPauseGame() {  
  // Location in character coordinates
  unsigned ww = 16, hh = 5;
  byte b = 0, f = 14;
  SplinterDrawDialogBox(ww, hh, f, b);
  
  // Locations in pixel coordinates
  unsigned xx = (40 - ww)/2, yy = (24 - hh)/2;
  unsigned x = xx * 8, y = yy * 8;
  
  // Draw the game paused message
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  f, b, 112, y + 16, 1, "Game Paused");

  // Wait for the user to response
  waitkey(0);

  // Restore the screen
  SplinterEraseDialogBox(ww, hh, b);
  SplinterRefresh();
}


byte SplinterQuitGame() {  
  // Sizes in character coordinaes
  unsigned ww = 34, hh = 5;
  byte b = 0, f = 14;
  SplinterDrawDialogBox(ww, hh, f, b);
  
  // Locations in pixel coordinates
  unsigned xx = (40 - ww)/2, yy = (27 - hh)/2;
  unsigned x = xx * 8, y = yy * 8;
  
  // Draw the quit message
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  f, b, 40, y - 6, 1, "Are you sure you want to stop?");
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  f, b, 120, y + 6, 1, "Press Y or N");

  // Wait for the user to response
  byte shouldQuit = FALSE;
  do {
    byte c = inkey();
    if ((c == 'n') || (c == 'N'))
      break;
    if ((c == 'y') || (c == 'Y')) {
      shouldQuit = TRUE;
      break;
    }
  } while(TRUE);

  // Restore the screen
  SplinterEraseDialogBox(ww, hh, b);
  SplinterRefresh();

  return shouldQuit;
}


void SplinterShowTitleScreen() {
  BlitterClearScreen(0);
  
  // Draw the border
  byte b = 0, f = 14;    
  SplinterDrawBorderedScreen(7, b);

  // Draw the title
  unsigned int x = 0, y = 0;
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
		   15, x + 112, y + 11, 2, VERSION);
  BlitterDrawText2(FontDataFontIndex, FontDataFontData,
		   2, x + 113, y + 10, 2, VERSION);
  
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

  CoCoMiscFadeIn(splinterRGBColorPalette, splinterCMPColorPalette,
		 SPLINTER_FADE_DELAY, 0);
  waitkey(0);

  CoCoMiscFadeOut(splinterRGBColorPalette, splinterCMPColorPalette,
		  SPLINTER_FADE_DELAY, b);
}


void SplinterDrawDialogBox(unsigned ww, unsigned hh, byte f, byte b) {
  unsigned xx = (40 - ww)/2, yy = (24 - hh)/2;

  // Locations in pixel coordinates
  unsigned x = xx * 8, y = yy * 8;
  unsigned w = ww * 8 - 8, h = hh * 8 - 8;
  f = 14;
  b = 0;

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


void SplinterEraseDialogBox(unsigned ww, unsigned hh, byte b) {
  unsigned xx = (40 - ww)/2;
  unsigned yy = (24 - hh)/2;

  // Locations in pixel coordinates
  unsigned x = xx * 8;
  unsigned y = yy * 8;
  unsigned w = ww * 8;
  unsigned h = hh * 8;

  // White out the selected area
  BlitterFillRectangle(x, y, w, h, b);
}


void SplinterShowGameOver() {
  // Location in character coordinates
  unsigned ww = 16, hh = 5;
  byte b = 0, f = 14;
  SplinterDrawDialogBox(ww, hh, f, b);

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


void SplinterRefresh() {
  SplinterBallRefresh();
  SplinterBallDrawCount();
  BricksRefresh();
  BlitterDrawGraphics(GrafxDataPaddleData, 4, splinterPaddlePosition);  
}
