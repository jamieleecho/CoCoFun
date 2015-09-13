/*
  Bricks.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Represents the bricks in a Breakout game.
*/


#ifndef _Bricks_c
#define _Bricks_c

#include "Bricks.h"


/** Number of bricks removed */
byte bricksRemovedCount = 0;


/** Colors to use for the bricks */
byte brickColors[] = { 1, 3, 9, 5, 4, 14, 10 };


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
      if (brickYPosition != 0xff)
	BlitterFillRectangle(xpos, brickYPosition, 4, 11, color);
    }
  }
}


#endif
