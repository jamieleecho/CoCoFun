/*
  Blitter.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Blitting functions
*/


#ifndef _Blitter_c
#define _Blitter_c


#define BREAKOUT_SCORE_NUM_BYTES 4


void blitGraphics2(byte *bitmap, byte x, byte y) {
  // Bounds check
  if ((x > 159) || (y > 191))
    return;

  asm {
      ldx bitmap
      stx  MEMAR
      lda  x
      ldb  y
      std  XAXIS

      orcc      #$50
      pshs      u

* Map the graphics screen starting at 0x8000
* Squirrel away the MMU register settings
      ldx       #$ffa4
      ldu       mmu
      ldb       #$30

      lda       ,x
      sta       ,u+
      stb       ,x+
      incb
      lda       ,x
      sta       ,u+
      stb       ,x+
      incb
      lda       ,x
      sta       ,u+
      stb       ,x+
      incb
      lda       ,x
      sta       ,u+
      stb       ,x+
      incb

* Setup registers to point to memory
* U - src
* X - dst
* Y - current row
* b - running counter of number of bytes
      ldu       MEMAR
      clra
      ldb       YAXIS
      tfr       d,y
      lda       YAXIS
      ldb       #160
yloop mul
      addd      #$8000      
      tfr       d,X
      ldb       XAXIS
      abx

* Transfer one line of pixels
xloop lda       ,u+
      cmpa      #$ff
      beq       yend
      sta       ,x+
      incb
      cmpb      #160
      bcs       xloop

* Skip source bytes due to edge clipping
xskip lda       ,u+
      cmpa      #$ff
      bne       xskip

* Go to the next line
yend  lda       ,u
      cmpa      #$ff
      beq       end_blit
      leay      1,y
      cmpy      #192
      bge       end_blit
      tfr       y,d
      lda       #160
      bra       yloop

XAXIS	FCB	10
YAXIS	FCB	10
MEMAR	FDB	$E00
TEMPA	FDB	0
XCOUNT	FDB	0
BNK	FCB	0
mmu     rmb     4

end_blit
* Map the ROMs back to 0x8000
      ldx       #$ffa4
      ldu       mmu
      lda       ,u+
      sta       ,x+
      lda       ,u+
      sta       ,x+
      lda       ,u+
      sta       ,x+
      lda       ,u+
      sta       ,x+

end_blit2
* Enable interrupts
      andcc     #$af

      puls      u
  }
}


void blitNumericText(char *text, byte x, byte y) {
  for(char c = *text++; c != 0; c = *text++) {
    if (c < '0') c = '0';
    if (c > '9') c = '9';
    c = c - '0';
    blitGraphics2((byte *)GrafxDataNumberData + ((int)49 * (int)c), x, y);
    x += 3;
  }
}


#endif
