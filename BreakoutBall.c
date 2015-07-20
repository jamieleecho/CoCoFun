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


void BreakoutBallReset() {
  breakoutBallPositionX = 30;
  breakoutBallPositionY = 3;
  breakoutBallIncrementX = -1;
  breakoutBallIncrementY = 1;
  breakoutBallSlopeX = 1;
  breakoutBallSlopeY = 5;
  breakoutBallCounterX = breakoutBallSlopeX;
  breakoutBallCounterY = breakoutBallSlopeY;
}


void BreakoutBallInit() {
  BreakoutBallReset();
}


void BreakoutBallDrawCount() {
  char buffer[2];
  buffer[0] = breakoutNumberOfBalls + '0';
  buffer[1] = 0;
  blitNumericText(buffer, 0x78, 0x6f);
}


void BreakoutBallMiss() {
  // Decrease the number of balls
  breakoutNumberOfBalls--;
  BreakoutBallDrawCount();
  sound(1, 10);

  // Erase the displayed ball
  blitGraphics2(GrafxDataBlankData, breakoutBallPositionX, breakoutBallPositionY);

  // Reset the ball location
  BreakoutBallReset();
}


void BreakoutBallCheckBrickCollision(byte lineBrickXPos, byte *lineBrickYPositions) {
  for(byte ii=0; ii<brickYPositionsSz; ii++) {    
    byte yPosition = lineBrickYPositions[ii];

    if ((breakoutBallPositionY == yPosition) 
	|| ((yPosition < breakoutBallPositionY) && ((yPosition + 14) > breakoutBallPositionY))
	|| ((yPosition > breakoutBallPositionY) && (yPosition < (breakoutBallPositionY + 11)))) {
      blitGraphics2(GrafxDataBlankData, lineBrickXPos, lineBrickYPositions[ii]);
      lineBrickYPositions[ii] = 0xff;
      breakoutBallSlopeX = (byte)random(5);
      breakoutBallSlopeY = (byte)random(5);
      if (breakoutBallSlopeX & 1)
	breakoutBallIncrementX = -1;
      breakoutBallCounterX = breakoutBallSlopeX;
      breakoutBallCounterY = breakoutBallSlopeY;
      BreakoutScoreIncrement(&breakoutScore, 10);
      BreakoutDrawScore();      
      sound(1, 1);
      sound(20, 1);

      // Remove the brick - if none left, reset the level
      BricksRemove();
      if (BricksAllGone()) {
	blitGraphics2(GrafxDataBlankData, breakoutBallPositionX, breakoutBallPositionY);
	BricksReset();
	BreakoutBallReset();	
	BricksDrawBricks();
	return;
      }
    }    
  }
}


void BreakoutBallTick() {
  // Move the ball in the X direction
  if (breakoutBallCounterX > 0) {
    breakoutBallCounterX--;
    breakoutBallPositionX += breakoutBallIncrementX;

    if (breakoutBallPositionX > 110) { 
      breakoutBallIncrementX = -1;
    } else if (breakoutBallPositionX < 5) {
      breakoutBallIncrementX = 1;
      
      // Check collision with the paddle
      if (breakoutPaddlePosition <= 7) {
	if ((breakoutPaddlePosition + 32) >= breakoutBallPositionY) {
	  breakoutBallSlopeX = (byte)random(5);
	  sound(1, 1);
	} else {
	  BreakoutBallMiss();
	  return;
	}
      } else if (breakoutPaddlePosition == breakoutBallPositionY) {
	breakoutBallSlopeX = (byte)random(5);
	sound(1, 1);
      } else if ((breakoutPaddlePosition - 7) < breakoutBallPositionY) {
	if ((breakoutPaddlePosition + 32) >= breakoutBallPositionY) {
	  breakoutBallSlopeX = (byte)random(5);
	  sound(1, 1);
	}	  
      } else {
	BreakoutBallMiss();
	return;
      }
    }
  }
  
  // Move the ball in the Y direction
  if (breakoutBallCounterY > 0) {
    breakoutBallCounterY--;
    breakoutBallPositionY += breakoutBallIncrementY;
    if (breakoutBallPositionY > 179) {
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

  // Draw the graphics
  blitGraphics2(GrafxDataBallData, breakoutBallPositionX, breakoutBallPositionY);

  // Check collisions with bricks
  if ((breakoutBallPositionX >= 43) && (breakoutBallPositionX < 50))
    BreakoutBallCheckBrickCollision(50, line1BrickYPositions);
  else if ((breakoutBallPositionX >= 58) && (breakoutBallPositionX < 65))
    BreakoutBallCheckBrickCollision(65, line2BrickYPositions);
  else if ((breakoutBallPositionX >= 73) && (breakoutBallPositionX < 80))
    BreakoutBallCheckBrickCollision(80, line3BrickYPositions);
  else if ((breakoutBallPositionX >= 88) && (breakoutBallPositionX < 95))
    BreakoutBallCheckBrickCollision(95, line4BrickYPositions);
}


#endif
