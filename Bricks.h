/*
  Bricks.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Represents the bricks in a Breakout game.
*/


#ifndef _Bricks_h
#define _Bricks_h



/** X positions of all bricks */
#define brickXPositionsSz 7
byte brickXPositions[brickXPositionsSz] = {
  50, 59, 68, 77, 86, 95, 104
};

/** brick sounds */
byte brickXSounds[brickXPositionsSz] = {
  89, 108, 125, 133, 147, 159, 170
};


/** Y Positions of all bricks. This array is never modified */
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

/** Positions of first line of bricks. Either position or 255 if brick is missing */
byte line5BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
byte line6BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
byte line7BrickYPositions[brickYPositionsSz];


/** Array of all brick T positions */
byte *lineBrickYPositions[brickXPositionsSz] = {
  line1BrickYPositions, line2BrickYPositions, line3BrickYPositions,
  line4BrickYPositions, line5BrickYPositions, line6BrickYPositions,
  line7BrickYPositions
};


/** Initializes the Bricks data structures */
void BricksInit();

/** Resets the bricks for the beginning of a new level */
void BricksReset();

/** Removes a brick */
void BricksRemove();

/** @return 1 if all bricks were removed and 0 otherwise */
byte BricksAllGone();

/** Refreshes the display */
void BricksRefresh();

#endif
