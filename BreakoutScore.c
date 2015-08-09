/*
  BreakoutScore.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Represents the score in a Breakout game.
*/


#ifndef _BreakoutScore_c
#define _BreakoutScore_c


#include "BreakoutScore.h"


void BreakoutScoreInit(BreakoutScore *score) {
  memset(score, 0, sizeof(*score));
}


void BreakoutScoreReset(BreakoutScore *score) {
  memset(score, 0, sizeof(*score));
}


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


void BreakoutScoreFormat(BreakoutScore *score, char *dst) {
  for(byte ii=1; ii<BREAKOUT_SCORE_NUM_BYTES; ii++) {
    dst[2 * (ii - 1)] = ((score->score[ii] >> 4) & 0xf) + '0';
    dst[2 * (ii - 1) + 1] = (score->score[ii] & 0xf) + '0';
  }
  dst[2 * (BREAKOUT_SCORE_NUM_BYTES - 1)] = 0;
}


#endif
