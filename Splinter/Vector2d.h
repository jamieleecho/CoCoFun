/*
  Vector2d.h
  
  This software is for demonstration purposes only. Use at your own
  risk.
*/


#ifndef _Vector2d_h
#define _Vector2d_h

#include "FixedPoint.h"

/** Representation of a vector */
typedef struct Vector2d {
  FixedPoint data[2];
} Vector2d;


/**
 * Sets all elements to zero.
 * @param a[out] vector to clear
 */
void Vector2dZero(Vector2d *a);


/**
 * Computes a = b + c
 * @param a[out] return value
 * @param b[in] addend
 * @param c[in] addend
 */
void Vector2dAdd(Vector2d *a, Vector2d *b, Vector3d *c);


/**
 * Computes a = b - c
 * @param a[out] return value
 * @param b[in] quantity
 * @param c[in] subtractand
 */
void Vector2dSub(Vector2d *a, Vector2d *b, Vector3d *c);


/**
 * Computes a = b * c
 * @param a[out] return value
 * @paran b[in] scalar multiplier
 * @param c[in] vector to multiply
 */
void Vector2dMul(Vector2d *a, FixedPoint *b, Vector3d *c);


/**
 * Computes a = b dot c
 * @param a[out] return value
 * @paran b[in] scalar multiplier
 * @param c[in] vector to multiply
 */
void Vector2dMul(Vector2d *a, FixedPoint *b, Vector3d *c);

/** 
 * For initializing FixedPoint numbers.
 * @param w[in] whole part
 * @param f[in] fractional part
 */
#define FixedPointInit(w, f) { (w), (f) }

/** 
 * Initize the FixedPoint number to pi
 */
#define FixedPointInitPi() { 3, 0x2434 }

/** 
 * Initize the FixedPoint number to -pi
 */
#define FixedPointInitMinusPi() { 0xfffc, 0xdbcc }

/** 
 * Initize the FixedPoint number to pi/2
 */
#define FixedPointInitPiOver2() { 1, 0x9210 }

/** 
 * Initize the FixedPoint number to -pi/2
 */
#define FixedPointInitMinusPiOver2() { 0xfffe, 0x6df0 }

/** 
 * Initize the FixedPoint number to e
 */
#define FixedPointInitE() { 2, 0xb7d2 }

/**
 * Returns a new FixedPoint containing whole.fraction
 * @param c[out] whole.fraction
 * @param whole[in] whole
 * @param decimal[in] fraction
 */
void FixedPointSet(FixedPoint *c, int whole, unsigned decimal);


/**
 * @param c[out] -input
 * @param a[in] input
 */
void FixedPointNegate(FixedPoint *c, FixedPoint *a);


/**
 * Returns *a + *b
 * @param c[out] sum
 * @param a[in] addened 1
 * @param b[in] addened 2
 */
void FixedPointAdd(FixedPoint *c, FixedPoint *a, FixedPoint *b);


/**
 * Returns *a - *b
 * @param c[out] difference
 * @param a[in] minuend
 * @param b[in] subtrahend
 */
void FixedPointSub(FixedPoint *c, FixedPoint *a, FixedPoint *b);


/**
 * Returns *a * *b
 * @param c[out] product
 * @param a[in] multiplicand
 * @param b[in] multiplicand
 */
void FixedPointMul(FixedPoint *c, FixedPoint *a, FixedPoint *b);


#endif
