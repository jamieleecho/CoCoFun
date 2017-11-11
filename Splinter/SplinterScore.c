/*
  SplinterScore.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Represents the score in a Splinter game.
*/


#include "Splinter.h"
#include "SplinterScore.h"


void SplinterScoreInit(SplinterScore *score) {
  memset(score, 0, sizeof(*score));
}


void SplinterScoreReset(SplinterScore *score) {
  memset(score, 0, sizeof(*score));
}


void SplinterScoreIncrement(SplinterScore *score, byte amount) {
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


void SplinterScoreFormat(SplinterScore *score, char *dst) {
  for(byte ii=1; ii<SPLINTER_SCORE_NUM_BYTES; ii++) {
    dst[2 * (ii - 1)] = ((score->score[ii] >> 4) & 0xf) + '0';
    dst[2 * (ii - 1) + 1] = (score->score[ii] & 0xf) + '0';
  }
  dst[2 * (SPLINTER_SCORE_NUM_BYTES - 1)] = 0;
}
