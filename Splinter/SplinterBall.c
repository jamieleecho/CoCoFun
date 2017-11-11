/*
  SplinterBall.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Logic for controlling the ball in Splinter.
*/


#include "Blitter.h"
#include "Bricks.h"
#include "CoCoMisc.h"
#include "FontData.h"
#include "GrafxData.h"
#include "Sound.h"
#include "Splinter.h"
#include "SplinterBall.h"


/** Ball position */
Vector2d splinterBallPosition;

/** Speed of the ball on [.25, 1] */
FixedPoint splinterBallVelocity = FixedPointInit(0, 0x4000);

/** Number of balls */
char splinterNumberOfBalls;

/** Current paddle position */
byte splinterPaddlePosition;

/** 
 * Amount to increment each iteration
 * magnitude(splinterBallIncrementVector) * splinterBallVelocity
 * == splinterBallVelocity
 */
Vector2d splinterBallIncrementVector;

/** Current splinter ball level */
byte splinterBallLevel = 0;

/** Flag - if 1, then the ball was missed, let it hit left side of screen */
byte splinterBallWasMissed = 0;

/** TRUE if the ball has been hit by the paddle */
byte splinterBallHitByPaddle = FALSE;

/** Amount to increase velocity by when the paddle is hit the first time */
FixedPoint splinterBallMultiplyFactor;

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

/** .125 */
FixedPoint splinterBallPoint125 = FixedPointInit(0, 0x2000);

/** .0625 */
FixedPoint splinterBallPoint0625 = FixedPointInit(0, 0x1000);

/** Right normal */
Vector2d splinterBallRightNormal;

/** Normals used to compute bounce off paddle */
Vector2d splinterBallPaddleNormals[8];

/** Normals used to compute bounce off bricks */
Vector2d splinterBallBrickNormals[12];


void SplinterBallInit() {
  FixedPointSet(&splinterBallRightNormal.data[0], -1, 0);
  FixedPointSet(&splinterBallRightNormal.data[1], 0, 0);

  // Initialize paddle normals
  FixedPoint angle =  FixedPointInit(30, 0); // in degrees
  FixedPoint one80 = FixedPointInit(180, 0);
  FixedPoint degreesToRadians = FixedPointInitPi();
  FixedPointDiv(&degreesToRadians, &degreesToRadians, &one80);
  FixedPointMul(&angle, &angle, &degreesToRadians);
  FixedPoint numIncrements = FixedPointInit(3, 0);
  FixedPoint angleIncrement;
  FixedPointDiv(&angleIncrement, &angle, &numIncrements);
  FixedPoint currentTopAngle = FixedPointInit(0, 0);
  FixedPoint currentBottomAngle = FixedPointInit(0, 0);
  for(byte ii=0; ii<4; ii++) {
    byte topIndex = 3 - ii;
    byte bottomIndex = 4 + ii;

    // Fill out the top vector
    Vector2d tmpVector;
    FixedPointCos((&tmpVector.data[0]), &currentTopAngle);
    FixedPointSin((&tmpVector.data[1]), &currentTopAngle);
    memcpy(&(splinterBallPaddleNormals[topIndex]), &tmpVector,
	   sizeof(tmpVector));

    // Fill out the bottom vector
    FixedPointSin((&tmpVector.data[1]), &currentBottomAngle);
    memcpy(&(splinterBallPaddleNormals[bottomIndex]), &tmpVector,
	   sizeof(tmpVector));

    // Increment angles
    FixedPointSub(&currentTopAngle, &currentTopAngle, &angleIncrement);
    FixedPointAdd(&currentBottomAngle, &currentBottomAngle, &angleIncrement);
  }

  // Initialize brick normals
  FixedPointSet(&angle, 30, 0);
  FixedPointMul(&angle, &angle, &degreesToRadians);
  FixedPointSet(&numIncrements,  5, 0);
  FixedPointDiv(&angleIncrement, &angle, &numIncrements);
  FixedPointSet(&currentTopAngle, 0, 0);
  FixedPointSet(&currentBottomAngle, 0, 0);
  for(byte ii=0; ii<6; ii++) {
    byte topIndex = 5 - ii;
    byte bottomIndex = 6 + ii;

    // Fill out the top vector
    Vector2d tmpVector;
    FixedPointCos((&tmpVector.data[0]), &currentTopAngle);
    FixedPointSin((&tmpVector.data[1]), &currentTopAngle);
    memcpy(&(splinterBallBrickNormals[topIndex]), &tmpVector,
	   sizeof(tmpVector));

    // Fill out the bottom vector
    FixedPointSin((&tmpVector.data[1]), &currentBottomAngle);
    memcpy(&(splinterBallBrickNormals[bottomIndex]), &tmpVector,
	   sizeof(tmpVector));
    
    // Increment angles
    FixedPointSub(&currentTopAngle, &currentTopAngle, &angleIncrement);
    FixedPointAdd(&currentBottomAngle, &currentBottomAngle, &angleIncrement);
  }

  SplinterBallReset(TRUE);
}


void SplinterBallReset(byte fullReset) {
  if (fullReset)
    splinterBallLevel = 0;

  // Random ball position
  byte initialPosition = (byte)random(150);
  FixedPointSet(&(splinterBallPosition.data[0]), 88, 0);  
  FixedPointSet(&(splinterBallPosition.data[1]), initialPosition, 0);

  // Make the initial velocity slow
  FixedPoint splinterBallInitialVelocity;
  FixedPointCopy(&splinterBallInitialVelocity, &splinterBallPoint5);

  // Velocity after the first hit
  FixedPointSet(&splinterBallVelocity, splinterBallLevel, 0);
  FixedPointMul(&splinterBallVelocity, &splinterBallVelocity,
		&splinterBallPoint0625);
  FixedPointAdd(&splinterBallVelocity, &splinterBallVelocity,
		&splinterBallPoint75);

  // Initial ball slope
  FixedPoint splinterBallSlope;
  FixedPointCopy(&splinterBallSlope, &splinterBall2);
  FixedPointNegate(&(splinterBallIncrementVector.data[0]), &splinterBall1);

  if (initialPosition > 96)
    FixedPointNegate(&(splinterBallIncrementVector.data[1]), &splinterBallSlope);
  else
    FixedPointCopy(&(splinterBallIncrementVector.data[1]), &splinterBallSlope);
  
  Vector2dNormalize(&splinterBallIncrementVector, &splinterBallIncrementVector);
  
  Vector2dMul(&splinterBallIncrementVector, &splinterBallInitialVelocity,
	      &splinterBallIncrementVector);
  FixedPointDiv(&splinterBallMultiplyFactor, &splinterBallVelocity, &splinterBallInitialVelocity);
  
  splinterBallWasMissed = 0;
  splinterBallHitByPaddle = FALSE;  
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
  BlitterFillRectangle((splinterBallPosition.data[0].Whole > 1)
		       ? (splinterBallPosition.data[0].Whole - 2) : 0,
		       splinterBallPosition.data[1].Whole
		       ? splinterBallPosition.data[1].Whole - 1 : 0, 12, 8, 0);

  // Reset the ball location
  SplinterBallReset(FALSE);
}


/** Make sure that the increment vector is not too low in the
 *  horizontal direction */
void SplinterBallFixIncrementVector() {
  // Make sure the horizontal component is positive
  byte mustNegate = FALSE;
  if (splinterBallIncrementVector.data[0].Whole < 0) {
    mustNegate = TRUE;
    FixedPointNegate(&(splinterBallIncrementVector.data[0]),
		     &(splinterBallIncrementVector.data[0]));
  }
    
  // Check if the slope is too low
  if (FixedPointLessThan(&(splinterBallIncrementVector.data[0]),
			 &splinterBallPoint25)) {
    memcpy(&(splinterBallIncrementVector.data[0]),
	   &splinterBallPoint25, sizeof(splinterBallPoint25));
    if (mustNegate)
      FixedPointNegate(&(splinterBallIncrementVector.data[0]),
		       &(splinterBallIncrementVector.data[0]));

    mustNegate = &(splinterBallIncrementVector.data[1].Whole) < 0;
    FixedPointSet(&(splinterBallIncrementVector.data[1]), 0, 63454);
    if (mustNegate)
      FixedPointNegate(&(splinterBallIncrementVector.data[1]),
		       &(splinterBallIncrementVector.data[1]));

    Vector2dNormalize(&splinterBallIncrementVector,
		      &splinterBallIncrementVector);
    Vector2dMul(&splinterBallIncrementVector, &splinterBallVelocity,
		&splinterBallIncrementVector);
  } else {
    if (mustNegate)
      FixedPointNegate(&(splinterBallIncrementVector.data[0]),
		       &(splinterBallIncrementVector.data[0]));
  }
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
	alreadyHit = TRUE;
	p1 = p1 >> 4;
	Vector2dReflectionVector(&splinterBallIncrementVector,
				 &splinterBallIncrementVector,
				 &(splinterBallBrickNormals[p1]));
	SplinterBallFixIncrementVector();
      }
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
  byte oldX = (byte)(splinterBallPosition.data[0].Whole >> 1);
  byte oldY = (byte)splinterBallPosition.data[1].Whole;

  // Move the ball
  Vector2dAdd(&splinterBallPosition, &splinterBallPosition,
	      &splinterBallIncrementVector);

  // Did we hit the right side of the screen?
  if (splinterBallPosition.data[0].Whole > 216) { 
    FixedPointNegate(&splinterBallIncrementVector.data[0],
		     &splinterBallIncrementVector.data[0]);
  }
  
  // Are we hitting the left end of the screen?
  else if (splinterBallPosition.data[0].Whole < 14) {      
    if (splinterBallWasMissed) {
      if (splinterBallPosition.data[0].Whole <= 0) {
	SplinterBallMiss();
	return;
      }
    } else if (splinterBallIncrementVector.data[0].Whole < 0) {	
	// Check collision with the paddle       
	byte p1 = (splinterPaddlePosition < 7) ? 0 : (splinterPaddlePosition - 8);
	byte b1 = (byte)splinterBallPosition.data[1].Whole - 1;
	byte pend = (splinterPaddlePosition < 8) ? p1 + 45 : p1 + 51;
	byte bend = b1 + 6;
	
	// Was there a collision?
	if (((p1 <= b1) && (pend >= b1))
	    || ((b1 <= p1) && (bend >= p1))) {
	  SoundPlay(30, 1, 1, 64);
	  splinterBallWasMissed = 0;
	  if (!splinterBallHitByPaddle) {
	    splinterBallHitByPaddle = TRUE;
	    Vector2dMul(&splinterBallIncrementVector,
			&splinterBallMultiplyFactor,
			&splinterBallIncrementVector);
	  }
	  int offset = (splinterBallPosition.data[1].Whole
			- (int)splinterPaddlePosition
			+ 3);

	  if (offset < 0) offset = 0;
	  byte boffset = (byte)offset;
	  boffset = boffset >> 2;
	  if (boffset > 7) boffset = 7;

	  Vector2dReflectionVector(&splinterBallIncrementVector,
				   &splinterBallIncrementVector,
				   &(splinterBallPaddleNormals[boffset]));
	  
	  if (splinterBallIncrementVector.data[0].Whole < 0)
	    FixedPointNegate(&(splinterBallIncrementVector.data[0]),
			     &(splinterBallIncrementVector.data[0]));

	  SplinterBallFixIncrementVector();

 	} else {
	  if (splinterBallPosition.data[0].Whole < 12)
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
    if (((splinterBallPosition.data[0].Whole >> 1) >= (pos - 5)) 
	&& ((splinterBallPosition.data[0].Whole >> 1) < (pos + 4))) {
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
  byte offsetX = ((byte)(splinterBallPosition.data[0].Whole >> 1) < oldX) ? 6 : 0;
  byte offsetY = ((byte)splinterBallPosition.data[1].Whole < oldY) ? 5 : 0;
  if ((byte)(splinterBallPosition.data[0].Whole >> 1) != oldX)
    BlitterFillRectangle(xx + offsetX, oldY, 2, 6, 0);
  if ((byte)splinterBallPosition.data[1].Whole != oldY)
    BlitterFillRectangle(xx, oldY + offsetY, 8, 1, 0);

  // Draw the graphics
  SplinterBallRefresh();

  // Play a sound if we hit any bricks
  if (numHit) {
    // Update the score, play a sound
    SplinterDrawScore();
    SoundPlay(250, brickXSounds[lastHitX], brickXSounds[lastHitX], 128);

    // Do we have to reset the board?
    if (numHit == 255) {
      BricksReset();
      
      BlitterFillRectangle((splinterBallPosition.data[0].Whole > 1)
			   ? ((byte)splinterBallPosition.data[0].Whole-2) : 0,
			   (byte)splinterBallPosition.data[1].Whole
			   ? (byte)splinterBallPosition.data[1].Whole-1
			   : 0, 12, 8, 0);
      SplinterBallReset(FALSE);	
      BricksRefresh();
      BlitterDrawGraphics(GrafxDataBallData,
			  (byte)(splinterBallPosition.data[0].Whole >> 1),
			  (byte)splinterBallPosition.data[1].Whole);

      if (++splinterBallLevel > 4)
	splinterBallLevel = 4;

      SoundPlayAndWait(600, 6, 6, 192);
      SoundPlayAndWait(900, 5, 5, 192);
      SoundPlayAndWait(1800, 4, 4, 192);
    }
  }
}


void SplinterBallRefresh() {
    if (splinterBallPosition.data[0].Whole & 1)
    BlitterDrawGraphics(GrafxDataBallData2,
			(byte)splinterBallPosition.data[0].Whole >> 1,
			(byte)splinterBallPosition.data[1].Whole);
  else
    BlitterDrawGraphics(GrafxDataBallData,
			(byte)splinterBallPosition.data[0].Whole >> 1,
			(byte)splinterBallPosition.data[1].Whole);
}
