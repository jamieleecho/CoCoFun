/*
  Splinter.h
  
  This software is for demonstration purposes only. Use at your own
  risk.
*/


#ifndef _Splinter_h
#define _Splinter_h

#include "SplinterScore.h"

/** No motion */
#define SplinterLastDirectionNone (0)

/** Moving up */
#define SplinterLastDirectionUp (1)

/** Moving down */
#define SplinterLastDirectionDown (2)

/** Last direction of the paddle */
byte SplinterLastDirection;

/** First direction of the paddle when both keys are detected */
byte SplinterFirstDirection;

/** Score during a splinter game */
SplinterScore splinterScore;

/** Initializes Splinter */
void SplinterInit();

/** Show the title screen */
void SplinterShowMonitorScreen();

/** Draws the current score on the screen */
void SplinterDrawScore();

/** Video game loop resides here */
void SplinterPlay();

/** Video game loop for one game of splinter */
void SplinterPlayGame();

/** Moves the paddle up and down based on the joystick */
void SplinterControlPaddle();

/** Pause the game */
void SplinterPauseGame();

/** 
 * Draws a bordered screen with the given.
 * @param b background color [0, 15]
 * @param f foreground color [0, 15]
 */
void SplinterDrawBorderedScreen(byte f, byte b);


/** Shows the title screen */
void SplinterShowTitleScreen();


/**
 * Draws a centered dialog box.
 * @param ww[in] width in character coordinates
 * @param hh[in] height in character coordinates
 * @param b background color [0, 15]
 * @param f foreground color [0, 15]
 */
void SplinterDrawDialogBox(unsigned ww, unsigned hh, byte f, byte b);


/**
 * Clears a centered dialog box with color 0
 * @param ww[in] width in character coordinates
 * @param hh[in] height in character coordinates
 * @param b background color [0, 15]
 */
void SplinterEraseDialogBox(unsigned ww, unsigned hh, byte b);


/** Tells the user the game is over */
void SplinterShowGameOver();


/** Refresh the splinter board */
void SplinterRefresh();

#endif
