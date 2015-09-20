/*
  Breakout.h
  
  This software is for demonstration purposes only. Use at your own
  risk.
*/


#ifndef _Breakout_h
#define _Breakout_h


/** No motion */
#define BreakoutLastDirectionNone (0)

/** Moving up */
#define BreakoutLastDirectionUp (1)

/** Moving down */
#define BreakoutLastDirectionDown (2)

/** Last direction of the paddle */
byte BreakoutLastDirection;

/** First direction of the paddle when both keys are detected */
byte BreakoutFirstDirection;

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
void BreakoutDrawBorderedScreen(byte f, byte b);


/** Shows the title screen */
void BreakoutShowTitleScreen();


/**
 * Draws a centered dialog box.
 * @param ww[in] width in character coordinates
 * @param hh[in] height in character coordinates
 * @param b background color [0, 15]
 * @param f foreground color [0, 15]
 */
void BreakoutDrawDialogBox(unsigned ww, unsigned hh, byte f, byte b);


/**
 * Clears a centered dialog box with color 0
 * @param ww[in] width in character coordinates
 * @param hh[in] height in character coordinates
 * @param b background color [0, 15]
 */
void BreakoutEraseDialogBox(unsigned ww, unsigned hh, byte b);


/** Tells the user the game is over */
void BreakoutShowGameOver();


/** Refresh the breakout board */
void BreakoutRefresh();

#endif
