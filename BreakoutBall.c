/*
  BreakoutBall.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Logic for controlling the ball in Breakout.
*/


#ifndef _BreakoutBall_c
#define _BreakoutBall_c


#include "BreakoutBall.h"
#include "Blitter.h"


/** Flag - if 1, then the ball was missed, let it hit left side of screen */
byte breakoutBallWasMissed = 0;


void BreakoutBallReset() {
  breakoutBallPositionX = 40;
  breakoutBallPositionY = 3;
  breakoutBallIncrementX = -1;
  breakoutBallIncrementY = 1;
  breakoutBallSlopeX = 1;
  breakoutBallSlopeY = 5;
  breakoutBallCounterX = breakoutBallSlopeX;
  breakoutBallCounterY = breakoutBallSlopeY;
  breakoutBallWasMissed = 0;
}


void BreakoutBallInit() {
  BreakoutBallReset();
}


void BreakoutBallDrawCount() {
  char buffer[4];
  buffer[0] = breakoutNumberOfBalls + '0';
  buffer[1] = ' ';
  buffer[2] = 0;
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
				  14, 0, 270, 70, 1, buffer);
}


void BreakoutBallMiss() {
  // Decrease the number of balls
  breakoutNumberOfBalls--;
  BreakoutBallDrawCount();
  // sound(1, 1);

  // Erase the displayed ball
  BlitterFillRectangle(breakoutBallPositionX << 1, breakoutBallPositionY, 12, 11, 0);

  // Reset the ball location
  BreakoutBallReset();
}


void BreakoutBallCheckBrickCollision(byte lineBrickXPos, byte *lineBrickYPositions) {
  // We are a little generous with hit detection here to reduce half destroyed bricks
  byte b1 = breakoutBallPositionY;
  byte bend = b1 + 6;
  byte numHit = 0;
  for(byte ii=0; ii<brickYPositionsSz; ii++) {    
    byte p1 = lineBrickYPositions[ii];
    byte pend = p1 + 14;
    
    if (((p1 <= b1) && (pend >= b1))
	|| ((b1 <= p1) && (bend >= p1))) {
      BlitterFillRectangle(lineBrickXPos << 1, lineBrickYPositions[ii], 12, 11, 0);
      lineBrickYPositions[ii] = 0xff;
      breakoutBallSlopeX = (byte)random(5);
      breakoutBallSlopeY = (byte)random(5);
      if (breakoutBallSlopeX & 1)
		breakoutBallIncrementX = -1;
      breakoutBallCounterX = breakoutBallSlopeX;
      breakoutBallCounterY = breakoutBallSlopeY;
      BreakoutScoreIncrement(&breakoutScore, 10);
      numHit++;
      
      // Remove the brick - if none left, reset the level
      BricksRemove();
      if (BricksAllGone()) {
	BlitterFillRectangle(breakoutBallPositionX << 1, breakoutBallPositionY,
			     12, 11, 0);
	BricksReset();
	BreakoutBallReset();	
	BricksRefresh();
	return;
      }
    }
  }

  // Play a sound if we hit any bricks
  if (numHit > 0) {
    BreakoutDrawScore();      
    sound(255, 1);
  }
}


void BreakoutBallTick() {
  byte oldX = breakoutBallPositionX;
  byte oldY = breakoutBallPositionY;

  // Move the ball in the X direction
  if (breakoutBallCounterX > 0) {
    breakoutBallCounterX--;
    breakoutBallPositionX += breakoutBallIncrementX;

    if (breakoutBallPositionX > 108) { 
      breakoutBallIncrementX = -1;
    } else if (breakoutBallPositionX < 7) {
      if (breakoutBallWasMissed) {
	if (breakoutBallPositionX <= 0) {
	  BreakoutBallMiss();
	  return;
	}
      } else {	
	// Check collision with the paddle       
	byte p1 = (breakoutPaddlePosition < 7) ? 0 : (breakoutPaddlePosition - 8);
	byte b1 = breakoutBallPositionY - 1;
	byte pend = (breakoutPaddlePosition < 8) ? p1 + 45 : p1 + 51;
	byte bend = b1 + 6;
	
	// Was there a collision?
	if (((p1 <= b1) && (pend >= b1))
	    || ((b1 <= p1) && (bend >= p1))) {
	  // sound(1, 1);
	  breakoutBallSlopeX = (byte)random(5);
	  breakoutBallSlopeY = (byte)random(5);
	  breakoutBallIncrementX = 1;
	} else {
	  breakoutBallWasMissed = 1;
	}
      }
    }
  }
  
  // Move the ball in the Y direction
  if (breakoutBallCounterY > 0) {
    breakoutBallCounterY--;
    breakoutBallPositionY += breakoutBallIncrementY;

    if (breakoutBallPositionY > 181) {
      breakoutBallIncrementY = -1;  
    } else if (breakoutBallPositionY < 2) {
      breakoutBallIncrementY = +1;  
    }
  }

  // Reset the slope counters if needed
  if ((breakoutBallCounterX == 0) && (breakoutBallCounterY == 0)) {
    breakoutBallCounterX = breakoutBallSlopeX;
    breakoutBallCounterY = breakoutBallSlopeY;
  }

  // Check collisions with bricks
  for (byte ii=0; ii<brickXPositionsSz; ii++) {
    byte pos = brickXPositions[ii];
    if ((breakoutBallPositionX >= (pos - 5)) && (breakoutBallPositionX < pos))
      BreakoutBallCheckBrickCollision(pos, lineBrickYPositions[ii]);
  }

  // Erase the part of the ball the will not be reset  
  byte xx = (oldX << 1);
  byte offsetX = (breakoutBallPositionX < oldX) ? 6 : 0;
  byte offsetY = (breakoutBallPositionY < oldY) ? 5 : 0;
  BlitterFillRectangle(xx + offsetX, oldY, 2, 6, 0);
  BlitterFillRectangle(xx, oldY + offsetY, 8, 1, 0);

  // Draw the graphics
  BlitterDrawGraphics(GrafxDataBallData, breakoutBallPositionX, breakoutBallPositionY);
}


void BreakoutBallRefresh() {
  BlitterDrawGraphics(GrafxDataBallData, breakoutBallPositionX, breakoutBallPositionY);
}


#endif
