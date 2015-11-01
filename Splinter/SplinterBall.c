/*
  SplinterBall.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Logic for controlling the ball in Splinter.
*/


#ifndef _SplinterBall_c
#define _SplinterBall_c


#include "SplinterBall.h"
#include "Blitter.h"


/** Flag - if 1, then the ball was missed, let it hit left side of screen */
byte splinterBallWasMissed = 0;


void SplinterBallReset() {
  splinterBallPositionX = 40;
  splinterBallPositionY = 3;
  splinterBallIncrementX = -1;
  splinterBallIncrementY = 1;
  splinterBallSlopeX = 1;
  splinterBallSlopeY = 3;
  splinterBallCounterX = splinterBallSlopeX;
  splinterBallCounterY = splinterBallSlopeY;
  splinterBallWasMissed = 0;
}


void SplinterBallInit() {
  SplinterBallReset();
}


void SplinterBallDrawCount() {
  char buffer[4];
  buffer[0] = splinterNumberOfBalls + '0';
  buffer[1] = ' ';
  buffer[2] = 0;
  BlitterDrawText(FontDataFontIndex, FontDataFontData,
		  14, 0, 270, 70, 1, buffer);
}


void SplinterBallMiss() {
  // Decrease the number of balls
  splinterNumberOfBalls--;
  SplinterBallDrawCount();
  SoundPlayAndWait(600, 4, 4, 192);
  SoundPlayAndWait(900, 5, 5, 192);
  SoundPlayAndWait(1800, 6, 6, 192);

  // Erase the displayed ball
  BlitterFillRectangle(splinterBallPositionX 
		       ? (splinterBallPositionX << 1) - 2 : 0,
		       splinterBallPositionY
		       ? splinterBallPositionY - 1 : 0, 12, 8, 0);

  // Reset the ball location
  SplinterBallReset();
}


byte SplinterBallCheckBrickCollision(byte lineBrickXPos, byte *lineBrickYPositions) {
  // We are a little generous with hit detection here to reduce half
  // destroyed bricks
  byte b1 = splinterBallPositionY;
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
	splinterBallIncrementX = -1;
      byte changeDirY = (byte)random(21);
      if (changeDirY > 18)
	splinterBallIncrementY = splinterBallIncrementY * -1;
      
      // Change the slope
      byte changeSlopeX = (byte)random(21);
      if ((changeSlopeX < 10) && (splinterBallSlopeX > 1))
	splinterBallSlopeX--;
      else if ((changeSlopeX > 17) && (splinterBallSlopeX < 5))
	splinterBallSlopeX++;
      
      // Change the slope
      byte changeSlopeY = (byte)random(21);
      if ((changeSlopeY < 5) && (splinterBallSlopeY > 1))
	splinterBallSlopeY--;
      else if ((changeSlopeY > 17) && (splinterBallSlopeY < 5))
	splinterBallSlopeY++;

      // Update the counters
      splinterBallCounterX = splinterBallSlopeX;
      splinterBallCounterY = splinterBallSlopeY;
      SplinterScoreIncrement(&splinterScore, 10);
      numHit++;
      
      // Remove the brick - if none left, reset the level
      BricksRemove();
      if (BricksAllGone())
	return 255;
    }
  }

  return numHit;
}


void SplinterBallTick() {
  byte oldX = splinterBallPositionX;
  byte oldY = splinterBallPositionY;

  // Move the ball in the X direction
  if (splinterBallCounterX > 0) {
    splinterBallCounterX--;
    splinterBallPositionX += splinterBallIncrementX;

    if (splinterBallPositionX > 108) { 
      splinterBallIncrementX = -1;
    } else if (splinterBallPositionX < 7) {
      if (splinterBallWasMissed) {
	if (splinterBallPositionX <= 0) {
	  SplinterBallMiss();
	  return;
	}
      } else {	
	// Check collision with the paddle       
	byte p1 = (splinterPaddlePosition < 7) ? 0 : (splinterPaddlePosition - 8);
	byte b1 = splinterBallPositionY - 1;
	byte pend = (splinterPaddlePosition < 8) ? p1 + 45 : p1 + 51;
	byte bend = b1 + 6;
	
	// Was there a collision?
	if (((p1 <= b1) && (pend >= b1))
	    || ((b1 <= p1) && (bend >= p1))) {
	  SoundPlay(30, 1, 1, 64);
	  int offset = ((int)splinterBallPositionY - (int)splinterPaddlePosition
			+ 3 - 19);
	  if (offset < -6) {
	    splinterBallSlopeY -=  splinterBallIncrementY;
	  } else if (offset > 6) {
	    splinterBallSlopeY += splinterBallIncrementY;
	  } else {	
	    if (splinterBallSlopeY > 1)
	      splinterBallSlopeY--;
	  }
	  if (splinterBallSlopeY >= 0x80) {	    
	    splinterBallSlopeY = 2;
	    splinterBallIncrementY = -splinterBallIncrementY;
	  } else if (splinterBallSlopeY > 5) {
	    splinterBallSlopeY = 5;
	    if (splinterBallSlopeX > 1)
	      splinterBallSlopeX--;
	  }

	  splinterBallIncrementX = 1;
	} else {
	  splinterBallWasMissed = 1;
	}
      }
    }
  }
  
  // Move the ball in the Y direction
  if (splinterBallCounterY > 0) {
    splinterBallCounterY--;
    splinterBallPositionY += splinterBallIncrementY;

    if (splinterBallPositionY > 181) {
      splinterBallIncrementY = -1;  
    } else if (splinterBallPositionY < 2) {
      splinterBallIncrementY = +1;  
    }
  }

  // Reset the slope counters if needed
  if ((splinterBallCounterX == 0) && (splinterBallCounterY == 0)) {
    splinterBallCounterX = splinterBallSlopeX;
    splinterBallCounterY = splinterBallSlopeY;
  }

  // Check collisions with bricks
  byte numHit = 0;
  byte lastHitX = 0;
  for (byte ii=0; ii<brickXPositionsSz; ii++) {
    byte pos = brickXPositions[ii];
    if ((splinterBallPositionX >= (pos - 5)) 
	&& (splinterBallPositionX < (pos + 4))) {
      byte newNumHit = SplinterBallCheckBrickCollision(pos,
						       lineBrickYPositions[ii]);
      if (newNumHit)
	lastHitX = ii;
      numHit |= newNumHit;
    }
  }

  // Erase the part of the ball the will not be reset  
  byte xx = (oldX << 1);
  byte offsetX = (splinterBallPositionX < oldX) ? 6 : 0;
  byte offsetY = (splinterBallPositionY < oldY) ? 5 : 0;
  if (splinterBallPositionX != oldX)
    BlitterFillRectangle(xx + offsetX, oldY, 2, 6, 0);
  if (splinterBallPositionY != oldY)
    BlitterFillRectangle(xx, oldY + offsetY, 8, 1, 0);

  // Draw the graphics
  BlitterDrawGraphics(GrafxDataBallData, splinterBallPositionX, splinterBallPositionY);

  // Play a sound if we hit any bricks
  if (numHit) {
    // Update the score, play a sound
    SplinterDrawScore();      
    SoundPlay(250, brickXSounds[lastHitX], brickXSounds[lastHitX], 128);

    // Do we have to reset the board?
    if (numHit == 255) {
      BricksReset();
      
      BlitterFillRectangle(splinterBallPositionX 
			   ? (splinterBallPositionX << 1) - 2 : 0,
			   splinterBallPositionY
			   ? splinterBallPositionY - 1 : 0, 12, 8, 0);
      SplinterBallReset();	
      BricksRefresh();
      BlitterDrawGraphics(GrafxDataBallData, splinterBallPositionX,
			  splinterBallPositionY);
      
      SoundPlayAndWait(600, 4, 4, 192);
      SoundPlayAndWait(900, 5, 5, 192);
      SoundPlayAndWait(1800, 6, 6, 192);
    }
  }
}


void SplinterBallRefresh() {
  BlitterDrawGraphics(GrafxDataBallData, splinterBallPositionX, splinterBallPositionY);
}


#endif
