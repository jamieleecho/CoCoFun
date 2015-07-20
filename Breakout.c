/*
  Breakout.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  The Breakout program was originally written in Assembly Language
  back around 1989 or so.
*/

#pragma org 0x4000
#include <cmoc.h>
#include <legacy.h>
#include <coco.h>

#include "BreakoutScore.c"
#include "GrafxData.c"


/** CoCo base palette register */
byte *cocoPaletteBaseReg = (byte *)0xffb0;

/** Number of palette registers */
#define COCO_NUM_PALETTE_REGISTERS 16

/** CoCo slow speed control register */
byte *cocoSlowSpeedReg = (byte *)0xffd8;

/** CoCo high speed control register */
byte *cocoHighSpeedReg = (byte *)0xffd9;


/** Breakout palette colors */
byte breakoutColorPalette[COCO_NUM_PALETTE_REGISTERS] = {
  127, 118, 110, 102, 89, 80, 79, 120,
  71, 98, 104, 96, 74, 66, 73, 0
};


/** Positions of all bricks. This array is never modified */
#define brickYPositionsSz 17
byte brickYPositions[brickYPositionsSz] = {
  2, 13, 24, 35, 46, 57, 68, 79, 90,
  101, 112, 123, 134, 145, 156, 167,
  178
};

/** Position of the paddle */
byte paddlePosition = 72;
    
/** Positions of first line of bricks. Either position or 255 if brick is missing */
byte line1BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
byte line2BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
byte line3BrickYPositions[brickYPositionsSz];

/** Positions of first line of bricks. Either position or 255 if brick is missing */
byte line4BrickYPositions[brickYPositionsSz];

/** current number of balls */
char numberOfBalls;


/** Video game loop resides here */
void playBreakout();

/** Video game loop for one game of breakout */
void playBreakoutGame();

/** Set up data structures for a new breakout level */
void playNewBreakoutLevel();

/** Video game loop for one ball of breakout */
void playBreakoutBall();

/** Blit graphics to the screen */
void blitGraphics();

/** Moves the paddle up and down based on the joystick */
void controlPaddle();

/**
 * Blits graphics located at bitmap to (x, y)
 * @param bitmap[input] location of bitmap data
 * @param x[input] x byte location
 * @param y[input] y pixel location
 */
void blitGraphics2(byte *bitmap, byte x, byte y);


/**
 * Blits numeric text data to (x, y)
 * @param text[input] null terminated text to blit
 * @param x[input] x byte location
 * @param y[input] y pixel location
 */
void blitNumericText(char *text, byte x, byte y) {
  for(char c = *text++; c != 0; c = *text++) {
    if (c < '0') c = '0';
    if (c > '9') c = '9';
    c = c - '0';
    blitGraphics2((byte *)GrafxDataNumberData + ((int)49 * (int)c), x, y);
    x += 3;
  }
}


/** Clear and show the graphics screen. */
void clearAndShowGraphicsScreen() {
  asm {
    ldb #2
    JSR	$E69E
  }
}


/**
 * Reads the joystick.
 *
 * @param x[out] x position
 * @param y[out] y position
 */
void readJoystick(byte *x, byte *y) {
  asm {
    PSHS U
    JSR	[$A00A]
    PULS U
    LDD	$15a
    ldx x
    ldy y
    sta ,x
    stb ,y
  }
}

BreakoutScore breakoutScore;

/** Draws the current score on the screen */
void BreakoutDrawScore() {
  char buffer[7];
  BreakoutScoreFormat(&breakoutScore, buffer);
  blitNumericText(buffer, 0x78, 0x50);
}


#include "BreakoutBall.c"



/** Initializes Breakout */
void initBreakout() {
  // High speed mode
  *cocoHighSpeedReg = 0;
  
  // Black out the screen
  memset(cocoPaletteBaseReg, 0, COCO_NUM_PALETTE_REGISTERS);

  // Show the graphics screen
  clearAndShowGraphicsScreen();

  // Initialize objects
  BreakoutScoreInit(&breakoutScore);
  BreakoutBallInit();

  // Set final palette
  memcpy(cocoPaletteBaseReg, breakoutColorPalette, COCO_NUM_PALETTE_REGISTERS);
}


/** Force certain functions to be emitted */
void forceFunctionsToBeEmitted() {
  if (0) {
    playNewBreakoutLevel();
    blitGraphics();
    controlPaddle();
  }
}


int main() {
  forceFunctionsToBeEmitted();

  initBreakout();
  playBreakout();

  return 0;
}


void playBreakout() {
  while(1) {
    playBreakoutGame();
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


void blitGraphics() {
  asm {
BLITTER	LDA	XAXIS
      CMPA	#159
      LBHI	END
      LDB	YAXIS
      LDX	MEMAR
      STA	TEMPA+1
      CLR	XCOUNT+1
FIND	PSHS	A
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
      PULS	A
CALC	PSHS	D
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
PULL	PULS	D
CHEK1	ADDA	XCOUNT+1
      CMPA	#159
      BHI	CUTOFF
CHEK2	LDA	,X+
      CMPA	#$FF
      BEQ	ADDY
STA	STA	,Y+
      INC	XCOUNT+1
      CMPY	#$8000
      BNE	GOON
      INC	$FFA3
      LEAY	-$2000,Y
GOON	LDA	TEMPA+1
      BRA	CHEK1
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
LOONY	LDA	#32
      CMPA	TEMPA+1
      LBLO	CONT1
      LBRA	SEL1
LOON	LDA	#64
      CMPA	TEMPA+1
      LBLO	CONT2
      LBRA	SEL2
LOON2	LDA	#96
      CMPA	TEMPA+1
      LBLO	CONT3
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


void playBreakoutGame() {
  BreakoutScoreReset(&breakoutScore);
  BreakoutBallReset();

  // Initialize men, etc
  asm {
    ORCC #80
    LDD	#0
    STD	SCORE
    STD	SCORE+2
    STD	SCORE+4
    STD	SCORFMT
    STD	SCORFMT+2
    STD	SCORFMT+4
  }

  numberOfBalls = 9;  

  BreakoutDrawScore();  
  BreakoutBallDrawCount();

  // Play breakout until we run out of balls
  while(numberOfBalls > 0) {
    playBreakoutBall();
  }
}


void playNewBreakoutLevel() {
  memcpy(line1BrickYPositions, brickYPositions, brickYPositionsSz);
  memcpy(line2BrickYPositions, brickYPositions, brickYPositionsSz);
  memcpy(line3BrickYPositions, brickYPositions, brickYPositionsSz);
  memcpy(line4BrickYPositions, brickYPositions, brickYPositionsSz);
  clearAndShowGraphicsScreen();

  asm {
	LDA	#20
	LDB	#2
	STD	XX
	CLR	COBOL
	LDA	#$FF
	STA	$FF9A
  }

  // Draw graphic bars
  blitGraphics2(GrafxDataBricksData, 50, 2);
  blitGraphics2(GrafxDataBricksData, 65, 2);
  blitGraphics2(GrafxDataBricksData, 80, 2);
  blitGraphics2(GrafxDataBricksData, 95, 2);

  asm {
* Draw Score
	LBSR	CTART

* Draw number of balls
	LDA	numberOfBalls
	LDB	#255
	STD	SCORFMT
	LDD	#$786F
	STD	XAXIS
	LDX	#SCORFMT
	LBSR	BLIT_TEXT
  }
}


/** Moves the paddle up and down based on the joystick */
void controlPaddle() {
  byte joyX, joyY;
  readJoystick(&joyX, &joyY);
  if (joyY < 20) {
    if (paddlePosition > 2)
      paddlePosition -= 2;
  } else if (joyY > 40) {
    if (paddlePosition < 146)
      paddlePosition += 2;
  }
  blitGraphics2(GrafxDataPaddleData, 4, paddlePosition);
}


/** Breakout video game */
void playBreakoutBall() {
  asm {
                pragma  cescapes
        LBRA    START

SCORFMT	FCC	"\0\0\0\0\0\0\0\0\xff"
TMPACCUM  fdb     0

* Scratch pad
TMPYPOS   FDB     0

* Returns a random number from 1 (0?) to 3
* in the D register
RAND3 	LDD	#3
 	JSR	$B4F4
 	JSR	$BF1F
 	JSR	$B3ED
	RTS

* BLIT text located at SCORFMT at score location until 0xff hit
BLIT_SCORE LDX	#SCORFMT
	LDD	#$7850
	STD	XAXIS
BLIT_TEXT LDA	,X+
* Start of graphics for numbers
* Numbers separated by 49 bytes
	CMPA	#255
	BEQ	CEND
        CMPA    #9
        BLS     COMPUTE_OFFSET
        LDA     #9

COMPUTE_OFFSET
        LDB     #49
        MUL
        STD     TMPACCUM
        LEAY	GrafxDataNumberData
        TFR     Y,D
        ADDD    TMPACCUM
        
	STD	MEMAR
	PSHS	X
	LBSR	BLITTER
	PULS	X
	INC	XAXIS
	INC	XAXIS
	INC	XAXIS
	LBSR	BLIT_TEXT
CEND	RTS
SCORE	FDB	0
	FDB	0
	FDB	0
RESULT	FDB	0
	FDB	0
CTART	LDX	#SCORE+3
	LDY	#RESULT+3
	ANDCC	#$FE
SUN	LDA	,-X
	ADCA	#$10
	DAA
	STA	,-Y
	LDA	,-X
	ADCA	#0
	DAA
	STA	,-Y
	LDA	,-X
	ADCA	#0
	DAA
	STA	,-Y
CEGINR	LDA	RESULT
	STA	SCORE
	PSHS	A
	ANDA	#240
	RORA
	RORA
	RORA
	RORA
	CMPA	#10
	BLO	JNXT
	CLRA
JNXT	TFR	A,B
	PULS	A
	ANDA	#15
	EXG	A,B
	STD	SCORFMT
CEGNA	LDA	RESULT+1
	STA	SCORE+1
	PSHS	A
	ANDA	#240
	RORA
	RORA
	RORA
	RORA
	ANDA	#15
	TFR	A,B
	PULS	A
	ANDA	#15
	EXG	A,B
	STD	SCORFMT+2
CEGNB	LDA	RESULT+2
	STA	SCORE+2
	PSHS	A
	ANDA	#240
	RORA
	RORA
	RORA
	RORA
	TFR	A,B
	PULS	A
	ANDA	#15
	EXG	A,B
	STD	SCORFMT+4
	LDA	#255
	STA	SCORFMT+6
	LBSR	BLIT_SCORE
	RTS

* Ball slope and current point in slope
SLPX	FCB	5
SLPY	FCB	2
FLGX	FCB	5
FLGY	FCB	2

* Amount to add to XX and YY
ADX	FCB	1
ADY	FCB	1

* 
XX	FCB	20
YY	FCB	16


START
HERE	LBSR _playNewBreakoutLevel
REJIN	LDA	#30
	LDB	#3
	STD	XX
	LDD	#$0105
	STD	SLPX
	STD	FLGX
REGIN	LBSR	_controlPaddle
        LBSR    _BreakoutBallTick
	LEAX	GrafxDataBallData
	STX	MEMAR
GLGL	LDA	FLGX
	TSTA
	BEQ	TDK
	DECA
	STA	FLGX
	LDB	XX
	ADDB	ADX
	CMPB	#110
	BHI	NXT
	CMPB	#5
	LBLO	ZZTP
	STB	XAXIS
  // STB	XX
TDK	LDA	FLGY
	TSTA
	LBEQ	RCHK
	DECA
	STA	FLGY
	LDB	YY
	ADDB	ADY
	CMPB	#179
	BHI	NXT1
	CMPB	#2
	BLO	NTX1
	STB	YAXIS
  //	STB	YY
	LDB	XX
	STB	XAXIS
MCDLT	//LBSR	BLITTER
	LDA	XX
	CMPA	#43
	LBLO	GLOR
	CMPA	#50
	LBLO	FNCK
GLOR	CMPA	#58
	LBLO	GLR
	CMPA	#65
	LBLO	FNCK1
GLR	CMPA	#73
	LBLO	GLOR2
	CMPA	#80
	LBLO	FNCK2
GLOR2	CMPA	#88
	LBLO	GLOR1
	CMPA	#95
	LBLO	FNCK3
GLOR1	JSR	[$A000]
	CMPA	#3
	LBEQ	PAUSE
TAG	LDA	#68
	CMPA	COBOL
	LBNE	REGIN
	LBRA	HERE
NXT	LDA	#-1
	STA	ADX
	LBRA	REGIN
NTX	LDA	#1
	STA	ADX
	LBRA	REGIN
NXT1	LDA	#-1
	STA	ADY
	LBRA	DUKE
NTX1	LDA	#1
	STA	ADY
	LBRA	DUKE
ZZTP	LDA	paddlePosition
	CMPA	#7
	BHI	BLOB
	ADDA	#32
	BRA	BLAR
BLOB	CMPA	YY
	BNE	ZZZZ
        lbsr    RAND3
	STB	SLPX
	BRA	NTX
ZZZZ	SUBA	#7
	CMPA	YY
	BHI	MISS
	ADDA	#44
BLAR	CMPA	YY
	BLO	MISS
        lbsr    RAND3
	STB	SLPX
	BRA	NTX
MISS	
	LDA	numberOfBalls
        DECA
  //STA     numberOfBalls
	LDB	#$FF
	STD	SCORFMT
	LDX	#SCORFMT
	LDD	#$786F
	STD	XAXIS
	LBSR	BLIT_TEXT
	LEAY	GrafxDataBlankData
	STY	MEMAR
	LDD	XX
	STD	XAXIS
	LBSR	BLITTER
	LDA	numberOfBalls
	TSTA
	BEQ	POLIN
	LBRA	REJIN
POLIN	JSR	[$A000]
	CMPA	#0
	BEQ	POLIN
	LBRA	GAMEOVER
PAUSE	JSR	[$A000]
	CMPA	#3
	BNE	PAUSE
	LBRA	TAG
RCHK	LDA	FLGX
	TSTA
	BEQ	RESET
DUKE	LDD	XX
	STD	XAXIS
	LBRA	MCDLT
RESET	LDD	SLPX
	STD	FLGX
	LBRA	GLGL

FNCK	LEAX	line1BrickYPositions
        LEAX    17,x
        stx     TMPYPOS
        LEAX	line1BrickYPositions
LDCK	LDA	,X
	CMPA	YY
	BEQ	CLRBLK
	LBLO	FRCK
GOOP	BHI	RCK
OVFLW	LEAX	1,X
	CMPX	TMPYPOS
	BNE	LDCK
	LBRA	GLOR
RCK	LDA	YY
	ADDA	#11
	STA	YY
	LDA	,X
	CMPA	YY
	BLO	FIXY
	LDB	YY
	SUBB	#11
	STB	YY
	BRA	OVFLW
FIXY	LDB	YY
	SUBB	#11
	STB	YY
CLRBLK	LDB	#255
	PSHS	X
	STB	,X
        LEAY	GrafxDataBlankData
	STY	MEMAR
	TFR	A,B
	LDA	#50
	STD	XAXIS
	LBSR	BLITTER
	LBSR	CTART
	INC	COBOL
        lbsr    RAND3
	PSHS	D
	ANDB	#1
	TSTB
	BEQ	MRM
	LDA	ADX
	CMPA	#1
	BNE	MRM
	LDA	#-1
	STA	ADX
MRM	PULS	D
	STB	SLPX	
        lbsr    RAND3
	STB	SLPY
	LDD	#$FF01
	STA	$8C
	JSR	$A951
	PULS	X
	LBRA	OVFLW
FRCK	ADDA	#14
	CMPA	YY
	BHI	FREEZE
	LBRA	GOOP
FREEZE	SUBA	#14
	BRA	CLRBLK
FNCK1	LEAX	line2BrickYPositions
        LEAX    17,x
        stx     TMPYPOS,PCR
        LEAX	line2BrickYPositions
LDCK1	LDA	,X
	CMPA	YY
	BEQ	CLRBL1
	LBLO	FRCK1
GOOP1	BHI	RCK1
OVFLW1	LEAX	1,X
	CMPX	TMPYPOS
	BNE	LDCK1
	LBRA	GLOR1
RCK1	LDA	YY
	ADDA	#11
	STA	YY
	LDA	,X
	CMPA	YY
	BLO	FIXY1
	LDB	YY
	SUBB	#11
	STB	YY
	BRA	OVFLW1
FIXY1	LDB	YY
	SUBB	#11
	STB	YY
CLRBL1	LDB	#255
	PSHS	X
	STB	,X
        LEAY	GrafxDataBlankData
	STY	MEMAR
	TFR	A,B
	LDA	#65
	STD	XAXIS
	LBSR	BLITTER
	LBSR	CTART
	INC	COBOL
        lbsr    RAND3
	PSHS	D
	ANDB	#1
	TSTB
	BEQ	MRMR
	LDA	ADX
	CMPA	#1
	BNE	MRMR
	LDA	#-1
	STA	ADX
MRMR	PULS	D
	STB	SLPX	
        lbsr    RAND3
	PSHS	B
	ANDB	#1
	TSTB
	BEQ	MEME
MEME	PULS	B
	STB	SLPY
	LDD	#$FF01
	STA	$8C
	JSR	$A951
	PULS	X
	LBRA	OVFLW1
FRCK1	ADDA	#14
	CMPA	YY
	BHI	FREEZ1
	LBRA	GOOP1
FREEZ1	SUBA	#14
	BRA	CLRBL1
FNCK2	LEAX	line3BrickYPositions
        LEAX    17,x
  stx     TMPYPOS
        LEAX	line3BrickYPositions
LDCK2	LDA	,X
	CMPA	YY
	BEQ	CLRBL2
	LBLO	FRCK2
GOOP2	BHI	RCK2
OVFLW2	LEAX	1,X
	CMPX	TMPYPOS
	BNE	LDCK2
	LBRA	GLOR1
RCK2	LDA	YY
	ADDA	#11
	STA	YY
	LDA	,X
	CMPA	YY
	BLO	FIXY2
	LDB	YY
	SUBB	#11
	STB	YY
	BRA	OVFLW2
FIXY2	LDB	YY
	SUBB	#11
	STB	YY
CLRBL2	LDB	#255
	PSHS	X
	STB	,X
        LEAY	GrafxDataBlankData
	STY	MEMAR
	TFR	A,B
	LDA	#80
	STD	XAXIS
	LBSR	BLITTER
	LBSR	CTART
	INC	COBOL
        lbsr    RAND3
	PSHS	D
	ANDB	#1
	TSTB
	BEQ	MRMR1
	LDA	ADX
	CMPA	#1
	BNE	MRMR1
	LDA	#-1
	STA	ADX
MRMR1	PULS	D
	STB	SLPX	
        lbsr    RAND3
	PSHS	B
	ANDB	#1
	TSTB
	BEQ	MEME1
MEME1	PULS	B
	STB	SLPY
	LDD	#$FF01
	STA	$8C
	JSR	$A951
	PULS	X
	LBRA	OVFLW2
FRCK2	ADDA	#14
	CMPA	YY
	BHI	FREEZ2
	LBRA	GOOP2
FREEZ2	SUBA	#14
	BRA	CLRBL2
FNCK3	LEAX	line4BrickYPositions
        LEAX    17,x
        stx     TMPYPOS
        LEAX	line4BrickYPositions
LDCK3	LDA	,X
	CMPA	YY
	BEQ	CLRBL3
	LBLO	FRCK3
GOOP3	BHI	RCK3
OVFLW3	LEAX	1,X
	CMPX	TMPYPOS
	BNE	LDCK3
	LBRA	GLOR1
RCK3	LDA	YY
	ADDA	#11
	STA	YY
	LDA	,X
	CMPA	YY
	BLO	FIXY3
	LDB	YY
	SUBB	#11
	STB	YY
	BRA	OVFLW3
FIXY3	LDB	YY
	SUBB	#11
	STB	YY
CLRBL3	LDB	#255
	PSHS	X
	STB	,X
 	LEAY	GrafxDataBlankData
	STY	MEMAR
	TFR	A,B
	LDA	#95
	STD	XAXIS
	LBSR	BLITTER
	LBSR	CTART
	INC	COBOL
        lbsr    RAND3
	PSHS	D
	ANDB	#1
	TSTB
	BEQ	MRMR2
	LDA	ADX
	CMPA	#1
	BNE	MRMR2
	LDA	#-1
	STA	ADX
MRMR2	PULS	D
	STB	SLPX	
        lbsr    RAND3
	PSHS	B
	ANDB	#1
	TSTB
	BEQ	MEME2
MEME2	PULS	B
	STB	SLPY
	LDD	#$FF01
	STA	$8C
	JSR	$A951
	PULS	X
	LBRA	OVFLW3
FRCK3	ADDA	#14
	CMPA	YY
	BHI	FREEZ3
	LBRA	GOOP3
FREEZ3	SUBA	#14
	BRA	CLRBL3
COBOL	FCB	0
GAMEOVER
  }
}
