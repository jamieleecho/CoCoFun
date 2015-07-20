/*
  Bricks.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Represents the bricks in a Breakout game.
*/


#ifndef _Bricks_h
#define _Bricks_h


/** Positions of all bricks. This array is never modified */
#define brickYPositionsSz 17
byte brickYPositions[brickYPositionsSz] = {
  2, 13, 24, 35, 46, 57, 68, 79, 90,
  101, 112, 123, 134, 145, 156, 167,
  178
};


/** Positions of first line of bricks. Either position or 255 if brick is missing */
byte line1BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
byte line2BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
byte line3BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
byte line4BrickYPositions[brickYPositionsSz];

/** Initializes the Bricks data structures */
void BricksInit();

/** Resets the bricks for the beginning of a new level */
void BricksReset();

/** Removes a brick */
void BricksRemove();

/** @return 1 if all bricks were removed and 0 otherwise */
byte BricksAllGone();

/** Draw the bricks */
void BricksDrawBricks();

#endif
