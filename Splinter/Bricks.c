/*
  Bricks.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Represents the bricks in a Splinter game.
*/


#include <coco.h>

#include "Blitter.h"
#include "Bricks.h"


/** X positions of all bricks */
byte brickXPositions[brickXPositionsSz] = {
  50, 59, 68, 77, 86, 95, 104
};

/** brick sounds */
byte brickXSounds[brickXPositionsSz] = {
  9, 8, 7, 6, 5, 4, 3
};

/** Y Positions of all bricks. This array is never modified */
byte brickYPositions[brickYPositionsSz] = {
  2, 13, 24, 35, 46, 57, 68, 79, 90,
  101, 112, 123, 134, 145, 156, 167,
  178
};

/** Number of bricks removed */
byte bricksRemovedCount = 0;

/** Colors to use for the bricks */
byte brickColors[] = { 1, 3, 9, 5, 4, 14, 10 };

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


void BricksInit() {
  BricksReset();
}


void BricksReset() {
  memcpy(line1BrickYPositions, brickYPositions, brickYPositionsSz);
  memcpy(line2BrickYPositions, brickYPositions, brickYPositionsSz);
  memcpy(line3BrickYPositions, brickYPositions, brickYPositionsSz);
  memcpy(line4BrickYPositions, brickYPositions, brickYPositionsSz);
  memcpy(line5BrickYPositions, brickYPositions, brickYPositionsSz);
  memcpy(line6BrickYPositions, brickYPositions, brickYPositionsSz);
  memcpy(line7BrickYPositions, brickYPositions, brickYPositionsSz);
  bricksRemovedCount = 0;
}


void BricksRemove() {
  bricksRemovedCount++;
}


byte BricksAllGone() {
  return bricksRemovedCount >= (brickYPositionsSz * brickXPositionsSz);
}


void BricksRefresh() {
  for (byte ii=0; ii<brickXPositionsSz; ii++) {
    byte xpos = 2 * brickXPositions[ii];
    byte color = brickColors[ii];
    byte *brickYPositions = lineBrickYPositions[ii];
    for (byte jj=0; jj<brickYPositionsSz; jj++) {
      byte brickYPosition = brickYPositions[jj];
      if (brickYPosition != 0xff) {
	BlitterFillRectangle(xpos, brickYPosition, 5, 10, 7);
	BlitterFillRectangle(xpos+1, brickYPosition+1, 3, 8, color);
      }
    }
  }
}
