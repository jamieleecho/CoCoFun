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
  return bricksRemovedCount >= (17 * brickXPositionsSz);
}


void BricksDrawBricks() {
  for (byte ii=0; ii<brickXPositionsSz; ii++)
    BlitterDrawGraphics(GrafxDataBricksData[ii], brickXPositions[ii], 2);
}


#endif
