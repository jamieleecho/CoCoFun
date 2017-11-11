/*
  Bricks.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Represents the bricks in a Splinter game.
*/


#ifndef _Bricks_h
#define _Bricks_h

#include <coco.h>


/** X positions of all bricks */
#define brickXPositionsSz 7
extern byte brickXPositions[brickXPositionsSz];

/** brick sounds */
extern byte brickXSounds[brickXPositionsSz];

/** Y Positions of all bricks. This array is never modified */
#define brickYPositionsSz 17
extern byte brickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
extern byte line1BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
extern byte line2BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
extern byte line3BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
extern byte line4BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
extern byte line5BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
extern byte line6BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
extern byte line7BrickYPositions[brickYPositionsSz];

/** Array of all brick T positions */
extern byte *lineBrickYPositions[brickXPositionsSz];


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
