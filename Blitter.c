/*
  Blitter.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Blitting functions
*/


#ifndef _Blitter_c
#define _Blitter_c


#define BREAKOUT_SCORE_NUM_BYTES 4


void blitGraphics() {
  asm {
      LDA	XAXIS
      CMPA	#159
      LBHI	END
      LDB	YAXIS
      LDX	MEMAR
      STA	TEMPA+1
      CLR	XCOUNT+1
FIND
      CMPB	#51
      BLO	SEL1
      CMPB	#51
      LBEQ	LOONY
CONT1	SUBB	#51
      CMPB	#51
      BLO	SEL2
      CMPB	#51
      LBEQ	LOON
CONT2	SUBB	#51
      CMPB	#51
      BLO	SEL3
      CMPB	#51
      LBEQ	LOON2
CONT3	SUBB	#51
      LDA	#$33
      BRA	STBNK
SEL1	LDA	#$30
      BRA	STBNK
SEL2	LDA	#$31
      BRA	STBNK
SEL3	LDA	#$32
STBNK	STA	$FFA3
      STA	BNK
      LDA       XAXIS
      LDA	#160
      MUL
      ADDD	TEMPA
      ADDD	#$6000
TRANS	TFR	D,Y
      LDA	BNK
      CMPA	#$31
      BEQ	AD192
      CMPA	#$32
      BEQ	AD384
      CMPA	#$33
      BEQ	AD576
      BRA	PULL
AD192	LEAY	-32,Y
      BRA	PULL
AD384	LEAY	-64,Y
      BRA	PULL
AD576	LEAY	-96,Y
PULL	LDB     YAXIS
CHEK1 LDA       XAXIS
      ADDA	XCOUNT+1
      CMPA	#159
      BHI	CUTOFF
CHEK2	LDA	,X+
      CMPA	#$FF
      BEQ	ADDY
STA	STA	,Y+
      INC	XCOUNT+1
      CMPY	#$8000
      BNE	CHEK1
      INC	$FFA3
      LEAY	-$2000,Y
CUTOFF	LDA	,X+
      CMPA	#$FF
      BNE	CUTOFF
ADDY	LDA	,X+
      CMPA	#$FF
      BEQ	END
      LEAY	160,Y
      EXG	Y,D
      SUBD	XCOUNT
      CLR	XCOUNT+1
      EXG	Y,D
      CMPY	#$8000
      BLO	STA
      LEAY	-$2000,Y
      INC	$FFA3
      BRA	STA
LOONY	LDA	TEMPA+1
      CMPA	#32
      LBGE	CONT1
      LBRA	SEL1
LOON	LDA	TEMPA+1
      CMPA	#64
      LBGE	CONT2
      LBRA	SEL2
LOON2	LDA	TEMPA+1
      CMPA	#96
      LBGE	CONT3
      LBRA	SEL3
XAXIS	FCB	10
YAXIS	FCB	10
MEMAR	FDB	$E00
TEMPA	FDB	0
XCOUNT	FDB	0
BNK	FCB	0
END
      }
}


void blitGraphics2(byte *bitmap, byte x, byte y) {
  asm {
      ldx bitmap
      stx  MEMAR
      lda  x
      ldb  y
      std  XAXIS
  }
  blitGraphics();
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
