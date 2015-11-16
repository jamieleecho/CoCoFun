/*
  Vector2d.c
  
  This software is for demonstration purposes only. Use at your own
  risk.
*/


#ifndef _Vector2d_c
#define _Vector2d_c

#include "Vector2d.h"


void Vector2dZero(Vector2d *a) {
  memset(a, 0, sizeof(*a));
}


void Vector2dAdd(Vector2d *a, Vector2d *b, Vector2d *c) {
  FixedPointAdd(&(a->data), &(b->data), &(c->data));
  FixedPointAdd(&(a->data + 1), &(b->data + 1), &(c->data + 1));
}


void Vector2dSub(Vector2d *a, Vector2d *b, Vector2d *c) {
  FixedPointSub(&(a->data), &(b->data), &(c->data));
  FixedPointSub(&(a->data + 1), &(b->data + 1), &(c->data + 1));
}


void Vector2dMul(Vector2d *a, FixedPoint *b, Vector2d *c) {
  FixedPointSub(&(a->data), b, &(c->data));
  FixedPointSub(&(a->data + 1), b, &(c->data + 1));
}


/**
 * Computes a = b dot c
 * @param a[out] return value
 * @paran b[in] scalar multiplier
 * @param c[in] vector to multiply
 */
void Vector2dDot(FixedPoint *a, Vector2d *b, Vector2d *c) {
}


/**
 * Computes a unit vector from b
 * @param a[out] unit vector
 * @paran b[in] non-zero input vector
 */
void Vector2dUnit(Vector2d *a, Vector2d *b) {
}


/**
 * Computes the length of b.
 * @param a[out] length of b
 * @param b[in] input vector
 */
void Vector2dLength(FixedPoint *a, Vector2d *b) {
}


#endif
