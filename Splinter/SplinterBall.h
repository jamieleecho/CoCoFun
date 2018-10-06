/*
  SplinterBall.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Logic for controlling the ball in Splinter.
*/


#ifndef _SplinterBall_h
#define _SplinterBall_h


#include <coco.h>

#include "FixedPoint.h"
#include "Vector2d.h"


/** Ball position */
extern Vector2d splinterBallPosition;

/** Speed of the ball on [.25, 1] */
extern FixedPoint splinterBallVelocity;

/** Number of balls */
extern char splinterNumberOfBalls;

/** Current paddle position */
extern byte splinterPaddlePosition;

/** 
 * Amount to increment each iteration
 * magnitude(splinterBallIncrementVector) * splinterBallVelocity
 * == splinterBallVelocity
 */
extern Vector2d splinterBallIncrementVector;


/** Must be invoked before invoking other SplinterBallFunctions. */
void SplinterBallInit();

/** 
 * Resets the splinterBall position to the default 
 * @param fullReset[in] true to reset everything.
 */
void SplinterBallReset(byte fullReset);

/** Initializes the splinterBall position to the default */
void SplinterBallInit();

/** Display the current number of balls */
void SplinterBallDrawCount();

/** Invoked when a ball is missed */
void SplinterBallMiss();

/**
 * Determines whether the ball is colliding with bricks in lineBrickYPositions.
 * @param lineBrickXPos[in] x position of bricks
 * @param lineBrickYPositions[in/out] Y positions of a line of bricks.
 * @param alreadyHit whether or not a collision was previously made
 * @return number of bricks we hit - 255 means that we cleared the screen.
 */
byte SplinterBallCheckBrickCollision(byte lineBrickXPos,
				     byte *lineBrickYPositions,
				     byte alreadyHit);

/** Controls the splinterBall motion */
void SplinterBallTick();

/** Draw the splinter ball */
void SplinterBallRefresh();

#endif
