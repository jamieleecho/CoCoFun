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

/** Show the title screen */
void BreakoutShowMonitorScreen();

/** Draws the current score on the screen */
void BreakoutDrawScore();

/** Video game loop resides here */
void BreakoutPlay();

/** Video game loop for one game of breakout */
void BreakoutPlayGame();

/** Moves the paddle up and down based on the joystick */
void BreakoutControlPaddle();

/** Pause the game */
void BreakoutPauseGame();

/** 
 * Draws a bordered screen with the given.
 * @param b background color [0, 15]
 * @param f foreground color [0, 15]
 */
void BreakoutDrawBorderedScreen(byte b, byte f);


/** Shows the title screen */
void BreakoutShowTitleScreen();


#endif
