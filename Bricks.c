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
  bricksRemovedCount = 0;
}


void BricksRemove() {
  bricksRemovedCount++;
}


byte BricksAllGone() {
  return bricksRemovedCount >= (17 * 4);
}


void BricksDrawBricks() {
  // Draw graphic bars
  blitGraphics2(GrafxDataBricksData, 50, 2);
  blitGraphics2(GrafxDataBricksData, 65, 2);
  blitGraphics2(GrafxDataBricksData, 80, 2);
  blitGraphics2(GrafxDataBricksData, 95, 2);
}


#endif
