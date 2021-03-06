/*
  CoCoMisc.c
  
  This software is for demonstration purposes only. Use at your own
  risk.

  Misc CoCo support
*/


#include "CoCoMisc.h"


/** Whether or not we are in RGB mode */
byte _cocoRGBMode = 1;


int random(int value) {
  asm {
    ldd value
    pshs u
    jsr $B4F4
    jsr $BF1F
    jsr $B3ED
    puls u
    std value
  }
  return value;
}


void readJoystick(byte *xx, byte *yy) {
  asm {
    PSHS U
    JSR [$A00A]
    PULS U
    LDD $15a
    ldx xx
    ldy yy
    sta ,x
    stb ,y
  }
}


void CoCoMiscPaletteFade(byte *rgbPalette, byte *cmpPalette, byte level,
    byte border) {
  byte *palette = (_cocoRGBMode) ? rgbPalette : cmpPalette;
  
  switch(level) {
  case 0:
    memset(cocoPaletteBaseReg, 0, COCO_NUM_PALETTE_REGISTERS);
    break;
  case 1:
    if (_cocoRGBMode)
      for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
 cocoPaletteBaseReg[ii] = palette[ii] >> 3;
    else
      for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
 cocoPaletteBaseReg[ii] = palette[ii] & 0x1f;
    break;
  case 2:
    if (_cocoRGBMode)
      for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
 cocoPaletteBaseReg[ii] = palette[ii] & 0x38;
    else
      for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
 cocoPaletteBaseReg[ii] = palette[ii] & 0x2f;
    break;
  case 3:
    for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
      memcpy(cocoPaletteBaseReg, palette, COCO_NUM_PALETTE_REGISTERS);
    break;
  case 4:
    if (_cocoRGBMode)
      for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
 cocoPaletteBaseReg[ii] = palette[ii] | 0x7;
    else
      for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
 cocoPaletteBaseReg[ii] = palette[ii] | 0x10;
    break;
  case 5:
    if (_cocoRGBMode)
      for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
 cocoPaletteBaseReg[ii] = 0x38 | palette[ii];
    else
      for(int ii=0; ii<COCO_NUM_PALETTE_REGISTERS; ii++)
 cocoPaletteBaseReg[ii] = palette[ii] | 0x20;
    break;
  case 6:
    memset(cocoPaletteBaseReg, 0x3f, COCO_NUM_PALETTE_REGISTERS);
    break;
  }
  *cocoBorderRegister = cocoPaletteBaseReg[border];
}


void CoCoMiscDelay(unsigned int count) {
  for(int ii=0; ii<count; ii++);
}


void CoCoMiscFadeIn(byte *rgbPalette, byte *cmpPalette, unsigned int count,
      byte border) {
  for(byte ii=0; ii<4; ii++) {
    CoCoMiscPaletteFade(rgbPalette, cmpPalette, ii, border);
    CoCoMiscDelay(count);
  }
}


void CoCoMiscFadeOut(byte *rgbPalette, byte *cmpPalette, unsigned int count,
       byte border) {
  for(byte ii=4; ii>0; ii--) {
    CoCoMiscPaletteFade(rgbPalette, cmpPalette, ii-1, border);
    CoCoMiscDelay(count);
  }
}


void CoCoMiscSetRGBMode(byte mode) { _cocoRGBMode = mode; }


byte CoCoMiscGetRGBMode() { return _cocoRGBMode; }


void __CoCoMiscJoystickRead();


int CoCoMiscReadJoystick(byte axis, byte scale) {
  int result;
  byte piaValA = *CoCoMiscPIA1ControlRegisterA;
  byte piaValB = *CoCoMiscPIA1ControlRegisterB;

  // Select the joystick axis
  *CoCoMiscPIA1ControlRegisterB = (axis > 1)
                                ? (piaValB | 0x08)
                                : (piaValB & 0xf7);
  *CoCoMiscPIA1ControlRegisterA = (axis & 1)
                                ? (piaValA | 0x08)
                                : (piaValA & 0xf7);

  if (FALSE)
    __CoCoMiscJoystickRead();

  asm {
    ldb scale
    lda axis
    anda #1
    sta AXIS
    bne CoCoMiscReadJoystickLoadDACY
    ldy #DACX
    stb XSCALE
    bra CoCoMiscReadJoystickRead
CoCoMiscReadJoystickLoadDACY:
    ldy #DACY
    stb YSCALE
CoCoMiscReadJoystickRead:
    orcc #$50
    lbsr ___CoCoMiscJoystickRead
    andcc #$af
    stx result
  }

  *CoCoMiscPIA1ControlRegisterA = piaValA;
  *CoCoMiscPIA1ControlRegisterB = piaValB;

  return result;
}


asm void __CoCoMiscJoystickRead() {
  asm {
* * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                   *
* HIGH RESOLUTION SOFTWARE-ONLY JOYSTICK SUBROUTINE *
*                                                   *
*             256/320/640 x 192/200/225             *
*                                                   *
* ON ENTRY                                          *
* Y = 15 BIT SAMPLE POINTER (#DACX OR #DACY)        *
* AXIS = JOYSTICK AXIS TO SCAN (0=X 1=Y)            *
* XSCALE = 1(256) OR 2(320) OR 3(640)               *
* YSCALE = 1(192) OR 2(200) OR 3(225)               *
*                                                   *
* ON EXIT                                           *
* X = SCALED JOYSTICK READING                       *
*                                                   *
* * * * * * * * * * * * * * * * * * * * * * * * * * *

 * 6-BIT SUCCESSIVE APPROXIMATION

 STY SMPTR
 LDA #SCANS
 STA SCANC

JOYLP LDD #$8240 ;A=SAMPLE(130) B=OFFSET(64)
LLA STB TEMP
 STA $FF20 ;LOAD DAC WITH SAMPLE VALUE
 EXG X,X ;DELAY
 EXG X,X
 LDB $FF00 ;GET COMPARATOR OUTPUT
 BPL LLB ;BRANCH IF COMPARATOR LOW
 ADDA TEMP ;INCREMENT SAMPLE
 BRA LLC
LLB SUBA TEMP ;DECREMENT SAMPLE
LLC LDB TEMP
 LSRB  ;HALF OFFSET VALUE
 CMPB #2 ;LAST SAMPLE ?
 BNE LLA
 CMPA #250 ;CHECK FOR OVERFLOW (DAC=63)
 BHI LLF

 * CLOSE IN ON PRECISE 6-BIT VALUE

 SUBA #4 ;MOVE BACK 1 DAC POINT
LLD STA $FF20 ;LOAD DAC WITH SAMPLE VALUE
 EXG X,X ;DELAY
 LDB $FF00 ;GET COMPARATOR OUTPUT
 BPL LLF
 ADDA #4 ;MOVE FORWARD 1 DAC POINT
 BRA LLD ;REPEAT

LLF STA DAC64 ;STORE FINAL 6-BIT RESULT


 * EXTENDED 15-BIT 'FUZZY LOGIC' SCAN

 LDX #$FF00
 LDY #1023
 LDB #6 ;SET OVERSAMPLING TO 6X
 STB COUNT

SCAN4D SUBA #4 ;FALLING VOLTAGE SCAN AT 4 CYCLES
 STA $FF20
 LDB ,X
 EXG X,X
 BMI SCAN4U ;DETECT?
 LEAY -56,Y ;ADD THIS READING WEIGHT

SCAN4U ADDA #4 ;RISING VOLTAGE SCAN AT 4 CYCLES
 STA $FF20
 LDB ,X
 EXG X,X
 BPL SCAN5D ;DETECT?
 LEAY 12,Y ;ADD THIS READING WEIGHT

SCAN5D SUBA #4 ;FALLING VOLTAGE SCAN AT 5 CYCLES
 STA $FF20
 LDB $FF00
 EXG X,X
 BMI SCAN5U ;DETECT?
 LEAY -29,Y ;ADD THIS READING WEIGHT

SCAN5U ADDA #4 ;RISING VOLTAGE SCAN AT 5 CYCLES
 STA $FF20
 LDB $FF00
 EXG X,X
 BPL SCAN6D ;DETECT?
 LEAY 8,Y ;ADD THIS READING WEIGHT

SCAN6D SUBA #4 ;FALLING VOLTAGE SCAN AT 6 CYCLES
 STA $FF20
 NOP
 LDB ,X
 EXG X,X
 BMI SCAN6U ;DETECT?
 LEAY -16,Y ;ADD THIS READSING WEIGHT

SCAN6U ADDA #4 ;RISING VOLTAGE SCAN AT 6 CYCLES
 STA $FF20
 NOP
 LDB ,X
 EXG X,X
 BPL SCAN7D ;DETECT?
 LEAY 6,Y ;ADD THIS READING WEIGHT

SCAN7D SUBA #4 ;FALLING VOLTAGE SCAN AT 7 CYCLES
 STA $FF20
 NOP
 LDB $FF00
 EXG X,X
 BMI SCAN7U ;DETECT?
 LEAY -14,Y ;ADD THIS READING WEIGHT

SCAN7U ADDA #4 ;RISING VOLTAGE SCAN AT 7 CYCLES
 STA $FF20
 NOP
 LDB $FF00
 EXG X,X
 BPL SCAN8D ;DETECT?
 LEAY 5,Y ;ADD THIS READING WEIGHT

SCAN8D SUBA #4 ;FALLING VOLTAGE SCAN AT 8 CYCLES
 STA $FF20
 NOP
 NOP
 LDB ,X
 EXG X,X
 BMI SCAN8U ;DETECT?
 LEAY -12,Y ;ADD THIS READING WEIGHT

SCAN8U ADDA #4 ;RISING VOLTAGE SCAN AT 8 CYCLES
 STA $FF20
 NOP
 NOP
 LDB ,X
 BPL DONE ;DETECT?
 LEAY 4,Y ;ADD THIS READING WEIGHT

DONE LDA DAC64
 DEC COUNT
 LBNE SCAN4D ;LOOP FOR OVERSAMPLING

 * COMBINE EXTENDED DATA WITH 6 BIT SAMPLE

 TFR Y,D
 LSRA
 RORB
 TFR D,Y
 LDA DAC64
 ANDA #253 ;RESET BIT 1 (SERIAL)
 CLRB
 LSRA
 RORB
 LEAY D,Y ;Y CONTAINS 15 BIT SAMPLE
 LEAY -127,Y ;RECENTER SAMPLE AROUND 0

 * ROLL SAMPLE INTO 16 MOST RECENT SAMPLES BUFFER

 LDX SMPTR
 LDB SCANC
 ADDB SMIDX
 ANDB #BUFFRS-1
 ASLB
 STY B,X

 DEC SCANC
 LBNE JOYLP ;SCAN MULTIPLE TIMES?

 * GET AVERAGE OF LAST 16 SAMPLE RESULTS

 LDA #BUFFRS
 STA COUNT
 CLR TEMP
 CLRA
 CLRB
SMLP ADDD ,X++
 BCC SMNC
 INC TEMP
SMNC DEC COUNT
 BNE SMLP
 LSR TEMP
 RORA
 RORB
 LSR TEMP
 RORA
 RORB
 LSR TEMP
 RORA
 RORB
 LSR TEMP
 RORA
 RORB

*--------------------------------------------------------
 TFR D,X ;UNCOMMENT THESE LINES TO RETURN
 RTS  ;15 BIT SAMPLE TO BASIC.
*--------------------------------------------------------

 TFR D,Y ;Y CONTAINS AVERAGED 15 BIT SAMPLE

 TST AXIS ;DOING X OR Y READS?
 BEQ DOX ;DO X SCALE
 LDB YSCALE
 CMPB #1
 BEQ SIZ192
 CMPB #2
 BEQ SIZ200
 LBRA SIZ225
 
DOX LDB XSCALE
 CMPB #1
 BEQ SIZ256
 CMPB #2
 BEQ SIZ320


 * SCALE 15 BIT SAMPLE TO 640 FOR X-AXIS

SIZ640 TFR Y,D
 LSRA  ;MULTIPLY BY .5
 RORB
 STD TEMP
 TFR Y,D
 LSLB  ;MULTIPLY BY 2
 ROLA
 ADDD TEMP ;NOW 2.5X ORIGINAL WITH OVERFLOW
 STB TEMP ;SAVE LSB
 TFR A,B ;DIVIDE RESULT BY 256
 LDA #0 ;NO CHANGE TO CARRY
 ROLA  ADD IN CARRY
 LSL TEMP ;GIVES ANSWER * 2
 ROLB
 ROLA
 BRA AVRP


 * SCALE 15 BIT SAMPLE TO 256 FOR X-AXIS

SIZ256 TFR Y,D
 STB TEMP ;DIVIDE D BY 256
 TFR A,B
 CLRA
 LSL TEMP ;MULTIPLY RESULT BY 2
 ROLB
 ROLA  ;D = INITIAL D DIVIDED BY 128
 BRA AVRP


 * SCALE 15 BIT SAMPLE TO 320 FOR X-AXIS

SIZ320 TFR Y,D
 LSRA  ;MULTIPLY BY 1.25
 RORB
 LSRA
 RORB
 LEAY D,Y
 TFR Y,D ;DIVIDE BY 128
 STB TEMP
 TFR A,B
 CLRA
 LSL TEMP
 ROLB
 ROLA
 BRA AVRP


 * SCALE 15 BIT SAMPLE TO 192 FOR Y-AXIS

SIZ192 TFR Y,D
 LSRA  ;DIVIDE BY 8
 RORB
 LSRA
 RORB
 LSRA
 RORB
 COMA  ;SUBTRACT RESULT FROM 15 BIT SAMPLE
 COMB
 ADDD #1
 LEAY D,Y
 ASRA  ;DIVIDE BY 16
 RORB
 LEAY D,Y ;SUBTRACT FROM SAMPLE
 ASRA  ;DIVIDE BY 32
 RORB
 LEAY D,Y ;SUBTRACT FROM SAMPLE
 LEAY D,Y
 TFR Y,D ;DIVIDE BY 128
 ASLB
 ROLA
 TFR A,B
 CLRA
 BRA AVRP


 * SCALE 15 BIT SAMPLE TO 200 FOR Y-AXIS

SIZ200 TFR Y,D
 LSRA  ;DIVIDE BY 8
 RORB
 LSRA
 RORB
 LSRA
 RORB
 COMA  ;SUBTRACT RESULT FROM 15 BIT SAMPLE
 COMB
 ADDD #1
 LEAY D,Y
 ASRA  ;DIVIDE BY 16
 RORB
 LEAY D,Y ;SUBTRACT FROM SAMPLE
 ASRA  ;DIVIDE BY 32
 RORB
 LEAY D,Y ;SUBTRACT FROM SAMPLE
 TFR Y,D ;DIVIDE BY 128
 ASLB
 ROLA
 TFR A,B
 CLRA
 BRA AVRP


 * SCALE 15 BIT SAMPLE TO 225 FOR Y-AXIS

SIZ225 TFR Y,D
 LSRA  ;DIVIDE BY 8
 RORB
 LSRA
 RORB
 LSRA
 RORB
 COMA  ;SUBTRACT RESULT FROM 15 BIT SAMPLE
 COMB
 ADDD #1
 LEAY D,Y
 TFR Y,D ;DIVIDE BY 128
 ASLB
 ROLA
 TFR A,B
 CLRA


 * EXIT SCAN SUBROUTINE

AVRP TFR D,X
 RTS


 * VARIABLES STORE

DAC64 FCB 0
TEMP FDB 0
COUNT FCB 0

SMIDX FCB 0
SMPTR FDB 0
AXIS FCB 0
SCANC FCB 0
SCANS EQU 5
BUFFRS EQU 16


XSCALE FCB 1
YSCALE FCB 1
DACX FDB 8192,8192,8192,8192,8192,8192,8192,8192
 FDB 8192,8192,8192,8192,8192,8192,8192,8192
DACY FDB 8192,8192,8192,8192,8192,8192,8192,8192
 FDB 8192,8192,8192,8192,8192,8192,8192,8192
  }
}
