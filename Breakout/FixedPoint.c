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
  c->Decimal = decimal;
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

  c->Whole = ((int)results[2] << 8) + results[3];
  c->Decimal = ((unsigned)results[4] << 8) + results[5];
  if (numNegatives & 1)
    FixedPointNegate(c, c);
}


#endif
