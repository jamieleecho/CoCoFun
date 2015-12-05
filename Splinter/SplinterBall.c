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


/** 2.0 */
FixedPoint splinterBall2 = FixedPointInit(2, 0x0000);

/** 1.0 */
FixedPoint splinterBall1 = FixedPointInit(1, 0x0000);


/** .75 */
FixedPoint splinterBallPoint75 = FixedPointInit(0, 0xc000);


/** .5 */
FixedPoint splinterBallPoint5 = FixedPointInit(0, 0x8000);


/** .25 */
FixedPoint splinterBallPoint25 = FixedPointInit(0, 0x4000);


/** Top normal */
Vector2d splinterBallTopNormal;

/** Bottom normal */
Vector2d splinterBallBottomNormal;

/** Right normal */
Vector2d splinterBallRightNormal;

/** Left normal */
Vector2d splinterBallLeftNormal;


void SplinterBallInit() {
  FixedPointSet(&splinterBallTopNormal.data[0], 0, 0);
  FixedPointSet(&splinterBallTopNormal.data[1], 1, 0);
  FixedPointSet(&splinterBallBottomNormal.data[0], 0, 0);
  FixedPointSet(&splinterBallBottomNormal.data[1], -1, 0);
  FixedPointSet(&splinterBallRightNormal.data[0], -1, 0);
  FixedPointSet(&splinterBallRightNormal.data[1], 0, 0);
  FixedPointSet(&splinterBallLeftNormal.data[0], -1, 0);
  FixedPointSet(&splinterBallLeftNormal.data[1], 0, 0);
  SplinterBallReset();
}


/**
 * Updates splinterBallIncrementVector with the current slope and whether
 * or not we should be moving left and up.
 *
 * @param moveLeft[in] if true, ensure the ball will move left
 * @param moveUp[in] if true, ensure the ball will move up
 */
void SplinterBallSetSlope(byte moveLeft, byte moveUp) {
  if (moveLeft)
    FixedPointNegate(&(splinterBallIncrementVector.data[0]), &splinterBall1);
  else
    FixedPointCopy(&(splinterBallIncrementVector.data[0]), &splinterBall1);

  if (moveUp)
    FixedPointNegate(&(splinterBallIncrementVector.data[1]), &splinterBallSlope);
  else
    FixedPointCopy(&(splinterBallIncrementVector.data[1]), &splinterBallSlope);
  Vector2dNormalize(&splinterBallIncrementVector, &splinterBallIncrementVector);

  FixedPointMul(&(splinterBallIncrementVector.data[0]),
		&(splinterBallIncrementVector.data[0]), &splinterBallVelocity);
  FixedPointMul(&(splinterBallIncrementVector.data[1]),
		&(splinterBallIncrementVector.data[1]), &splinterBallVelocity);
}


void SplinterBallReset() {
  FixedPointSet(&(splinterBallPosition.data[0]), 40, 0);
  FixedPointSet(&(splinterBallPosition.data[1]), 3, 0);
  FixedPointCopy(&splinterBallVelocity, &splinterBallPoint75);
  FixedPointCopy(&splinterBallSlope, &splinterBall2);

  SplinterBallSetSlope(TRUE, FALSE);
  
  splinterBallWasMissed = 0;
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
  BlitterFillRectangle((splinterBallPosition.data[0]).Whole 
		       ? (splinterBallPosition.data[0].Whole << 1) - 2 : 0,
		       splinterBallPosition.data[1].Whole
		       ? splinterBallPosition.data[1].Whole - 1 : 0, 12, 8, 0);

  // Reset the ball location
  SplinterBallReset();
}


byte SplinterBallCheckBrickCollision(byte lineBrickXPos,
				     byte *lineBrickYPositions,
				     byte alreadyHit) {
  // We are a little generous with hit detection here to reduce half
  // destroyed bricks
  byte b1 = (byte)splinterBallPosition.data[1].Whole;
  byte bend = b1 + 6;
  byte numHit = 0;
  for(byte ii=0; ii<brickYPositionsSz; ii++) {    
    byte p1 = lineBrickYPositions[ii];
    byte pend = p1 + 14;
    
    if (((p1 <= b1) && (pend >= b1))
	|| ((b1 <= p1) && (bend >= p1))) {
      BlitterFillRectangle(lineBrickXPos << 1, lineBrickYPositions[ii], 5, 11, 0);
      lineBrickYPositions[ii] = 0xff;

      if (!alreadyHit) {
	Vector2dReflectionVector(&splinterBallIncrementVector,
				 &splinterBallIncrementVector,
				 &splinterBallRightNormal);
      }
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
  byte oldX = (byte)splinterBallPosition.data[0].Whole;
  byte oldY = (byte)splinterBallPosition.data[1].Whole;

  // Move the ball
  Vector2dAdd(&splinterBallPosition, &splinterBallPosition,
	      &splinterBallIncrementVector);

  // Did we hit the right side of the screen?
  if (splinterBallPosition.data[0].Whole > 108) { 
    FixedPointNegate(&splinterBallIncrementVector.data[0],
		     &splinterBallIncrementVector.data[0]);
  }
  
  // Are we hitting the left end of the screen?
  else if (splinterBallPosition.data[0].Whole < 7) {      
    if (splinterBallWasMissed) {
      if (splinterBallPosition.data[0].Whole <= 0) {
	SplinterBallMiss();
	return;
      }
    } else {	
	// Check collision with the paddle       
	byte p1 = (splinterPaddlePosition < 7) ? 0 : (splinterPaddlePosition - 8);
	byte b1 = (byte)splinterBallPosition.data[1].Whole - 1;
	byte pend = (splinterPaddlePosition < 8) ? p1 + 45 : p1 + 51;
	byte bend = b1 + 6;
	
	// Was there a collision?
	if (((p1 <= b1) && (pend >= b1))
	    || ((b1 <= p1) && (bend >= p1))) {
	  SoundPlay(30, 1, 1, 64);
	  int offset = (splinterBallPosition.data[1].Whole
			- (int)splinterPaddlePosition
			+ 3 - 19);
	  if (offset < -6) {
	    FixedPointNegate(&splinterBallIncrementVector.data[0],
			     &splinterBallIncrementVector.data[0]);
	  } else if (offset > 6) {
	    FixedPointNegate(&splinterBallIncrementVector.data[0],
			     &splinterBallIncrementVector.data[0]);
	  } else {	
	    FixedPointNegate(&splinterBallIncrementVector.data[0],
			     &splinterBallIncrementVector.data[0]);
	  }
	} else {
	  splinterBallWasMissed = 1;
	}
    }
  }

  // Did we hit the bottom of the screen?
  if ((byte)splinterBallPosition.data[1].Whole > 181) {
    FixedPointNegate(&splinterBallIncrementVector.data[1],
		     &splinterBallIncrementVector.data[1]);
  }

  // Did we hit the top of the screen?
  else if ((byte)splinterBallPosition.data[1].Whole < 2) {
    FixedPointNegate(&splinterBallIncrementVector.data[1],
		     &splinterBallIncrementVector.data[1]);
  }

  // Check collisions with bricks
  byte numHit = 0;
  byte lastHitX = 0;
  for (byte ii=0; ii<brickXPositionsSz; ii++) {
    byte pos = brickXPositions[ii];
    if ((splinterBallPosition.data[0].Whole >= (pos - 5)) 
	&& (splinterBallPosition.data[0].Whole < (pos + 4))) {
      byte newNumHit = SplinterBallCheckBrickCollision(pos,
						       lineBrickYPositions[ii],
						       numHit);
      if (newNumHit)
	lastHitX = ii;
      numHit |= newNumHit;
    }
  }

  // Erase the part of the ball the will not be reset  
  byte xx = (oldX << 1);
  byte offsetX = ((byte)splinterBallPosition.data[0].Whole < oldX) ? 6 : 0;
  byte offsetY = ((byte)splinterBallPosition.data[1].Whole < oldY) ? 5 : 0;
  if ((byte)splinterBallPosition.data[0].Whole != oldX)
    BlitterFillRectangle(xx + offsetX, oldY, 2, 6, 0);
  if ((byte)splinterBallPosition.data[1].Whole != oldY)
    BlitterFillRectangle(xx, oldY + offsetY, 8, 1, 0);

  // Draw the graphics
  BlitterDrawGraphics(GrafxDataBallData,
		      (byte)splinterBallPosition.data[0].Whole,
		      (byte)splinterBallPosition.data[1].Whole);

  // Play a sound if we hit any bricks
  if (numHit) {
    // Update the score, play a sound
    SplinterDrawScore();      
    SoundPlay(250, brickXSounds[lastHitX], brickXSounds[lastHitX], 128);

    // Do we have to reset the board?
    if (numHit == 255) {
      BricksReset();
      
      BlitterFillRectangle(splinterBallPosition.data[0].Whole 
			   ? ((byte)splinterBallPosition.data[0].Whole<<1)-2 : 0,
			   (byte)splinterBallPosition.data[1].Whole
			   ? (byte)splinterBallPosition.data[1].Whole-1
			   : 0, 12, 8, 0);
      SplinterBallReset();	
      BricksRefresh();
      BlitterDrawGraphics(GrafxDataBallData,
			  (byte)splinterBallPosition.data[0].Whole,
			  (byte)splinterBallPosition.data[1].Whole);
      
      SoundPlayAndWait(600, 6, 6, 192);
      SoundPlayAndWait(900, 5, 5, 192);
      SoundPlayAndWait(1800, 4, 4, 192);
    }
  }
}


void SplinterBallRefresh() {
  BlitterDrawGraphics(GrafxDataBallData,
		      (byte)splinterBallPosition.data[0].Whole,
		      (byte)splinterBallPosition.data[1].Whole);
}


#endif
