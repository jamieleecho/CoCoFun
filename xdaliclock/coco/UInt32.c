/*
  UInt32.c
  
  This software is for demonstration purposes only. Use at your own
  risk.
*/


#ifndef _UInt32_c
#define _UInt32_c

#include "UInt32.h"
#include "cmoc.h"


/** Table holding powers of 10 */
UInt32 UInt32PowersOf10[] = {
  UInt32Init(0, 1),
  UInt32Init(0, 10),
  UInt32Init(0, 100),
  UInt32Init(0, 1000),
  UInt32Init(0, 10000),
  UInt32Init(0x1, 0x86a0),
  UInt32Init(0xf, 0x4240),
  UInt32Init(0x98, 0x9680),
  UInt32Init(0x5f5, 0xe100),
};

/** -1 */
UInt32 UInt32MinusOne = UInt32Init(-1, 0);

/** 0 */
UInt32 UInt32Zero = UInt32Init(0, 0);


asm void UInt32Set(UInt32 *c, int whole, unsigned decimal) {
  asm {
    ldx     2,s             variable c
    ldd     4,s             variable whole
    std     ,x
    ldd     6,s             variable decimal
    std     2,x
  }
}


asm void UInt32Negate(UInt32 *c, UInt32 *a) {
  asm {
* Transfer *a into *c
    ldy 4,s
    ldx 2,s
    ldd ,y
    std ,x
    ldd 2,y
    std 2,x

    neg 3,x
    bcs UInt32NegateCom2
    neg 2,x
    bcs UInt32NegateCom1
    neg 1,x
    bcs UInt32NegateCom0
    neg ,x
    bra UInt32NegateDone

UInt32NegateCom2:
    com 2,x
UInt32NegateCom1:
    com 1,x
UInt32NegateCom0:
    com ,x
UInt32NegateDone:
  }  
}


asm void UInt32Add(UInt32 *c, UInt32 *a, UInt32 *b) {
  asm {
    pshs u
    ldx 6,s
    ldy 4,s
    ldu 8,s
    
    lda 3,x
    adda 3,u
    sta 3,y
    lda 2,x
    adca 2,u
    sta 2,y
    lda 1,x
    adca 1,u
    sta 1,y
    lda ,x
    adca ,u
    sta ,y

    puls u
  }
}


void UInt32Sub(UInt32 *c, UInt32 *a, UInt32 *b) {
  UInt32 tmp;
  UInt32Negate(&tmp, b);
  UInt32Add(c, a, &tmp);
}


void UInt32Mul(UInt32 *c, UInt32 *a, UInt32 *b) {
  byte results[8], aa[4], bb[4];
  memset(results, 0, sizeof(results));

  // Convert all operands to positive, count number of negates
  memcpy(aa, (byte *)a, sizeof(aa));
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
    lbsr UInt32MulCarry4

    lda 1,x
    ldb 3,y
    mul
    addd 4,u
    std 4,u

* Multiply second byte of y
    lda 3,x
    ldb 2,y
    mul
    addd 5,u
    std 5,u
    lbsr UInt32MulCarry4

    lda 2,x
    ldb 2,y
    mul
    addd 4,u
    std 4,u

* Multiply third byte of y
    lda 3,x
    ldb 1,y
    mul
    addd 4,u
    std 4,u

    bra UInt32MulAsmDone

*******************************************************************************
* Routine for performing carry results on product
*******************************************************************************
UInt32MulCarry4:
    bcc UInt32MulCarryDone
    lda 4,u
    adda #1
    sta 4,u
UInt32MulCarryDone:
    rts

*******************************************************************************
* Invoked when calculation is complete
*******************************************************************************
UInt32MulAsmDone:
    puls u
  }

  // Make result negative if needed
  c->Hi = ((int)results[4] << 8) + results[5];
  c->Lo = ((unsigned)results[6] << 8) + results[7];
}


void UInt32Div(UInt32 *c, UInt32 *a, UInt32 *b) {
  // aa[0] = dividend, aa[1] = divisor, aa[2] = quotient
  UInt32 aa[3];
  char numDivisorShifts = 0, numDividendShifts = 0;
  memcpy((byte *)(aa + 0), a, sizeof(aa[0]));
  memcpy((byte *)(aa + 1), b, sizeof(aa[1]));
  memset((byte *)(aa + 2), 0, sizeof(aa[2]));

  // Can't divide by zero, set to big value
  if ((aa[1].Hi== 0) && (aa[1].Lo== 0)) {
    c->Hi = 0xffff;
    c->Lo = 0xffff;
    return;
  }

  // Result is zero, so return 0
  if ((aa[0].Hi == 0) && (aa[0].Lo == 0)) {
    memset(c, 0, sizeof(*c));
    return;
  }

  // Note to self: Never ever do a div routine again
  asm {
* The first thing we have to do is shift the divisor left until its most
* significant bit is 1
    leax aa
UInt32DivCheckDivisor:
    lda 4,x
    bmi UInt32DivFixDividend
    asl 7,x
    rol 6,x
    rol 5,x
    rol 4,x
    inc numDivisorShifts
    bra UInt32DivCheckDivisor

* We now have to fix the dividend so that its most significant bit is 1
UInt32DivFixDividend:
    lda ,x    
    bmi UInt32DivMainSetup
    asl 3,x
    rol 2,x
    rol 1,x
    rol ,x
    inc numDividendShifts
    bra UInt32DivFixDividend

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
UInt32DivMainSetup:
    ldb #1      * B = max number of loops
    subb numDividendShifts
    addb numDivisorShifts    

    tstb
    ble UInt32DivMainLoopEnd

UInt32DivMainLoop:
* Compare dividend to divisor
    bsr UInt32DivCompareDividendToDivisor

* Branch if dividend too small
    blo UInt32DivDividendTooSmall
    bra UInt32DivPut1InQuotient

UInt32DivDividendTooSmall:
    decb
    beq UInt32DivMainLoopEnd
    bsr UInt32DivShiftDividendLeft
    bsr UInt32DivShiftQuotientLeft

UInt32DivPut1InQuotient:
* Put 1 in quotient, subtract divisor from dividend
    lda #1
    ora 11,x
    sta 11,x

UInt32DivSubtractAndShift:
    bsr UInt32DivSubtractDivisorFromDividend

UInt32DivMainLoopRepeat:
    lda ,x
    bmi UInt32DivMainLoop

    decb
    beq UInt32DivMainLoopEnd

    bsr UInt32DivShiftDividendLeft
    bsr UInt32DivShiftQuotientLeft
    bra UInt32DivMainLoopRepeat

UInt32DivShiftQuotientLeft:
    asl 11,x
    rol 10,x
    rol 9,x
    rol 8,x
    rts

UInt32DivShiftDividendLeft:
    asl 3,x
    rol 2,x
    rol 1,x
    rol ,x
    rts

UInt32DivCompareDividendToDivisor:
    lda ,x 
    cmpa 4,x
    bne UInt32DivCompareDividendToDivisorDone
    lda 1,x 
    cmpa 5,x
    bne UInt32DivCompareDividendToDivisorDone
    lda 2,x 
    cmpa 6,x
    bne UInt32DivCompareDividendToDivisorDone
    lda 3,x 
    cmpa 7,x
UInt32DivCompareDividendToDivisorDone:
    rts

UInt32DivSubtractNegateDivisor:
    neg 7,x
    bcs UInt32DivSubtractNegateDivisorCom2
    neg 6,x
    bcs UInt32DivSubtractNegateDivisorCom1
    neg 5,x
    bcs UInt32DivSubtractNegateDivisorCom0
    neg 4,x
    rts

UInt32DivSubtractNegateDivisorCom2:
    com 6,x
UInt32DivSubtractNegateDivisorCom1:
    com 5,x
UInt32DivSubtractNegateDivisorCom0:
    com 4,x
    rts

UInt32DivSubtractDivisorFromDividend:
    bsr UInt32DivSubtractNegateDivisor
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
    bsr UInt32DivSubtractNegateDivisor
    rts

UInt32DivMainLoopEnd:
  }

  // Transfer data, making result negative if needed
  memcpy((byte *)c, (byte *)(aa + 2), sizeof(aa[2]));
}


void UInt32Mod(UInt32 *c, UInt32 *d, UInt32 *a, UInt32 *b) {
  // aa[0] = dividend, aa[1] = divisor, aa[2] = quotient
  UInt32 aa[3];
  char numDivisorShifts = 0, numDividendShifts = 0, numTotalShifts;
  memset((byte *)(aa + 2), 0, sizeof(aa[2]));

  memcpy((byte *)aa, (byte *)a, sizeof(aa[0]));
  memcpy((byte *)(aa + 1), (byte *)b, sizeof(aa[1]));

  // Can't divide by zero, set to big value
  if ((aa[1].Hi == 0) && (aa[1].Lo == 0)) {
    c->Hi = 0xffff;
    c->Lo = 0xffff;
    d->Hi = 0xffff;
    d->Lo = 0xffff;
    return;
  }

  // Result is zero, so return 0
  if ((aa[0].Hi == 0) && (aa[0].Lo == 0)) {
    memset(c, 0, sizeof(*c));
    memset(d, 0, sizeof(*d));
    return;
  }

  // Note to self: Never ever do a div routine again
  asm {
* The first thing we have to do is shift the divisor left until its most
* significant bit is 1
    leax aa
UInt32ModCheckDivisor:
    lda 4,x
    bmi UInt32ModFixDividend
    asl 7,x
    rol 6,x
    rol 5,x
    rol 4,x
    inc numDivisorShifts
    bra UInt32ModCheckDivisor

* We now have to fix the dividend so that its most significant bit is 1
UInt32ModFixDividend:
    lda ,x    
    bmi UInt32ModMainSetup
    asl 3,x
    rol 2,x
    rol 1,x
    rol ,x
    inc numDividendShifts
    bra UInt32ModFixDividend

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
UInt32ModMainSetup:
    ldb #1      * B = max number of loops
    subb numDividendShifts
    addb numDivisorShifts    
    stb numTotalShifts    
    
    lble UInt32ModMainLoopEnd

UInt32ModMainLoop:
* Compare dividend to divisor
    lbsr UInt32DivCompareDividendToDivisor

* Branch if dividend too small
    blo UInt32ModDividendTooSmall
    bra UInt32ModPut1InQuotient

UInt32ModDividendTooSmall:
    dec numTotalShifts    
    decb
    beq UInt32ModMainLoopEnd
    lbsr UInt32DivShiftDividendLeft
    lbsr UInt32DivShiftQuotientLeft

UInt32ModPut1InQuotient:
* Put 1 in quotient, subtract divisor from dividend
    lda #1
    ora 11,x
    sta 11,x

UInt32ModSubtractAndShift:
    lbsr UInt32DivSubtractDivisorFromDividend

UInt32ModMainLoopRepeat:
    lda ,x
    bmi UInt32ModMainLoop

    dec numTotalShifts    
    decb
    beq UInt32ModMainLoopEnd

    lbsr UInt32DivShiftDividendLeft
    lbsr UInt32DivShiftQuotientLeft
    bra UInt32ModMainLoopRepeat

* Shift the quotient into the proper place
UInt32ModMainLoopEnd:
    ldb numTotalShifts
    beq UInt32ModMainLoopEnd3
UInt32ModMainLoopEnd2:
    lbsr UInt32DivShiftQuotientLeft
    decb
    bne UInt32ModMainLoopEnd2

* Shift the remainder into the proper place
UInt32ModMainLoopEnd3:
    clrb
    addb numDivisorShifts    
    subb numTotalShifts
    beq UInt32ModMainLoopEnd5
UInt32ModMainLoopEnd4:
    asr ,x
    ror 1,x
    ror 2,x
    ror 3,x
    lda #$7f
    anda ,x
    sta ,x
    decb
    bne UInt32ModMainLoopEnd4
   
UInt32ModMainLoopEnd5:
  }

  // Transfer data, making result negative if needed
  memcpy((byte *)c, (byte *)(aa + 2), sizeof(aa[2]));
  memcpy((byte *)d, (byte *)(aa), sizeof(aa[0]));
  return;

  // Force UInt32Div to not get optimized away
  UInt32Div(&aa[0], &aa[0], &aa[0]);
}


byte UInt32Equals(UInt32 *a, UInt32 *b) {
  return (a->Hi == b->Hi) && (a->Lo == b->Lo);
}


byte UInt32GreaterThan(UInt32 *a, UInt32 *b) {
  if (a->Hi > b->Hi) return TRUE;
  if (a->Hi < b->Hi) return FALSE;
  return (a->Lo > b->Lo);
}


byte UInt32LessThan(UInt32 *a, UInt32 *b) {
  if (a->Hi < b->Hi) return TRUE;
  if (a->Hi > b->Hi) return FALSE;
  return (a->Lo < b->Lo);
}


byte UInt32GreaterThanOrEqualTo(UInt32 *a, UInt32 *b) {
  if (a->Hi >= b->Hi) return TRUE;
  if (a->Hi <= b->Hi) return FALSE;
  return (a->Lo >= b->Lo);
}


byte UInt32LessThanOrEqualTo(UInt32 *a, UInt32 *b) {
  if (a->Hi <= b->Hi) return TRUE;
  if (a->Hi >= b->Hi) return FALSE;
  return (a->Lo <= b->Lo);
}


UInt32 *UInt32Copy(UInt32 *b, UInt32 *a) {
  memcpy((byte *)b, (byte *)a, sizeof(*a));
  return b;
}


char *UInt32ToA(char *buffer, UInt32 *a) {
  // Note that this algorithm uses mod. It is probably more
  // efficient to multiply by the reciprocal to get the digit
  // and due a UInt32 / integer multiply and subtract to
  // get the remainder.

  UInt32 quotient, remainder;
  byte powerIndex = 0;
  char *startBuffer = buffer;
  UInt32Copy(&remainder, a);

  // Output the whole part
  for(byte ii=0; ii<5; ii++) {
    UInt32Mod(&quotient, &remainder, &remainder,
		  UInt32PowersOf10 + powerIndex);
    *buffer++ = '0' + (byte)quotient.Hi;
    powerIndex++;
  }

  // Deal with the fractional part
  if (remainder.Lo != 0) {
    *buffer++ = '.';
    for(byte ii=0; ii<4; ii++) {
      UInt32Mod(&quotient, &remainder, &remainder,
		    UInt32PowersOf10 + powerIndex);
      
      // Deal with round off error.
      if (quotient.Hi > 9) {
	// This becomes 0 and we carry 1 all the way up
	quotient.Hi = 0;
	for(char *ptr = buffer - 1; ptr >= startBuffer; ptr--) {
	  char c = *ptr;
	  if ((c == '.') || (c == '-')) continue;
	  c = c + 1;
	  if (c <= '9') {
	    *ptr = c;
	    break;
	  }
	  
	  c = '0';
	  *ptr = c;
	}
      }
      
      *buffer++ = '0' + (byte)quotient.Hi;
      powerIndex++;
    }

    // Rewind buffer to the point to the last non-zero char
    for(buffer--; (buffer > startBuffer) && (*buffer == '0'); buffer--);
    buffer += (*buffer == '.') ? 0 : 1;
  }
  
  // Find the first non-zero char
  char *ptr = startBuffer;
  char *ptr2 = buffer - 1;
  for(; (ptr < ptr2) && (*ptr == '0' || *ptr == '-'); ptr++);
  if (*ptr == '.') ptr--; // make sure we show 0.1 not .1

  // Copy the text to the beginning
  byte numChars = (byte)(buffer - ptr);
  memcpy(startBuffer, ptr, numChars);
  startBuffer[numChars] = '\0';
  return startBuffer + numChars - 1;
}


void UInt32Parse(UInt32 *a, char *buffer) {
  // Zero out a by default
  UInt32Set(a, 0, 0);

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
  int powerIndex = 5;
  UInt32 tmp;
  for(c = *buffer++; ((c >= '0' && c <= '9') || (c == '.')) ; c = *buffer++) {
    if (c == '.') {
      if (sawDecimalPoint) break;
      sawDecimalPoint = TRUE;
      continue;
    }

    UInt32Set(&tmp, c - '0', 0);
    if (sawDecimalPoint) {
      UInt32Mul(&tmp, &tmp, UInt32PowersOf10 + powerIndex);
      UInt32Add(a, a, &tmp);
      powerIndex++;
      if (powerIndex >= sizeof(UInt32PowersOf10)/sizeof(UInt32PowersOf10[0]))
	break;
    } else {
      UInt32Mul(a, a, UInt32PowersOf10 + 3);
      UInt32Add(a, a, &tmp);
    }    
  }  

  if (resultIsNegative)
    UInt32Negate(a, a);
}

#endif
