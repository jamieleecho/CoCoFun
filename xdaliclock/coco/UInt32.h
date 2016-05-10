/*
  UInt32.h
  
  This software is for demonstration purposes only. Use at your own
  risk.
*/


#ifndef _UInt32_h
#define _UInt32_h


/** UInt32 data structure */
typedef struct UInt32 {
  /** MSB */
  unsigned Hi;

  /** LSB */
  unsigned Lo;
} UInt32;


/** Must be invoked before using other UInt32 functions. */
void UInt32Initialize();

/** 
 * For initializing UInt32 numbers.
 * @param h[in] Hi part
 * @param l[in] Lo part
 */
#define UInt32Init(h, l) { (h), (l) }

/**
 * Returns a new UInt32 containing hi.lo
 * @param c[out] hi.lo
 * @param hi[in] hi part
 * @param lo[in] lo part
 */
void UInt32Set(UInt32 *c, int hi, unsigned lo);

/**
 * @param c[out] -input
 * @param a[in] input
 */
void UInt32Negate(UInt32 *c, UInt32 *a);

/**
 * Returns *a + *b
 * @param c[out] sum
 * @param a[in] addened 1
 * @param b[in] addened 2
 */
void UInt32Add(UInt32 *c, UInt32 *a, UInt32 *b);

/**
 * Returns *a - *b
 * @param c[out] difference
 * @param a[in] minuend
 * @param b[in] subtrahend
 */
void UInt32Sub(UInt32 *c, UInt32 *a, UInt32 *b);

/**
 * Returns *a * *b
 * @param c[out] product
 * @param a[in] multiplicand
 * @param b[in] multiplicand
 */
void UInt32Mul(UInt32 *c, UInt32 *a, UInt32 *b);

/**
 * Returns *a / *b
 * @param c[out] quotient
 * @param a[in] dividend
 * @param b[in] divisor
 */
void UInt32Div(UInt32 *c, UInt32 *a, UInt32 *b);

/**
 * Returns *a / *b and *a % *b
 * @param c[out] quotient
 * @param d[out] mod
 * @param a[in] dividend
 * @param b[in] divisor
 */
void UInt32Mod(UInt32 *c, UInt32 *d, UInt32 *a, UInt32 *b);


/**
 * Returns *a == *b
 * @param a[in] arg1
 * @param b[in] arg2
 * @return *a == *b
 */
byte UInt32Equals(UInt32 *a, UInt32 *b);

/**
 * Returns *a > *b
 * @param a[in] arg1
 * @param b[in] arg2
 * @return *a > *b
 */
byte UInt32GreaterThan(UInt32 *a, UInt32 *b);

/**
 * Returns *a < *b
 * @param a[in] arg1
 * @param b[in] arg2
 * @return *a < *b
 */
byte UInt32LessThan(UInt32 *a, UInt32 *b);

/**
 * Returns *a >= *b
 * @param a[in] arg1
 * @param b[in] arg2
 * @return *a >= *b
 */
byte UInt32GreaterThanOrEqualTo(UInt32 *a, UInt32 *b);

/**
 * Returns *a <= *b
 * @param a[in] arg1
 * @param b[in] arg2
 * @return *a <= *b
 */
byte UInt32LessThanOrEqualTo(UInt32 *a, UInt32 *b);

/**
 * Copies a to b.
 * @param b[out] destination
 * @param a[in] source
 * @return b
 */
UInt32 *UInt32Copy(UInt32 *b, UInt32 *a);

/**
 * Creates an ascii decimal version of a.
 * @param buffer[out] output buffer of at least 12 bytes
 * @param a[in] input
 */
char *UInt32ToA(char *buffer, UInt32 *a);

/**
 * Parses buffer into a UInt32 value.
 * @param a[out] output parsed result
 * @param buffer[in] input
 */
void UInt32Parse(UInt32 *a, char *buffer);

#endif
