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
  breakoutBallSlopeY = 3;
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
  SoundPlayAndWait(600, 16, 16, 192);
  SoundPlayAndWait(900, 17, 17, 192);
  SoundPlayAndWait(1800, 18, 18, 192);

  // Erase the displayed ball
  BlitterFillRectangle(breakoutBallPositionX 
		       ? (breakoutBallPositionX << 1) - 2 : 0,
		       breakoutBallPositionY
		       ? breakoutBallPositionY - 1 : 0, 12, 8, 0);

  // Reset the ball location
  BreakoutBallReset();
}


byte BreakoutBallCheckBrickCollision(byte lineBrickXPos, byte *lineBrickYPositions) {
  // We are a little generous with hit detection here to reduce half
  // destroyed bricks
  byte b1 = breakoutBallPositionY;
  byte bend = b1 + 6;
  byte numHit = 0;
  for(byte ii=0; ii<brickYPositionsSz; ii++) {    
    byte p1 = lineBrickYPositions[ii];
    byte pend = p1 + 14;
    
    if (((p1 <= b1) && (pend >= b1))
	|| ((b1 <= p1) && (bend >= p1))) {
      BlitterFillRectangle(lineBrickXPos << 1, lineBrickYPositions[ii], 5, 11, 0);
      lineBrickYPositions[ii] = 0xff;

      // Change direction? Favor not changing X direction and favor going towards
      // paddle
      byte changeDirX = (byte)random(21);
      if (changeDirX > 4)
	breakoutBallIncrementX = -1;
      byte changeDirY = (byte)random(21);
      if (changeDirY > 18)
	breakoutBallIncrementY = breakoutBallIncrementY * -1;
      
      // Change the slope
      byte changeSlopeX = (byte)random(21);
      if ((changeSlopeX < 10) && (breakoutBallSlopeX > 1))
	breakoutBallSlopeX--;
      else if ((changeSlopeX > 17) && (breakoutBallSlopeX < 5))
	breakoutBallSlopeX++;
      
      // Change the slope
      byte changeSlopeY = (byte)random(21);
      if ((changeSlopeY < 5) && (breakoutBallSlopeY > 1))
	breakoutBallSlopeY--;
      else if ((changeSlopeY > 17) && (breakoutBallSlopeY < 5))
	breakoutBallSlopeY++;

      // Update the counters
      breakoutBallCounterX = breakoutBallSlopeX;
      breakoutBallCounterY = breakoutBallSlopeY;
      BreakoutScoreIncrement(&breakoutScore, 10);
      numHit++;
      
      // Remove the brick - if none left, reset the level
      BricksRemove();
      if (BricksAllGone())
	return 255;
    }
  }

  return numHit;
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
	  SoundPlay(60, 1, 1, 64);
	  int offset = ((int)breakoutBallPositionY - (int)breakoutPaddlePosition
			+ 3 - 19);
	  if (offset < -6) {
	    breakoutBallSlopeY -=  breakoutBallIncrementY;
	  } else if (offset > 6) {
	    breakoutBallSlopeY += breakoutBallIncrementY;
	  } else {	
	    if (breakoutBallSlopeY > 1)
	      breakoutBallSlopeY--;
	  }
	  if (breakoutBallSlopeY >= 0x80) {	    
	    breakoutBallSlopeY = 2;
	    breakoutBallIncrementY = -breakoutBallIncrementY;
	  } else if (breakoutBallSlopeY > 5) {
	    breakoutBallSlopeY = 5;
	    if (breakoutBallSlopeX > 1)
	      breakoutBallSlopeX--;
	  }

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
  byte numHit = 0;
  byte lastHitX = 0;
  for (byte ii=0; ii<brickXPositionsSz; ii++) {
    byte pos = brickXPositions[ii];
    if ((breakoutBallPositionX >= (pos - 5)) 
	&& (breakoutBallPositionX < (pos + 2))) {
      byte newNumHit = BreakoutBallCheckBrickCollision(pos,
						       lineBrickYPositions[ii]);
      if (newNumHit)
	lastHitX = ii;
      numHit |= newNumHit;
    }
  }

  // Erase the part of the ball the will not be reset  
  byte xx = (oldX << 1);
  byte offsetX = (breakoutBallPositionX < oldX) ? 6 : 0;
  byte offsetY = (breakoutBallPositionY < oldY) ? 5 : 0;
  if (breakoutBallPositionX != oldX)
    BlitterFillRectangle(xx + offsetX, oldY, 2, 6, 0);
  if (breakoutBallPositionY != oldY)
    BlitterFillRectangle(xx, oldY + offsetY, 8, 1, 0);

  // Draw the graphics
  BlitterDrawGraphics(GrafxDataBallData, breakoutBallPositionX, breakoutBallPositionY);

  // Play a sound if we hit any bricks
  if (numHit) {
    // Update the score, play a sound
    BreakoutDrawScore();      
    SoundPlayAndWait(250, brickXSounds[lastHitX], brickXSounds[lastHitX], 128);

    // Do we have to reset the board?
    if (numHit == 255) {
      BricksReset();
      
      BlitterFillRectangle(breakoutBallPositionX 
			   ? (breakoutBallPositionX << 1) - 2 : 0,
			   breakoutBallPositionY
			   ? breakoutBallPositionY - 1 : 0, 12, 8, 0);
      BreakoutBallReset();	
      BricksRefresh();
      BlitterDrawGraphics(GrafxDataBallData, breakoutBallPositionX,
			  breakoutBallPositionY);
      
      SoundPlayAndWait(600, 18, 18, 192);
      SoundPlayAndWait(900, 17, 17, 192);
      SoundPlayAndWait(1800, 16, 16, 192);
    }
  }
}


void BreakoutBallRefresh() {
  BlitterDrawGraphics(GrafxDataBallData, breakoutBallPositionX, breakoutBallPositionY);
}


#endif
