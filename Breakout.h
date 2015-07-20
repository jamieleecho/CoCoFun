/*
  Breakout.h
  
  This software is for demonstration purposes only. Use at your own
  risk.
*/


#ifndef _Breakout_h
#define _Breakout_h


/** Score during a breakout game */
BreakoutScore breakoutScore;

/** Initializes Breakout */
void BreakoutInit();

/** Draws the current score on the screen */
void BreakoutDrawScore();

/** Video game loop resides here */
void BreakoutPlay();

/** Video game loop for one game of breakout */
void BreakoutPlayGame();

/** Moves the paddle up and down based on the joystick */
void BreakoutControlPaddle();


#endif
