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
  
}


#endif
