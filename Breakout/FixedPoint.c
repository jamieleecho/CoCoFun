/*
  FixedPoint.c
  
  This software is for demonstration purposes only. Use at your own
  risk.
*/


#ifndef _FixedPoint_c
#define _FixedPoint_c

#include "FixedPoint.h"


asm void FixedPointSet(FixedPoint *c, int whole, unsigned decimal) {
  asm {
    ldx     2,s             variable c
    ldd     4,s             variable whole
    std     ,x
    ldd     6,s             variable decimal
    std     2,x
  }
}


asm void FixedPointNegate(FixedPoint *c, FixedPoint *a) {
  asm {
* Transfer *a into *c
    ldy 4,s
    ldx 2,s
    ldd ,y
    std ,x
    ldd 2,y
    std 2,x

    neg 3,x
    bcs FixedPointNegateCom2
    neg 2,x
    bcs FixedPointNegateCom1
    neg 1,x
    bcs FixedPointNegateCom0
    neg ,x
    bra FixedPointNegateDone

FixedPointNegateCom2:
    com 2,x
FixedPointNegateCom1:
    com 1,x
FixedPointNegateCom0:
    com ,x
FixedPointNegateDone:
  }  
}


asm void FixedPointAdd(FixedPoint *c, FixedPoint *a, FixedPoint *b) {
  asm {
    pshs u
    ldx 6,s
    ldy 4,s
    ldu 8,s
    
    lda 3,x
    adda 3,u
    sta 3,y
    lda 2,x
    adda 2,u
    sta 2,y
    lda 1,x
    adda 1,u
    sta 1,y
    lda ,x
    adda ,u
    sta ,y

    puls u
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


void FixedPointDiv(FixedPoint *c, FixedPoint *a, FixedPoint *b) {
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

    tstb
    ble FixedPointDivMainLoopEnd
    incb

FixedPointDivMainLoop:
* Compare dividend to divisor
    bsr FixedPointDivCompareDividendToDivisor

* Branch if dividend too small
    blo FixedPointDivDividendTooSmall
    bra FixedPointDivPut1InQuotient

FixedPointDivDividendTooSmall:
    decb
    beq FixedPointDivMainLoopEnd
    bsr FixedPointDivShiftDividendLeft
    bsr FixedPointDivShiftQuotientLeft

FixedPointDivPut1InQuotient:
* Put 1 in quotient, subtract divisor from dividend
    lda #1
    ora 11,x
    sta 11,x

FixedPointDivSubtractAndShift:
    bsr FixedPointDivSubtractDivisorFromDividend

FixedPointDivMainLoopRepeat:
    lda ,x
    bmi FixedPointDivMainLoop

    decb
    beq FixedPointDivMainLoopEnd

    bsr FixedPointDivShiftDividendLeft
    bsr FixedPointDivShiftQuotientLeft
    bra FixedPointDivMainLoopRepeat

FixedPointDivShiftQuotientLeft:
    asl 11,x
    rol 10,x
    rol 9,x
    rol 8,x
    rts

FixedPointDivShiftDividendLeft:
    asl 3,x
    rol 2,x
    rol 1,x
    rol ,x
    rts

FixedPointDivCompareDividendToDivisor:
    lda ,x 
    cmpa 4,x
    bne FixedPointDivCompareDividendToDivisorDone
    lda 1,x 
    cmpa 5,x
    bne FixedPointDivCompareDividendToDivisorDone
    lda 2,x 
    cmpa 6,x
    bne FixedPointDivCompareDividendToDivisorDone
    lda 3,x 
    cmpa 7,x
FixedPointDivCompareDividendToDivisorDone:
    rts

FixedPointDivSubtractNegateDivisor:
    neg 7,x
    bcs FixedPointDivSubtractNegateDivisorCom2
    neg 6,x
    bcs FixedPointDivSubtractNegateDivisorCom1
    neg 5,x
    bcs FixedPointDivSubtractNegateDivisorCom0
    neg 4,x
    rts

FixedPointDivSubtractNegateDivisorCom2:
    com 6,x
FixedPointDivSubtractNegateDivisorCom1:
    com 5,x
FixedPointDivSubtractNegateDivisorCom0:
    com 4,x
    rts

FixedPointDivSubtractDivisorFromDividend:
    bsr FixedPointDivSubtractNegateDivisor
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
    bsr FixedPointDivSubtractNegateDivisor
    rts

FixedPointDivMainLoopEnd:
  }

  // Transfer data, making result negative if needed
  if (numNegatives & 1) {
    FixedPointNegate(c, aa + 2);
  } else {
    memcpy((byte *)c, (byte *)(aa + 2), sizeof(aa[2]));
  }
}


void FixedPointMod(FixedPoint *c, FixedPoint *d, FixedPoint *a, FixedPoint *b) {
  // aa[0] = dividend, aa[1] = divisor, aa[2] = quotient
  FixedPoint aa[3];
  char numDivisorShifts = 0, numDividendShifts = 0, numTotalShifts = 16;
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
FixedPointModCheckDivisor:
    lda 4,x
    bmi FixedPointModFixDividend
    asl 7,x
    rol 6,x
    rol 5,x
    rol 4,x
    inc numDivisorShifts
    bra FixedPointModCheckDivisor

* We now have to fix the dividend so that its most significant bit is 1
FixedPointModFixDividend:
    lda ,x    
    bmi FixedPointModMainSetup
    asl 3,x
    rol 2,x
    rol 1,x
    rol ,x
    inc numDividendShifts
    bra FixedPointModFixDividend

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
FixedPointModMainSetup:
    ldb #16      * B = max number of loops
    subb numDividendShifts
    addb numDivisorShifts    
    stb numTotalShifts    
    subb #15
    
    lble FixedPointModMainLoopEnd
    inc numTotalShifts

FixedPointModMainLoop:
* Compare dividend to divisor
    lbsr FixedPointDivCompareDividendToDivisor

* Branch if dividend too small
    blo FixedPointModDividendTooSmall
    bra FixedPointModPut1InQuotient

FixedPointModDividendTooSmall:
    dec numTotalShifts    
    decb
    beq FixedPointModMainLoopEnd
    lbsr FixedPointDivShiftDividendLeft
    lbsr FixedPointDivShiftQuotientLeft

FixedPointModPut1InQuotient:
* Put 1 in quotient, subtract divisor from dividend
    lda #1
    ora 11,x
    sta 11,x

FixedPointModSubtractAndShift:
    lbsr FixedPointDivSubtractDivisorFromDividend

FixedPointModMainLoopRepeat:
    lda ,x
    bmi FixedPointModMainLoop

    dec numTotalShifts    
    decb
    beq FixedPointModMainLoopEnd

    lbsr FixedPointDivShiftDividendLeft
    lbsr FixedPointDivShiftQuotientLeft
    bra FixedPointModMainLoopRepeat

* Shift the quotient into the proper place
FixedPointModMainLoopEnd:
    ldb numTotalShifts
    beq FixedPointModMainLoopEnd3
FixedPointModMainLoopEnd2:
    lbsr FixedPointDivShiftQuotientLeft
    decb
    bne FixedPointModMainLoopEnd2

* Shift the remainder into the proper place
FixedPointModMainLoopEnd3:
    ldb #16
    addb numDivisorShifts    
    subb numTotalShifts
    beq FixedPointModMainLoopEnd5
FixedPointModMainLoopEnd4:
    asr ,x
    ror 1,x
    ror 2,x
    ror 3,x
    lda #$7f
    anda ,x
    sta ,x
    decb
    bne FixedPointModMainLoopEnd4
   
FixedPointModMainLoopEnd5:
  }

  // Transfer data, making result negative if needed
  if (numNegatives & 1) {
    FixedPointNegate(d, aa);
    FixedPointNegate(c, aa + 2);
  } else {
    memcpy((byte *)c, (byte *)(aa + 2), sizeof(aa[2]));
    memcpy((byte *)d, (byte *)(aa), sizeof(aa[0]));
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


FixedPoint *FixedPointCopy(FixedPoint *b, FixedPoint *a) {
  memcpy((byte *)b, (byte *)a, sizeof(*a));
  return b;
}


void FixedPointToA(char *buffer, FixedPoint *a) {
  FixedPoint base = FixedPointInit(10000, 0);
  FixedPoint base10 = FixedPointInit(10, 0);
  FixedPoint quotient, remainder;

  // Deal with negatives
  if (a->Whole < 0) {
    FixedPointNegate(&remainder, a);
    *buffer++ = '-';
  } else {
    FixedPointCopy(&remainder, a);
  }

  // Output the whole part
  byte nonZeroChars = 0;
  for(int ii=0; ii<5; ii++) {
    FixedPointMod(&quotient, &remainder, &remainder, &base);
    if (quotient.Whole > 0)
      nonZeroChars++;
    if (nonZeroChars > 0)
      *buffer++ = '0' + (byte)quotient.Whole;
    FixedPointDiv(&base, &base, &base10);
  }

  if (nonZeroChars == 0)
      *buffer++ = '0';

  // Return if there is no fractional part
  if (remainder.Fraction == 0) {
    *buffer++ = '\0';
    return;
  }

  // Deal with the fractional part
  byte zeroChars = 1;  
  *buffer++ = '.';
  for(int ii=0; ii<4; ii++) {
    FixedPointMod(&quotient, &remainder, &remainder, &base);
    if (quotient.Whole > 0)
      zeroChars = 0;
    else
      zeroChars++;
      
    *buffer++ = '0' + (byte)quotient.Whole;
    FixedPointDiv(&base, &base, &base10);
  }
  
  // Put the null character at the last non-zero char
  *(buffer - zeroChars) = '\0';
}


void FixedPointParse(FixedPoint *a, char *buffer) {
  // Zero out a by default
  FixedPointSet(a, 0, 0);

  // Skip any whitespace in buffer
  char c;
  for(c = *buffer++; (c == ' ' || c == '\t' || c=='\0'); c = *buffer++);
  if (c == '\0') return;

  // First char may be + or -
  byte resultIsNegative = FALSE;
  if (c == '-') {
    resultIsNegative = TRUE;
  } else if (c == '+') {
  } else if (c == '\0') {
    return;
  } else {
    buffer--;
  }

  // Go through each character adding the digits
  byte sawDecimalPoint = FALSE;
  FixedPoint base10 = FixedPointInit(10, 0);
  FixedPoint base = FixedPointInit(1, 0);
  FixedPoint tmp;
  for(c = *buffer++; ((c >= '0' && c <= '9') || (c == '.')) ; c = *buffer++) {
    if (c == '.') {
      if (sawDecimalPoint) break;
      sawDecimalPoint = TRUE;
      continue;
    }

    FixedPointSet(&tmp, c - '0', 0);
    if (sawDecimalPoint) {
      FixedPointDiv(&base, &base, &base10);
      FixedPointMul(&tmp, &tmp, &base);
      FixedPointAdd(a, a, &tmp);
    } else {
      FixedPointMul(a, a, &base10);
      FixedPointAdd(a, a, &tmp);
    }    
  }
  
  if (resultIsNegative)
    FixedPointNegate(a, a);
}


#endif
