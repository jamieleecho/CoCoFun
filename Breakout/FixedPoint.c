/*
  FixedPoint.c
  
  This software is for demonstration purposes only. Use at your own
  risk.
*/


#ifndef _FixedPoint_c
#define _FixedPoint_c

#include "FixedPoint.h"


void FixedPointMake(FixedPoint *c, int whole, unsigned decimal) {
  c->Whole = whole;
  c->Fraction = decimal;
}


void FixedPointNegate(FixedPoint *c, FixedPoint *a) {
  asm {
* Load src and dest into x and y
    ldx a
    ldy c

* Negate LSB
    ldd 2,x
    negb

* If no carry, we do not have to worry about carrying
    bcs FixedPointNegateNoCarry

* Carry to next byte 
    nega
    bcs FixedPointNegateNoCarryLSW

* Store result
    std 2,y

* Load next word
    ldd ,x

* Carry to next byte
    negb
    bcs FixedPointNegateNoCarryB2

* Carry to next byte
    nega
    bra FixedPointNegateFinish

* No need to worry about carrying here
FixedPointNegateNoCarry:
    coma

FixedPointNegateNoCarryLSW:
* Store result
    std 2,y

* Load next word
    ldd ,x
    comb
FixedPointNegateNoCarryB2:
    coma

* Store final word
FixedPointNegateFinish
    std ,y
  }  
}


void FixedPointAdd(FixedPoint *c, FixedPoint *a, FixedPoint *b) {
  asm {
    ldx a
    ldy b
    clra
    clrb
    ldd 2,x
    addd 2,y
    ldx c
    std 2,x
    clra
    clrb
    bcc FixedPointAddSkipCarry
    incb

 FixedPointAddSkipCarry:
    ldx a
    addd ,x
    addd ,y
    ldx c
    std ,x
  }
}


void FixedPointSub(FixedPoint *c, FixedPoint *a, FixedPoint *b) {
  FixedPointNegate(c, b);
  FixedPointAdd(c, a, c);
}


void FixedPointMul(FixedPoint *c, FixedPoint *a, FixedPoint *b) {
  byte results[8], aa[4], bb[4];
  memset(results, 0, sizeof(results));

  // Convert all operands to positive, count number of negates
  byte numNegatives = 0;
  if (a->Whole < 0x0) {
    FixedPointNegate((FixedPoint *)aa, a);
    numNegatives++;
  } else
    memcpy(aa, (byte *)a, sizeof(aa));
  if (b->Whole < 0x0) {
    FixedPointNegate((FixedPoint *)bb, b);
    numNegatives++;
  } else
    memcpy(bb, (byte *)b, sizeof(bb));

  asm {
    leax aa
    leay bb
    pshs u
    leau results

* Multiply first byte of y
    lda 3,x
    ldb 3,y
    mul
    std 6,u

    lda 2,x
    ldb 3,y
    mul
    addd 5,u
    std 5,u
    lbsr FixedPointCarry4

    lda 1,x
    ldb 3,y
    mul
    addd 4,u
    std 4,u
    lbsr FixedPointCarry3

    lda ,x
    ldb 3,y
    mul
    addd 3,u
    std 3,u
    lbsr FixedPointCarry2

* Multiply second byte of y
    lda 3,x
    ldb 2,y
    mul
    addd 5,u
    std 5,u
    lbsr FixedPointCarry4

    lda 2,x
    ldb 2,y
    mul
    addd 4,u
    std 4,u
    lbsr FixedPointCarry3

    lda 1,x
    ldb 2,y
    mul
    addd 3,u
    std 3,u
    bsr FixedPointCarry2

    lda ,x
    ldb 2,y
    mul
    addd 2,u
    std 2,u
    bsr FixedPointCarry1

* Multiply third byte of y
    lda 3,x
    ldb 1,y
    mul
    addd 4,u
    std 4,u
    bsr FixedPointCarry3

    lda 2,x
    ldb 1,y
    mul
    addd 3,u
    std 3,u
    bsr FixedPointCarry2

    lda 1,x
    ldb 1,y
    mul
    addd 2,u
    std 2,u
    bsr FixedPointCarry1

    lda ,x
    ldb 1,y
    mul
    addd 1,u
    std 1,u
    bsr FixedPointCarry0

* Multiply fourth byte of y
    lda 3,x
    ldb ,y
    mul
    addd 3,u
    std 3,u
    bsr FixedPointCarry3

    lda 2,x
    ldb ,y
    mul
    addd 2,u
    std 2,u
    bsr FixedPointCarry3

    lda 1,x
    ldb ,y
    mul
    addd 1,u
    std 1,u
    bsr FixedPointCarry0

    lda ,x
    ldb ,y
    mul
    addd ,u
    std ,u

    bra FixedPointAsmDone

*******************************************************************************
* Routine for performing carry results on product
*******************************************************************************
FixedPointCarry8:
    bcc FixedPointCarryDone
    inc 8,u
FixedPointCarry7:
    bcc FixedPointCarryDone
    inc 7,u
FixedPointCarry6:
    bcc FixedPointCarryDone
    inc 6,u
FixedPointCarry5:
    bcc FixedPointCarryDone
    inc 5,u
FixedPointCarry4:
    bcc FixedPointCarryDone
    inc 4,u
FixedPointCarry3:
    bcc FixedPointCarryDone
    inc 3,u
FixedPointCarry2:
    bcc FixedPointCarryDone
    inc 2,u
FixedPointCarry1:
    bcc FixedPointCarryDone
    inc 1,u
FixedPointCarry0:
    bcc FixedPointCarryDone
    inc ,u
FixedPointCarryDone:
    rts

*******************************************************************************
* Invoked when calculation is complete
*******************************************************************************
FixedPointAsmDone:
    puls u
  }

  // Make result negative if needed
  c->Whole = ((int)results[2] << 8) + results[3];
  c->Fraction = ((unsigned)results[4] << 8) + results[5];
  if (numNegatives & 1)
    FixedPointNegate(c, c);
}


void FixedPointDiv(FixedPoint *c, FixedPoint *d, FixedPoint *a, FixedPoint *b) {
  // aa[0] = dividend, aa[1] = divisor, aa[2] = quotient
  FixedPoint aa[3];
  char numDivisorShifts = 0, numDividendShifts = 0;
  memset((byte *)(aa + 2), 0, sizeof(aa[2]));

  // Convert all operands to positive, count number of negates
  byte numNegatives = 0;
  if (a->Whole < 0x0) {
    FixedPointNegate(aa, a);
    numNegatives++;
  } else
    memcpy((byte *)aa, (byte *)a, sizeof(aa[0]));
  if (b->Whole < 0x0) {
    FixedPointNegate((aa + 1), b);
    numNegatives++;
  } else
    memcpy((byte *)(aa + 1), (byte *)b, sizeof(aa[1]));

  // Can't divide by zero, simply set c to zero
  if ((aa[0].Whole == 0) && (aa[0].Fraction == 0)) {
    memset(c, 0, sizeof(*c));
    return;
  }

  // Result is zero, so return 0
  if ((aa[1].Whole == 0) && (aa[1].Fraction == 0)) {
    memset(c, 0, sizeof(*c));
    return;
  }

  // Note to self: Never ever do a div routine again
  asm {
* The first thing we have to do is shift the divisor left until its most
* significant bit is 1
    leax aa
FixedPointDivCheckDivisor:
    lda 4,x
    bmi FixedPointDivFixDividend
    asl 7,x
    rol 6,x
    rol 5,x
    rol 4,x
    inc numDivisorShifts
    bra FixedPointDivCheckDivisor

* We now have to fix the dividend so that its most significant bit is 1
FixedPointDivFixDividend:
    lda ,x    
    bmi FixedPointDivMainSetup
    asl 3,x
    rol 2,x
    rol 1,x
    rol ,x
    inc numDividendShifts
    bra FixedPointDivFixDividend

* At this point our divisor and dividend are shift all the way left.
* We must do the following while we have more quotient bits
* 1. If the most significant bits of the divisor
*     <= most significant bits of the dividend
*          a. Put 1 in the quotient
*     else
*          a. Put 0 in the quotient
*          b. If we have more quotient bits left, shift dividend left
*          c. Put 1 in the quotient
* 2. subtract most signficant divisor bits from most significant dividend bits       
* 3. while we have more quotient bits and there is a zero in the dividend
*          a. Shift the quotient left
*          b. Shift the dividend left
FixedPointDivMainSetup:
    ldb #16      * B = max number of loops
    subb numDividendShifts
    addb numDivisorShifts    

FixedPointDivMainLoop:
* Compare dividend to divisor
    bsr FixedPointCompareDividendToDivisor

* Branch if dividend too small
    blo FixedPointDivDividendTooSmall
    bra FixedPointDivPut1InQuotient

FixedPointDivDividendTooSmall:
    tstb
    lbeq FixedPointDivMainLoopEnd
    decb
    bsr FixedPointShiftDividendLeft
    bsr FixedPointShiftQuotientLeft

FixedPointDivPut1InQuotient:
* Put 1 in quotient, subtract divisor from dividend
    lda #1
    ora 11,x
    sta 11,x

FixedPointSubtractAndShift:
    bsr FixedPointSubtractDivisorFromDividend

FixedPointDivMainLoopRepeat:
    lda ,x
    bmi FixedPointDivMainLoop

    tstb
    beq FixedPointDivMainLoopEnd
    decb

    bsr FixedPointShiftDividendLeft
    bsr FixedPointShiftQuotientLeft
    bra FixedPointDivMainLoopRepeat

FixedPointShiftQuotientLeft:
    asl 11,x
    rol 10,x
    rol 9,x
    rol 8,x
    rts

FixedPointShiftDividendLeft:
    asl 3,x
    rol 2,x
    rol 1,x
    rol ,x
    rts

FixedPointShiftDividendRight:
    asr ,x
    ror 1,x
    ror 2,x
    ror 3,x
    rts

FixedPointCompareDividendToDivisor:
    lda ,x 
    cmpa 4,x
    bne FixedPointCompareDividendToDivisorDone
    lda 1,x 
    cmpa 5,x
    bne FixedPointCompareDividendToDivisorDone
    lda 2,x 
    cmpa 6,x
    bne FixedPointCompareDividendToDivisorDone
    lda 3,x 
    cmpa 7,x
FixedPointCompareDividendToDivisorDone:
    rts

FixedPointSubtractNegateDivisor:
    neg 7,x
    bcs FixedPointSubtractNegateDivisorCom2
    neg 6,x
    bcs FixedPointSubtractNegateDivisorCom1
    neg 5,x
    bcs FixedPointSubtractNegateDivisorCom0
    neg 4,x
    rts

FixedPointSubtractNegateDivisorCom2:
    com 6,x
FixedPointSubtractNegateDivisorCom1:
    com 5,x
FixedPointSubtractNegateDivisorCom0:
    com 4,x
    rts

FixedPointSubtractDivisorFromDividend:
    bsr FixedPointSubtractNegateDivisor
    lda 7,x
    adda 3,x
    sta 3,x
    lda 6,x
    adca 2,x
    sta 2,x
    lda 5,x
    adca 1,x
    sta 1,x
    lda 4,x
    adca ,x
    sta ,x
    bsr FixedPointSubtractNegateDivisor
    rts

FixedPointDivMainLoopEnd:
    ldb numDividendShifts
    beq FixedPointDivFixRemainderEnd
FixedPointDivFixRemainder
    bsr FixedPointShiftDividendRight
    decb
    beq FixedPointDivFixRemainder

FixedPointDivFixRemainderEnd:    
  }

  // Make result negative if needed
  memcpy((byte *)c, (byte *)(aa + 2), sizeof(aa[2]));
  memcpy((byte *)d, (byte *)(aa), sizeof(aa[0]));
  if (numNegatives & 1) {
    FixedPointNegate(c, c);
    FixedPointNegate(d, d);
  }
}


byte FixedPointEquals(FixedPoint *a, FixedPoint *b) {
  return (a->Whole == b->Whole) && (a->Fraction == b->Fraction);
}


byte FixedPointGreaterThan(FixedPoint *a, FixedPoint *b) {
  if (a->Whole > b->Whole) return TRUE;
  if (a->Whole < b->Whole) return FALSE;
  return (a->Whole >= 0) ? (a->Fraction > b->Fraction)
    : (b->Fraction > a->Fraction);
}


byte FixedPointLessThan(FixedPoint *a, FixedPoint *b) {
  if (a->Whole < b->Whole) return TRUE;
  if (a->Whole > b->Whole) return FALSE;
  return (a->Whole >= 0) ? (a->Fraction < b->Fraction)
    : (b->Fraction < a->Fraction);
}


byte FixedPointGreaterThanOrEqualTo(FixedPoint *a, FixedPoint *b) {
  if (a->Whole > b->Whole) return TRUE;
  if (a->Whole < b->Whole) return FALSE;
  return (a->Whole >= 0) ? (a->Fraction >= b->Fraction)
    : (b->Fraction >= a->Fraction);
}


byte FixedPointLessThanOrEqualTo(FixedPoint *a, FixedPoint *b) {
  if (a->Whole < b->Whole) return TRUE;
  if (a->Whole > b->Whole) return FALSE;
  return (a->Whole >= 0) ? (a->Fraction <= b->Fraction)
    : (b->Fraction <= a->Fraction);
}


#endif
