#include <cmoc.h>
#include <coco.h>

#include "CoCoMisc.h"


void PlayBalloon();
void SetupIRQ();


int main() {
  SetupIRQ();
  PlayBalloon();
  return 0;
}


asm void PlayBalloon() {
  asm {
        LBRA START_GAME
AUMDUM	FDB	3616
BCHAR	FCB	34
BXAX	FDB	0
BYAX	FCB	0
BHRST	FDB	1024
CMEMRY	FDB	$2000
CNUMBR	FDB	$FFFF
DPLYPC	FDB	$5000
DPLYPS	FDB	$6000
DWASTE	FCB	0
DTEMP	FCB	0
EPLYPC	FDB	$5000
EPLYPS	FDB	$E00
EWASTE	FCB	0
ETEMP	FCB	0
ETEMP1	FCB	0
FNUM	FCB	5
FSTPNT	FDB	$E00
FPTPNT	FDB	1024
GNUM	FCB	4
GSTPNT	FDB	$E00
GPTPNT	FDB	1024
HGOOD	FCB	0
HBFRE	FCB	154
HYGD	FCB	154
HSTAT	FCB	1
	FCB	0
IXBD	FCB	0
IYBD	FCB	0
IFRMR	FDB	0
ISTR	FCB	0
IFLAG	FCB	0
FLAG	FCB	0
GVR	FCC	/GAME*OVER/
	FCB	0
SCORE	FDB	0
	FDB	0
MEN	FCB	3
RESULT	FDB	0
	FDB	0
ITEMP	FDB	0
BANK1	FCB	0
BANK2	FCB	0
ZTEMP	FDB	0
ATART	LDX	#3584
	LDY	#31
ASUB1	LDA	3584,Y
ASUB2	LDB	,X
	STA	,X+
	TFR	B,A
AMMY	CMPX	AUMDUM
	BNE	ASUB2
	LEAY	+32,Y
	LDD	AUMDUM
	ADDD	#32
	STD	AUMDUM
	CMPY	#6175
	BNE	ASUB1
	LDD	#3616
	STD	AUMDUM
	RTS

*******************************************************************************
* DRAWS TEXT LOCATED IN BHRST UNTIL 0 FOUND
*******************************************************************************
BEGIN	LDU	BHRST
BLDA	LDA	,U+
	STA	BCHAR
	TSTA
	BEQ	BSTP
	BSR	BST
	INC	BXAX+1
	BRA	BLDA
BSTP	LDU	#0
	RTS

*******************************************************************************
* DRAW A CHARACTER
*******************************************************************************
BST	LDA	#31
	ANDA	BXAX+1
	STA	BXAX+1
	LDA	#23
	ANDA	BYAX
	STA	BYAX
	LDB	#8
	MUL
	LDA	#32
	MUL
	ADDD	#$E00
	ADDD	BXAX
        TFR	D,X
	LDB	BCHAR
	SUBB	#33
	LDA	#8
	MUL
	ADDD	#61605
	TFR	D,Y
	LDB	#8
BSUB1	LDA	,Y+
	STA	,X
	DECB
	BEQ	BSTOP
        LEAX    32,X
	BRA	BSUB1
BSTOP	RTS

*******************************************************************************
* SOUND ROUTINE
*******************************************************************************
PLAY_SOUND
	LDX	CMEMRY
	LDA	$FF01
	ANDA	#$F7
	STA	$FF01
	LDA	$FF03
	ANDA	#$F7
	STA	$FF03
	LDA	$FF23
	ORA	#8
	STA	$FF23
	LDY	CNUMBR
CJUNK	LDB	,X+
	ANDB	#$FC
	STB	$FF20
	LEAY	-1,Y
	BNE	CJUNK
	RTS

DBEGIN	LDX	DPLYPC
	LDY	DPLYPS
	LDA	,X+
	LDB	,Y
	STA	DTEMP
	ANDA	#3
	TSTA
	BNE	DEX000
	PSHS	B
	ANDB	#3
	LDA	DTEMP
	STB	DWASTE
	ORA	DWASTE
	PULS	B
	BRA	DX000
DEX000	LDA	DTEMP
DX000	STA	DTEMP
	ANDA	#12
	TSTA
	BNE	DEX001
	PSHS	B
	ANDB	#12
	LDA	DTEMP
	STB	DWASTE
	ORA	DWASTE
	PULS	B
	BRA	DX001
DEX001	LDA	DTEMP
DX001	STA	DTEMP
	ANDA	#48
	TSTA
	BNE	DEX002
	PSHS	B
	ANDB	#48
	LDA	DTEMP
	STB	DWASTE
	ORA	DWASTE
	PULS	B
	BRA	DX002
DEX002	LDA	DTEMP
DX002	STA	DTEMP
	ANDA	#192
	TSTA
	BNE	DX003
	PSHS	B
	ANDB	#192
	LDA	DTEMP
	STB	DWASTE
	ORA	DWASTE
	PULS	B
	BRA	DEX003
DX003	LDA	DTEMP
DEX003	STA	,Y
	RTS


*******************************************************************************
* TRANSFER VERTICAL STRIPE FROM EPLYPC TO TO EPLYPS.
*******************************************************************************
EBEGIN	LDX	EPLYPC
	LDY	EPLYPS
	LDU	#22
ESUB1	LDA	,X+
	LDB	,Y
	STD	ETEMP
	ANDA	#3
	BNE	EEX000
	ANDB	#3
	STB	EWASTE
	LDD	ETEMP
	ORA	EWASTE       
        STA	ETEMP
	BRA	ENX000

EEX000	LDA	ETEMP
ENX000 
	ANDA	#12
	BNE	EEX001
	ANDB	#12
	STB	EWASTE
	LDD	ETEMP
	ORA	EWASTE
        STA	ETEMP
	BRA	ENX001

EEX001	LDA	ETEMP
ENX001	
	ANDA	#48
	BNE	EEX002
	ANDB	#48
	STB	EWASTE
	LDD	ETEMP
	ORA	EWASTE
        STA	ETEMP
	BRA	ENX002

EEX002	LDA	ETEMP
ENX002	
	ANDA	#192
	BNE	ENX003
	ANDB	#192
	STB	EWASTE
	LDD	ETEMP
	ORA	EWASTE
        STA	,Y
	BRA	EEX003

ENX003	LDA	ETEMP
        STA	,Y
EEX003	
	LEAY	+32,Y
	LEAU	-1,U
	CMPU	#0
	LBNE	ESUB1
	RTS

*******************************************************************************
* TRANSFER FNUM BYTES FROM FSTPNT TO FPTPNT. STRIDE FPTPNT BY 32 BYTES. USED
* TO TRANSFER SCREEN MEMORY TO A BUFFER IN VERTICAL STRIPES
*******************************************************************************
FLDFIG  LDX     FSTPNT
        LDY     FPTPNT
        LDB     FNUM
FLD000  LDA     ,X
        STA     ,Y+
        LDA     32,X
        STA     ,Y+
        LDA     64,X
        STA     ,Y+
        LDA     96,X
        STA     ,Y+
        LDA     128,X
        STA     ,Y+
        LDA     160,X
        STA     ,Y+
        LDA     192,X
        STA     ,Y+
        LDA     224,X
        STA     ,Y+
        LDA     256,X
        STA     ,Y+
        LDA     288,X
        STA     ,Y+
        LDA     320,X
        STA     ,Y+
        LEAX    352,X
        SUBB    #11
        BNE     FLD000
        RTS

*******************************************************************************
* ERASES A VERTICAL STRIPE
*******************************************************************************
GLDFIG	LDX	GSTPNT
	LDY	GPTPNT
	LDB	GNUM
GLD000  LDA     ,Y+
        STA     ,X
        LDA     ,Y+
        STA     32,X
        LDA     ,Y+
        STA     64,X
        LDA     ,Y+
        STA     96,X
        LDA     ,Y+
        STA     128,X
        LDA     ,Y+
        STA     160,X
        LDA     ,Y+
        STA     192,X
        LDA     ,Y+
        STA     224,X
        LDA     ,Y+
        STA     256,X
        LDA     ,Y+
        STA     288,X
        LDA     ,Y+
        STA     320,X
        LEAX    352,X
        SUBB    #11
        BNE     GLD000
        RTS

*******************************************************************************
* MAIN ENTRY POINT
*******************************************************************************
START_GAME	LBSR	SAVE_SCREEN
* SET PALETTE
	CLR	$FFB4
	LDA	#17
	STA	$FFB5
	LDA	#38
	STA	$FFB6
	LDA	#9
	STA	$FFB7

	LDD	#3616
	STD	AUMDUM
	LDA	#34
	STA	BCHAR
	CLR	BXAX
	CLR	BXAX+1
	CLR	BYAX
	LDD	#1024
	STD	BHRST
	LDD	#$2000
	STD	CMEMRY
	LDD	#$FFFF
	STD	CNUMBR
	LDD	#$5000
	STD	DPLYPC
	LDD	#$6000
	STD	DPLYPS
	CLR	DWASTE
	CLR	DTEMP
	LDD	#$5000
	STD	EPLYPC
	LDD	#$E00
	STD	EPLYPS
	CLR	EWASTE
	CLR	ETEMP
	LDA	#5
	STA	FNUM
	LDD	#$E00
	STD	FSTPNT
	LDD	#1024
	STD	FPTPNT
	LDD	#$E00
	STD	GSTPNT
	LDD	#1024
	STD	GPTPNT
	CLR	HGOOD
	LDA	#154
	STA	HBFRE
	STA	HYGD
	LDA	#1
	STA	HSTAT
	CLR	HSTAT+1
	CLR	IXBD
	CLR	IYBD
	CLR	IFRMR
	CLR	IFRMR+1
	CLR	ISTR
	CLR	IFLAG
	LDX	#0
	STX	SCORE
	STX	SCORE+2
	STX	RESULT
	STX	RESULT+2
	STX	ITEMP
	LDA	#3
	STA	MEN
	LDA	#$7F
	CLR	65497
	TFR	A,DP
	LDA	#232
	STA	65314
	CLR	$FFC7
	CLR	$FFC9
	CLR	$FFCB
	CLR	$FFCC
	CLR	$FFCE
	CLR	$FFD0
	CLR	$FFD2
	CLR	65477
	CLR	65475
	CLR	65472
	LDX	#$E00
	STX	IFRMR
	LDA	,X
	STA	ISTR
	LDA	#22
	STA	FNUM
	STA	GNUM
	LDA	#15
	STA	HGOOD
	LDA	#154
	STA	HYGD
	STA	HBFRE
	LDA	#1
	STA	HSTAT
DMEN	LDA	MEN
	ADDA	#48
	LDX	#31
	STX	BXAX
	LDB	#23
	STB	BYAX
	STA	1024
	CLR	1025
	LBSR	BEGIN
	LBSR	BEGINR

* MAIN LOOP ENTRY POINT
HSUB	

* ROUTINE TO DRAW GOOD GUY
HGO	LDA	HYGD
	LDB	$FF00
	ANDB	#1
	BEQ	FGO
	CLR	FLAG
FGO	LDB	#32
	MUL
        ADDD    #$E00
	TFR	D,X
	LDB	HGOOD
	ABX
	STX	EPLYPS
	STX	FSTPNT
	LDY	#$7000
	LDA	#42
	CMPA	$15A
	BLO	HRT1
	LEAY	65,Y
HRT1	STY	EPLYPC
	LDY	#$6000
	STY	FPTPNT
	LBSR	FLDFIG
	LBSR	EBEGIN
	LDY	#$7000+22
	LDA	#42
	CMPA	$15A
	BLO	HRT2
	LEAY	65,Y
HRT2	STY	EPLYPC
	LDY	#$6000+22
	STY	FPTPNT
	LDA	HYGD
	LDB	#32
	MUL
        ADDD    #$E01
	TFR	D,X
	LDB	HGOOD
	ABX
	STX	EPLYPS
	STX	FSTPNT
	LBSR	FLDFIG
	LBSR	EBEGIN
	LDA	HYGD
	LDB	#32
	MUL
        ADDD    #$E02
	TFR	D,X
	LDB	HGOOD
	ABX
	STX	EPLYPS
	STX	FSTPNT
	LDY	#$602C
	STY	FPTPNT
	LBSR	FLDFIG
	LDY	#$702C
	LDA	#42
	CMPA	$15A
	BLO	HRT3
	LEAY	65,Y
HRT3	STY	EPLYPC
	LBSR	EBEGIN
	LDA	$FF00
	ANDA	#1
	LBEQ	FIRE
HCONT	LDX	#2000
HCMPR	CMPX	#0
	LBEQ	ISTART
	LEAX	-1,X
	BRA	HCMPR

HJOY	JSR	[$A00A]
	LDA	$15B
	CMPA	#20
	BLO	HUP
HIP	CLR	HSTAT
	LDA	HYGD
	STA	HBFRE
	CMPA	#154
	BEQ	HFIX
	ADDA	#4
	STA	HYGD
	BRA	HCLR
HUP	LDA	HSTAT
	TSTA
	BEQ	HIP
	LDA	HYGD
	CMPA	#24
	BLO	HIP
	STA	HBFRE
	SUBA	#4
	STA	HYGD
	BRA	HCLR
HFIX	LDA	#1
	STA	HSTAT

* ENTRY POINT FOR CLEARING THE BALLOON SPRITE
HCLR
        LBSR     KILLM
	JSR	[$A000]
	CMPA	#13
	BNE	HCLR0
        LBSR    PAUSE

* CLEAR THE BALLOON SPRITE
HCLR0	LBSR	KILLM
        CWAI    #$AF
	LDA	HBFRE
	LDB	#32
	MUL
        ADDD    #$E00
	TFR	D,X
	LDB	HGOOD
	ABX
	LDY	#$6000
	STX	GSTPNT
	STY	GPTPNT
	LBSR	GLDFIG

	LDA	HBFRE
	LDB	#32
	MUL
        ADDD    #$E01
	TFR	D,X
	LDB	HGOOD
	ABX
	STX	GSTPNT
	LDY	#$6000+22
	STY	GPTPNT
	LBSR	GLDFIG

	LDA	HBFRE
	LDB	#32
	MUL
        ADDD    #$E02
	TFR	D,X
	LDB	HGOOD
	ABX
	STX	GSTPNT
	LDY	#$6000+44
	STY	GPTPNT
	LBSR	GLDFIG

	LDA	$15A
	CMPA	#20
	BHI	HNXTJ
	LDB	HGOOD
	TSTB
	LBEQ	HSUB
	DECB
	STB	HGOOD
	LBRA	HSUB

HNXTJ	CMPA	#41
	LBLO	HSUB
	LDB	HGOOD
	CMPB	#29
	LBEQ	HSUB
	INCB
	STB	HGOOD
	LBRA	HSUB

ISTART	LDB	IYBD
	LDA	ISTR
	LDX	IFRMR
	STA	,X
	CMPB	HYGD
	BLO	IPLUS
	BHI	IMINUS
	BRA	IDISPL
IPLUS	INCB
	STB	IYBD
	BRA	IDISPL
IMINUS	DECB
	STB	IYBD
IDISPL	LDA	IXBD
	COM	IFLAG
	PSHS	B
	LDB	IFLAG
	TSTB
	BNE	ISHOW
	CMPA	HGOOD
	BLO	IXPLS
	BHI	IXMIN
	BRA	ISHOW
IXPLS	INCA
	STA	IXBD
	BRA	ISHOW
IXMIN	DECA
	STA	IXBD
ISHOW	LBSR	KILLM
	LDA	#32
	PULS	B
	MUL
	TFR	D,X
	LDB	IXBD
	ABX
	TFR	X,D
	ADDD	#$E00
	TFR	D,X
	LDA	,X
	STA	ISTR
	STX	IFRMR
	LDA	#255
	STA	,X
	LBRA	HJOY

*******************************************************************************
* INCREMENT THE SCORE BY 10
*******************************************************************************
INCREMENT_SCORE
	LDX	#SCORE+3
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

BEGINR	LDA	RESULT
	STA	SCORE
	PSHS	A
	ANDA	#240
	RORA
	RORA
	RORA
	RORA
	CMPA	#10
	BLO	NXT
	CLRA
NXT	ADDA	#48
	TFR	A,B
	PULS	A
	ANDA	#15
	ADDA	#48
	EXG	A,B
	STD	1024
BEGNA	LDA	RESULT+1
	STA	SCORE+1
	PSHS	A
	ANDA	#240
	RORA
	RORA
	RORA
	RORA
	ADDA	#48
	TFR	A,B
	PULS	A
	ANDA	#15
	ADDA	#48
	EXG	A,B
	STD	1026
	LDU	#1024
	STU	BHRST
BEGNB	LDA	RESULT+2
	STA	SCORE+2
	PSHS	A
	ANDA	#240
	RORA
	RORA
	RORA
	RORA
	ADDA	#48
	TFR	A,B
	PULS	A
	ANDA	#15
	ADDA	#48
	EXG	A,B
	CLR	1030
	CLR	BXAX+1
	STD	1028
	LDA	#23
	STA	BYAX
	LBSR	BEGIN
	RTS

*******************************************************************************
* CHECKS IF THE BAD GUY HIT THE BALLOON
*******************************************************************************
KILLM	LDA	HGOOD
	CMPA	IXBD
	BEQ	KNXT
	INCA
	CMPA	IXBD
	BEQ	KNXT
	INCA
	CMPA	IXBD
	BEQ	KNXT
	RTS
KNXT	LDA	HYGD
	CMPA	IYBD
	BEQ	KILL
	BLO	KIT
	RTS
KIT	ADDA	#22
	CMPA	IYBD
	BHI	KILL
	BEQ	KILL
	RTS

*******************************************************************************
* KILLS A BALLOON
*******************************************************************************
KILL	DEC	MEN
	LDX	#$8000
	LDY	#$6000
	STX	CMEMRY
	STY	CNUMBR
        LBSR	PLAY_SOUND
	LBSR	RESTORE_SCREEN
	LDA	MEN
	ADDA	#48
	LDX	#31
	STX	BXAX
	LDB	#23
	STB	BYAX
	CLRB
	STD	1024
	LBSR	BEGIN
	LDA	ISTR
	LDX	IFRMR
	STA	,X
	LDA	MEN
	TSTA
	BEQ	GAME_OVER
	LDA	ISTR
	CLR	IXBD
	CLR	IYBD
	CLR	IFRMR
 	CLR	IFRMR+1
	CLR	ISTR
	LEAS	-2,S
	LBRA	HSUB

*******************************************************************************
* GAME OVER SEQUENCE
*******************************************************************************
GAME_OVER	
        LDX	#GVR
	LDY	#1024
LDGV	LDA	,X+
	STA	,Y+
	TSTA
	BNE	LDGV
	LDX	#12
	STX	BXAX
	LDA	#12
	STA	BYAX
	LBSR	BEGIN
PLR	JSR	[$A000]
	TSTA
	BEQ	PLR
	LBSR	RESTORE_SCREEN
	LBSR	START_GAME
	BRA	GAME_OVER
	RTS

*******************************************************************************
* COPY THE SCREEN INTO PAGE #$30
*******************************************************************************
SAVE_SCREEN
	LDA	#$30
	STA	$FFA3
	LDA	#254
	ANDA	$FF91
	STA	$FF91

* COPY THE SCREEN
	LDX	#$E00
	LDY	#$6000
ZLD	LDD	,X++
	STD	,Y++
	CMPX	#$E00+(32*22*8)
	BNE	ZLD

* RETURN NORMAL PAGE
	LDA	#$3B
	STA	$FFA3
	RTS

*******************************************************************************
* RESTORE THE SCREEN FROM PAGE #$30
*******************************************************************************
RESTORE_SCREEN	
  LDA	#$30
	STA	$FFA3
	LDX	#$E00
	LDY	#$6000
        CWAI    #$AF
ZLDR	LDD	,Y++
	STD	,X++
	CMPX	#$E00+(32*22*8)
	BNE	ZLDR
	LDA	#$3B
	STA	$FFA3
	RTS

*******************************************************************************
* PAUSE ROUTINE
*******************************************************************************
PAUSE	JSR	[$A000]
	CMPA	#13
	BNE	PAUSE
	RTS

*******************************************************************************
* CRAZY FIRE ROUTINE
*******************************************************************************
FIRE	LDA	FLAG
	TSTA
*	LBNE	HCONT
	INC	FLAG
	LDX	#$8000
	LDY	#$500
	STX	CMEMRY
	STY	CNUMBR
	LDA	$15A
	CMPA	#42
	BLO	FFIRN
	LDA	HGOOD
	ADDA	#3
	LDB	HYGD
	ADDB	#9
	STA	ZTEMP+1
	LDA	#32
	MUL
	ADDD	ZTEMP
	ADDD	#$E00
	EXG	X,D
	LDA	#255
	LDB	,X
	STB	BANK1
	STA	,X+
	LDB	,X
	STB	BANK2
	STA	,X
	PSHS	X
	LBSR	PLAY_SOUND
	PULS	X
	LDB	BANK2
	STB	,X
	LEAX	-1,X
	LDB	BANK1
	STB	,X
	LBSR	DETEC
	LBRA	HCONT
FFIRN	LDA	HGOOD
	STA	ZTEMP+1
	LDA	HYGD
	LDB	#32
	ADDA	#9
	MUL
	ADDD	ZTEMP
	ADDD	#$E00
	EXG	X,D
	LDA	#255
	LDB	,-X
	STA	,X
	STB	BANK1
	LDB	,-X
	STA	,X
	STB	BANK2
	PSHS	X
	LBSR	PLAY_SOUND
	PULS	X
	LDB	BANK2
	STB	,X+
	LDB	BANK1
	STB	,X
	LBSR	SIM
	LBRA	HCONT
DETEC	LDB	HGOOD
	ADDB	#3
	CMPB	IXBD
	BEQ	DEXT
	INCB
	CMPB	IXBD
	BEQ	DEXT
	INCB
	BEQ	DEXT
RT	RTS


DEXT	LDB	HYGD
	TFR	B,A
	SUBB	#6
	ADDA	#6
	CMPB	IYBD
	BHI	RT
	CMPA	IYBD
	BLO	RT
SCR	LBSR	INCREMENT_SCORE
	CLR	IXBD
	CLR	IYBD
	LBSR	RESTORE_SCREEN
	RTS

SIM	LDA	HGOOD
	DECA
	CMPA	IXBD
	BEQ	SIMXT
	DECA
	CMPA	IXBD
	BEQ	SIMXT
	DECA
	CMPA	IXBD
	BEQ	SIMXT
	RTS
SIMXT	LDA	HYGD
	TFR	A,B
	SUBA	#6
	ADDB	#6
	CMPA	IYBD
	BHI	RT
	CMPB	IYBD
	BLO	RT
	BRA	SCR
		  }
}


interrupt void VerticalIRQ() {
  *CoCoMiscGIMEIRQEnable;
}


void SetupIRQ() {
  // Disable and turn off interrupts
  asm { orcc #$50 }
  byte disableInterrupts = 0x34;
  *CoCoMiscPIA1ControlRegisterA = disableInterrupts;
  *CoCoMiscPIA1ControlRegisterB = disableInterrupts;
  *CoCoMiscPIA2ControlRegisterA = disableInterrupts;
  *CoCoMiscPIA2ControlRegisterB = disableInterrupts;

  // Bleed off existing interrupts
  byte val = *CoCoMiscPIA1DataRegisterA;
  val = *CoCoMiscPIA1DataRegisterB;
  val = *CoCoMiscPIA2DataRegisterA;
  val = *CoCoMiscPIA2DataRegisterB;

  // Setup the RQ jump
  CoCoMiscIRQJumpVector[0] = 0x7e;
  CoCoMiscIRQJumpVector[1] = (byte)(((unsigned)(VerticalIRQ)) >> 0x8);
  CoCoMiscIRQJumpVector[2] = (byte)((unsigned)(VerticalIRQ)) & 0xff;

  // Enable IRQ
  *CoCoMiscGIMEInit0 |= 0x20;
  *CoCoMiscGIMEInit1 |= 0x20;
  *CoCoMiscGIMEIRQEnable = 0x08;
  
  // Bleed off GIME interrupts
  val = *CoCoMiscGIMEFIRQEnable;
  val = *CoCoMiscGIMEIRQEnable;

  // Enable interrupts
  asm { andcc #$aF }
  return;
}
