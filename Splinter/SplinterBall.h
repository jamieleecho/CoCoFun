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


/** Ball position */
FixedPoint splinterBallPosition[2];

/** Slope of the ball (x/y) on [0, 4] */
FixedPoint splinterBallSlope = FixedPointInit(1, 0);

/** Speed of the ball on [.25, 1] */
FixedPoint splinterBallVelocity = FixedPointInit(0, 0x4000);

/** 
 * Amount to increment each iteration
 * magnitude(splinterBallIncrementVector) * splinterBallVelocity
 * == splinterBallVelocity
 */
FixedPoint splinterBallIncrementVector[2];

/** Resets the splinterBall position to the default */
void SplinterBallReset();

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
 * @return number of bricks we hit - 255 means that we cleared the screen.
 */
byte SplinterBallCheckBrickCollision(byte lineBrickXPos, byte *lineBrickYPositions);

/** Controls the splinterBall motion */
void SplinterBallTick();

/** Draw the splinter ball */
void SplinterBallRefresh();

#endif
