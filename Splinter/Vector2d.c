/*
  Vector2d.c
  
  This software is for demonstration purposes only. Use at your own
  risk.
*/


#include <cmoc.h>

#include "Vector2d.h"


void Vector2dZero(Vector2d *a) {
  memset(a, 0, sizeof(*a));
}


void Vector2dAdd(Vector2d *a, Vector2d *b, Vector2d *c) {
  FixedPointAdd(&(a->data), &(b->data), &(c->data));
  FixedPointAdd(&(a->data[1]), &(b->data[1]), &(c->data[1]));
}


void Vector2dSub(Vector2d *a, Vector2d *b, Vector2d *c) {
  FixedPointSub(&(a->data[0]), &(b->data[0]), &(c->data[0]));
  FixedPointSub(&(a->data[1]), &(b->data[1]), &(c->data[1]));
}


void Vector2dMul(Vector2d *a, FixedPoint *b, Vector2d *c) {
  FixedPointMul(&(a->data), b, &(c->data));
  FixedPointMul(&(a->data[1]), b, &(c->data[1]));
}


void Vector2dDot(FixedPoint *a, Vector2d *b, Vector2d *c) {
  FixedPoint tmp;
  FixedPointMul(&tmp, &(b->data), &(c->data));
  FixedPointMul(a, &(b->data[1]), &(c->data[1]));
  FixedPointAdd(a, &tmp, a);
}


void Vector2dNormalize(Vector2d *a, Vector2d *b) {
  Vector2d tmp;
  memcpy(&tmp, b, sizeof(tmp));
  FixedPointMul(&(a->data), &(tmp.data),  &(tmp.data));
  FixedPointMul(&(a->data[1]), &(tmp.data[1]),  &(tmp.data[1]));
  FixedPointAdd(&(a->data[1]), &(a->data),  &(a->data[1]));
  FixedPointSqrt(&(a->data), &(a->data[1]));
  FixedPointDiv(&(a->data[1]), &(tmp.data[1]), &(a->data));
  FixedPointDiv(&(a->data), &(tmp.data), &(a->data));
}


void Vector2dLength(FixedPoint *a, Vector2d *b) {
  FixedPoint tmp1;
  FixedPointMul(&tmp1, &(b->data),  &(b->data));
  FixedPointMul(a, &(b->data[1]),  &(b->data[1]));
  FixedPointAdd(&tmp1, &tmp1,  a);
  FixedPointSqrt(a, &tmp1);
}


void Vector2dReflectionVector(Vector2d *a, Vector2d *b, Vector2d *n) {
  FixedPoint tmp1;
  Vector2d tmp2;
  Vector2dDot(&tmp1, b, n);
  FixedPointAdd(&tmp1, &tmp1, &tmp1);
  Vector2dMul(&tmp2, &tmp1, n);
  Vector2dSub(a, b, &tmp2);
}


char *Vector2dToA(char *buffer, Vector2d *a) {
  *buffer++ = '[';
  buffer = FixedPointToA(buffer, &(a->data));
  *buffer++ = ' ';
  buffer = FixedPointToA(buffer, &(a->data[1]));
  *buffer++ = ']';
  *buffer++ = '\0';
  return buffer;
}
