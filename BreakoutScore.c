/*
  BreakoutScore.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Represents the score in a Breakout game.
*/


#ifndef _BreakoutScore_c
#define _BreakoutScore_c


#define BREAKOUT_SCORE_NUM_BYTES 4


/** Score data structure */
typedef struct BreakoutScore {
  unsigned byte score[BREAKOUT_SCORE_NUM_BYTES];
} BreakoutScore;


/**
 * Initialized score to zero.
 * @param score[in/out] score to initialize to zero.
 */
void BreakoutScoreInit(BreakoutScore *score) {
  for(int ii=0; ii<BREAKOUT_SCORE_NUM_BYTES; ii++)
    score->score[ii] = 0;
}


/**
 * Sets score to zero
 * @param score[in/out] score to set to zero
 */
void BreakoutScoreReset(BreakoutScore *score) {
  for(int ii=0; ii<BREAKOUT_SCORE_NUM_BYTES; ii++)
    score->score[ii] = 0;
}


/**
 * Increments score by amount
 * @param score[in/out] score to set to zero
 * @param amount[in] amount to increment score on [0-99].
 */
void BreakoutScoreIncrement(BreakoutScore *score, byte amount) {
    asm {
      ldx  score
      lda amount
      leax 3,x
      adda ,x
      daa
      sta  ,x
NextDigit lda  ,-x
      adca #0
      daa
      sta  ,x
      cmpx score
      blt  NextDigit
  }
}


/**
 * Formats score into dst for printing.
 * @param score[in] score to format
 * @param dst[out] destination location that must contain at least 7 bytes
 */
void BreakoutScoreFormat(BreakoutScore *score, char *dst) {
  for(int ii=1; ii<BREAKOUT_SCORE_NUM_BYTES; ii++) {
    dst[2 * (ii - 1)] = ((score->score[ii] >> 4) & 0xf) + '0';
    dst[2 * (ii - 1) + 1] = (score->score[ii] & 0xf) + '0';
  }
  dst[2 * (BREAKOUT_SCORE_NUM_BYTES - 1)] = 0;
}


#endif
