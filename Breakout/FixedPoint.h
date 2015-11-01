/*
  FixedPoint.h
  
  This software is for demonstration purposes only. Use at your own
  risk.
*/


#ifndef _FixedPoint_h
#define _FixedPoint_h


/** FixedPoint data structure */
typedef struct FixedPoint {
  /** Whole part */
  int Whole;

  /** Fractional Part */
  unsigned Fraction;
} FixedPoint;


/** 
 * For initializing FixedPoint numbers.
 * @param w[in] whole part
 * @param f[in] fractional part
 */
#define FixedPointInit(w, f) { (w), (f) }

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


/**
 * Returns *a / *b
 * @param c[out] quotient
 * @param a[in] dividend
 * @param b[in] divisor
 */
void FixedPointDiv(FixedPoint *c, FixedPoint *a, FixedPoint *b);


/**
 * Returns *a / *b and *a % *b
 * @param c[out] quotient
 * @param d[out] mod
 * @param a[in] dividend
 * @param b[in] divisor
 */
void FixedPointMod(FixedPoint *c, FixedPoint *d, FixedPoint *a, FixedPoint *b);


/**
 * Returns *a == *b
 * @param a[in] arg1
 * @param b[in] arg2
 * @return *a == *b
 */
byte FixedPointEquals(FixedPoint *a, FixedPoint *b);

/**
 * Returns *a > *b
 * @param a[in] arg1
 * @param b[in] arg2
 * @return *a > *b
 */
byte FixedPointGreaterThan(FixedPoint *a, FixedPoint *b);

/**
 * Returns *a < *b
 * @param a[in] arg1
 * @param b[in] arg2
 * @return *a < *b
 */
byte FixedPointLessThan(FixedPoint *a, FixedPoint *b);

/**
 * Returns *a >= *b
 * @param a[in] arg1
 * @param b[in] arg2
 * @return *a >= *b
 */
byte FixedPointGreaterThanOrEqualTo(FixedPoint *a, FixedPoint *b);

/**
 * Returns *a <= *b
 * @param a[in] arg1
 * @param b[in] arg2
 * @return *a <= *b
 */
byte FixedPointLessThanOrEqualTo(FixedPoint *a, FixedPoint *b);

/**
 * Copies a to b.
 * @param b[out] destination
 * @param a[in] source
 * @return b
 */
FixedPoint *FixedPointCopy(FixedPoint *b, FixedPoint *a);

/**
 * Creates an ascii decimal version of a.
 * @param buffer[out] output buffer of at least 12 bytes
 * @param a[in] input
 */
void FixedPointToA(char *buffer, FixedPoint *a);

/**
 * Parses buffer into a FixedPoint value.
 * @param a[out] output parsed result
 * @param a[in] input
 */
void FixedPointParse(FixedPoint *a, char *buffer);

#endif
