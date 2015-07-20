/*
  BreakoutBall.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Logic for controlling the ball in Breakout.
*/


#ifndef _BreakoutBall_c
#define _BreakoutBall_c


/** Ball X position */
byte BreakoutBallPositionX = 20;

/** Ball Y position */
byte BreakoutBallPositionY = 16;

/** Ball increment X amount - either +/-1 */
byte BreakoutBallIncrementX = 1;

/** Ball increment Y amount - either +/-1 */
byte BreakoutBallIncrementY = 1;

/** Maximum amount to add to X before resetting BreakoutBallCounterX */
byte BreakoutBallSlopeX = 5;

/** Maximum amount to add to Y before resetting BreakoutBallCounterY */
byte BreakoutBallSlopeY = 2;

/** Current position in slope */
byte BreakoutBallCounterX = 5;

/** Current position in slope */
byte BreakoutBallCounterY = 2;


/**
 * @param value[in] range of random number
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


/** Resets the BreakoutBall position to the default */
void BreakoutBallReset() {
  BreakoutBallPositionX = 30;
  BreakoutBallPositionY = 3;
  BreakoutBallIncrementX = -1;
  BreakoutBallIncrementY = 1;
  BreakoutBallSlopeX = 1;
  BreakoutBallSlopeY = 5;
  BreakoutBallCounterX = BreakoutBallSlopeX;
  BreakoutBallCounterY = BreakoutBallSlopeY;
}


/** Initializes the BreakoutBall position to the default */
void BreakoutBallInit() {
  BreakoutBallReset();
}


/** Display the current number of balls */
void BreakoutBallDrawCount() {
  char buffer[2];
  buffer[0] = numberOfBalls + '0';
  buffer[1] = 0;
  blitNumericText(buffer, 0x78, 0x6f);
}


/** Invoked when a ball is missed */
void BreakoutBallMiss() {
  // Decrease the number of balls
  numberOfBalls--;
  BreakoutBallDrawCount();
  sound(1, 10);

  // Erase the displayed ball
  blitGraphics2(GrafxDataBlankData, BreakoutBallPositionX, BreakoutBallPositionY);

  // Reset the ball location
  BreakoutBallReset();
}


/**
 * Determines whether the ball is colliding with bricks in lineBrickYPositions.
 * @param lineBrickXPos[in] x position of bricks
 * @param lineBrickYPositions[in/out] Y positions of a line of bricks.
 */
void BreakoutBallCheckBrickCollision(byte lineBrickXPos, byte *lineBrickYPositions) {
  for(byte ii=0; ii<brickYPositionsSz; ii++) {    
    byte yPosition = lineBrickYPositions[ii];

    if ((BreakoutBallPositionY == yPosition) 
	|| ((yPosition < BreakoutBallPositionY) && ((yPosition + 14) > BreakoutBallPositionY))
	|| ((yPosition > BreakoutBallPositionY) && (yPosition < (BreakoutBallPositionY + 11)))) {
      blitGraphics2(GrafxDataBlankData, lineBrickXPos, lineBrickYPositions[ii]);
      lineBrickYPositions[ii] = 0xff;
      BreakoutBallSlopeX = (byte)random(5);
      BreakoutBallSlopeY = (byte)random(5);
      if (BreakoutBallSlopeX & 1)
	BreakoutBallIncrementX = -1;
      BreakoutBallCounterX = BreakoutBallSlopeX;
      BreakoutBallCounterY = BreakoutBallSlopeY;
      BreakoutScoreIncrement(&breakoutScore, 10);
      BreakoutDrawScore();
      sound(1, 1);
      sound(20, 1);
    }    
  }
}


/** Controls the BreakoutBall motion */
void BreakoutBallTick() {
  // Move the ball in the X direction
  if (BreakoutBallCounterX > 0) {
    BreakoutBallCounterX--;
    BreakoutBallPositionX += BreakoutBallIncrementX;

    if (BreakoutBallPositionX > 110) { 
      BreakoutBallIncrementX = -1;
    } else if (BreakoutBallPositionX < 5) {
      BreakoutBallIncrementX = 1;
      
      // Check collision with the paddle
      if (paddlePosition <= 7) {
	if ((paddlePosition + 32) >= BreakoutBallPositionY) {
	  BreakoutBallSlopeX = (byte)random(5);
	  sound(1, 1);
	} else {
	  BreakoutBallMiss();
	  return;
	}
      } else if (paddlePosition == BreakoutBallPositionY) {
	BreakoutBallSlopeX = (byte)random(5);
	sound(1, 1);
      } else if ((paddlePosition - 7) < BreakoutBallPositionY) {
	if ((paddlePosition + 32) >= BreakoutBallPositionY) {
	  BreakoutBallSlopeX = (byte)random(5);
	  sound(1, 1);
	}	  
      } else {
	BreakoutBallMiss();
	return;
      }
    }
  }
  
  // Move the ball in the Y direction
  if (BreakoutBallCounterY > 0) {
    BreakoutBallCounterY--;
    BreakoutBallPositionY += BreakoutBallIncrementY;
    if (BreakoutBallPositionY > 179) {
      BreakoutBallIncrementY = -1;  
    } else if (BreakoutBallPositionY < 2) {
      BreakoutBallIncrementY = +1;  
    }
  }

  // Reset the slope counters if needed
  if ((BreakoutBallCounterX == 0) && (BreakoutBallCounterY == 0)) {
    BreakoutBallCounterX = BreakoutBallSlopeX;
    BreakoutBallCounterY = BreakoutBallSlopeY;
  }

  // Draw the graphics
  blitGraphics2(GrafxDataBallData, BreakoutBallPositionX, BreakoutBallPositionY);

  // Check collisions with bricks
  if ((BreakoutBallPositionX >= 43) && (BreakoutBallPositionX < 50))
    BreakoutBallCheckBrickCollision(50, line1BrickYPositions);
  else if ((BreakoutBallPositionX >= 58) && (BreakoutBallPositionX < 65))
    BreakoutBallCheckBrickCollision(65, line2BrickYPositions);
  else if ((BreakoutBallPositionX >= 73) && (BreakoutBallPositionX < 80))
    BreakoutBallCheckBrickCollision(80, line3BrickYPositions);
  else if ((BreakoutBallPositionX >= 88) && (BreakoutBallPositionX < 95))
    BreakoutBallCheckBrickCollision(95, line4BrickYPositions);
}


#endif
