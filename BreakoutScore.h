/*
  BreakoutScore.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Represents the score in a Breakout game.
*/


#ifndef _BreakoutScore_h
#define _BreakoutScore_h


#define BREAKOUT_SCORE_NUM_BYTES 4


/** Score data structure */
typedef struct BreakoutScore {
  unsigned byte score[BREAKOUT_SCORE_NUM_BYTES];
} BreakoutScore;


/**
 * Initialized score to zero.
 * @param score[in/out] score to initialize to zero.
 */
void BreakoutScoreInit(BreakoutScore *score);


/**
 * Sets score to zero
 * @param score[in/out] score to set to zero
 */
void BreakoutScoreReset(BreakoutScore *score);


/**
 * Increments score by amount
 * @param score[in/out] score to set to zero
 * @param amount[in] amount to increment score on [0-99].
 */
void BreakoutScoreIncrement(BreakoutScore *score, byte amount);


/**
 * Formats score into dst for printing.
 * @param score[in] score to format
 * @param dst[out] destination location that must contain at least 7 bytes
 */
void BreakoutScoreFormat(BreakoutScore *score, char *dst);


#endif
