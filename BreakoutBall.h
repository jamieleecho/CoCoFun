/*
  BreakoutBall.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Logic for controlling the ball in Breakout.
*/


#ifndef _BreakoutBall_h
#define _BreakoutBall_h


/** Ball X position */
byte breakoutBallPositionX = 20;

/** Ball Y position */
byte breakoutBallPositionY = 16;

/** Ball increment X amount - either +/-1 */
byte breakoutBallIncrementX = 1;

/** Ball increment Y amount - either +/-1 */
byte breakoutBallIncrementY = 1;

/** Maximum amount to add to X before resetting breakoutBallCounterX */
byte breakoutBallSlopeX = 5;

/** Maximum amount to add to Y before resetting breakoutBallCounterY */
byte breakoutBallSlopeY = 2;

/** Current position in slope */
byte breakoutBallCounterX = 5;

/** Current position in slope */
byte breakoutBallCounterY = 2;


/**
 * @param value[in] range of random number
 * @return a random value on [1, value]
 */
int random(int value);


/** Resets the breakoutBall position to the default */
void BreakoutBallReset();


/** Initializes the breakoutBall position to the default */
void BreakoutBallInit();


/** Display the current number of balls */
void BreakoutBallDrawCount();


/** Invoked when a ball is missed */
void BreakoutBallMiss();


/**
 * Determines whether the ball is colliding with bricks in lineBrickYPositions.
 * @param lineBrickXPos[in] x position of bricks
 * @param lineBrickYPositions[in/out] Y positions of a line of bricks.
 */
void BreakoutBallCheckBrickCollision(byte lineBrickXPos, byte *lineBrickYPositions);


/** Controls the breakoutBall motion */
void BreakoutBallTick();


/** Draw the breakout ball */
void BreakoutBallRefresh();


#endif
