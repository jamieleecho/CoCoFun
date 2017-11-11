/*
  SplinterScore.h
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Represents the score in a Splinter game.
*/


#ifndef _SplinterScore_h
#define _SplinterScore_h

#include <coco.h>


#define SPLINTER_SCORE_NUM_BYTES 4


/** Score data structure */
typedef struct SplinterScore {
  unsigned byte score[SPLINTER_SCORE_NUM_BYTES];
} SplinterScore;


/**
 * Initialized score to zero.
 * @param score[in/out] score to initialize to zero.
 */
void SplinterScoreInit(SplinterScore *score);


/**
 * Sets score to zero
 * @param score[in/out] score to set to zero
 */
void SplinterScoreReset(SplinterScore *score);


/**
 * Increments score by amount
 * @param score[in/out] score to set to zero
 * @param amount[in] amount to increment score on [0-99].
 */
void SplinterScoreIncrement(SplinterScore *score, byte amount);


/**
 * Formats score into dst for printing.
 * @param score[in] score to format
 * @param dst[out] destination location that must contain at least 7 bytes
 */
void SplinterScoreFormat(SplinterScore *score, char *dst);


#endif
