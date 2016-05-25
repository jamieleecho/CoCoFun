#include <coco.h>
#include "tester.h"

#include "c_stuff.h"
#include "FixedPoint.h"

#include "c_stuff.c"
#include "FixedPoint.c"
#include "tester.c"


void testFixedPointToA() {
  FixedPoint val1 = FixedPointInit(0x1234, 0x0000);
  FixedPoint val2 = FixedPointInit(0x0000, 0x1234);
  char b1[15], b2[15];
  FixedPointToA(b1, &val1);
  FixedPointToA(b2, &val2);
  assertEqualString("4660", b1)
  assertEqualString("0.0711", b2)
}


void testFixedPointMod() {
  FixedPoint val1 = FixedPointInit(0x7fff, 0x0000);
  FixedPoint val2 = FixedPointInit(0x1234, 0x5678);
  FixedPoint val3, val4;
  FixedPointMod(&val3, &val4, &val2, &val1);
  assertEqual(val3.Whole, 0);
  assertEqual(val3.Fraction, 0);
  assertEqual(val4.Whole, 0x1234);
  assertEqual(val4.Fraction, 0x5678);
}


int main() {
  initCoCoSupport();
  width(80);

  // Test FixedPoint routines
  testFunctions(NF(testFixedPointToA),
                NF(testFixedPointMod),
                NULL, NULL);

  return 0;
}
