#include <coco.h>
#include "tester.h"

#include "c_stuff.h"
#include "FixedPoint.h"
#include "UInt32.h"

#include "c_stuff.c"
#include "FixedPoint.c"
#include "UInt32.c"
#include "tester.c"


void testUInt32Init() {
  UInt32 val = UInt32Init(0x1234, 0x5678);
  assertEqual(val.Hi, 0x1234);
  assertEqual(val.Lo, 0x5678);
}


void testUInt32Set() {
  UInt32 val;
  UInt32Set(&val, 0x1234, 0x5678);
  assertEqual(val.Hi, 0x1234);
  assertEqual(val.Lo, 0x5678);
}


void testUInt32Add() {
  UInt32 val1 = UInt32Init(0x1234, 0x5678);
  UInt32 val2 = UInt32Init(0x8345, 0xf123);
  UInt32 val3;
  UInt32Add(&val3, &val1, &val2);
  assertEqual(val3.Hi, 0x957a);
  assertEqual(val3.Lo, 0x479b);
}


void testUInt32Negate() {
  UInt32 val = UInt32Init(0x1234, 0x5678);
  UInt32Negate(&val, &val);
  assertEqual(val.Hi, 0xedcb);
  assertEqual(val.Lo, 0xa988);
}


void testUInt32Sub() {
  UInt32 val1 = UInt32Init(0x8345, 0xf123);
  UInt32 val2 = UInt32Init(0x1234, 0x5678);
  UInt32 val3;
  UInt32Sub(&val3, &val1, &val2);
  assertEqual(val3.Hi, 0x7111);
  assertEqual(val3.Lo, 0x9aab);
}


void testUInt32Mul() {
  UInt32 val1 = UInt32Init(0x1234, 0x5678);
  UInt32 val2 = UInt32Init(0x8345, 0xf123);
  UInt32 val3;
  UInt32Mul(&val3, &val1, &val2);
  assertEqual(val3.Hi, 0x78e6);
  assertEqual(val3.Lo, 0xca68);
}


void testUInt32Div() {
  UInt32 val1 = UInt32Init(0x0000, 0x8822);
  UInt32 val2 = UInt32Init(0x0000, 0x0000);
  UInt32 val3;
  UInt32Div(&val3, &val1, &val2);
  assertEqual(val3.Hi, 0xffff);
  assertEqual(val3.Lo, 0xffff);

  UInt32Div(&val3, &val2, &val1);
  assertEqual(val3.Hi, 0x0000);
  assertEqual(val3.Lo, 0x0000);

  UInt32Set(&val1, 0x8000, 0x0000);
  UInt32Div(&val3, &val1, &val1);
  assertEqual(val3.Hi, 0x0000);
  assertEqual(val3.Lo, 0x0001);

  UInt32Set(&val2, 0x0000, 0x8000);
  UInt32Div(&val3, &val1, &val2);
  assertEqual(val3.Hi, 0x0001);
  assertEqual(val3.Lo, 0x0000);

  UInt32Set(&val1, 0x4996, 0x02d2);
  UInt32Set(&val2, 0x0001, 0xe240);
  UInt32Div(&val3, &val1, &val2);
  assertEqual(val3.Hi, 0x0000);
  assertEqual(val3.Lo, 0x2710);

  UInt32Set(&val1, 0x1234, 0x5678);
  UInt32Set(&val2, 0x0000, 0x0001);
  UInt32Div(&val3, &val1, &val2);
  assertEqual(val3.Hi, 0x1234);
  assertEqual(val3.Lo, 0x5678);

  UInt32Set(&val2, 0x0000, 0x0002);
  UInt32Div(&val3, &val1, &val2);
  assertEqual(val3.Hi, 0x091a);
  assertEqual(val3.Lo, 0x2b3c);

  UInt32Set(&val1, 0x0001, 0x2345);
  UInt32Set(&val2, 0x0000, 0x0015);
  UInt32Div(&val3, &val1, &val2);
  assertEqual(val3.Hi, 0x0000);
  assertEqual(val3.Lo, 0x0dde);
}


void testUInt32Mod() {
  UInt32 val1 = UInt32Init(0x0000, 0x8822);
  UInt32 val2 = UInt32Init(0x0000, 0x0000);
  UInt32 val3, val4;
  UInt32Mod(&val3, &val4, &val1, &val2);
  assertEqual(val3.Hi, 0xffff);
  assertEqual(val3.Lo, 0xffff);
  assertEqual(val4.Hi, 0xffff);
  assertEqual(val4.Lo, 0xffff);

  UInt32Mod(&val3, &val4, &val2, &val1);
  assertEqual(val3.Hi, 0x0000);
  assertEqual(val3.Lo, 0x0000);
  assertEqual(val4.Hi, 0x0000);
  assertEqual(val4.Lo, 0x0000);

  UInt32Set(&val1, 0x8000, 0x0000);
  UInt32Mod(&val3, &val4, &val1, &val1);
  assertEqual(val3.Hi, 0x0000);
  assertEqual(val3.Lo, 0x0001);
  assertEqual(val4.Hi, 0x0000);
  assertEqual(val4.Lo, 0x0000);

  UInt32Set(&val2, 0x0000, 0x8000);
  UInt32Mod(&val3, &val4, &val1, &val2);
  assertEqual(val3.Hi, 0x0001);
  assertEqual(val3.Lo, 0x0000);
  assertEqual(val4.Hi, 0x0000);
  assertEqual(val4.Lo, 0x0000);

  UInt32Set(&val1, 0x4996, 0x02d2);
  UInt32Set(&val2, 0x0001, 0xe240);
  UInt32Mod(&val3, &val4, &val1, &val2);
  assertEqual(val3.Hi, 0x0000);
  assertEqual(val3.Lo, 0x2710);
  assertEqual(val4.Hi, 0x0000);
  assertEqual(val4.Lo, 0x1ed2);

  UInt32Set(&val1, 0x1234, 0x5678);
  UInt32Set(&val2, 0x0000, 0x0001);
  UInt32Mod(&val3, &val4, &val1, &val2);
  assertEqual(val3.Hi, 0x1234);
  assertEqual(val3.Lo, 0x5678);
  assertEqual(val4.Hi, 0x0000);
  assertEqual(val4.Lo, 0x0000);

  UInt32Set(&val2, 0x0000, 0x0002);
  UInt32Mod(&val3, &val4, &val1, &val2);
  assertEqual(val3.Hi, 0x091a);
  assertEqual(val3.Lo, 0x2b3c);
  assertEqual(val4.Hi, 0x0000);
  assertEqual(val4.Lo, 0x0000);

  UInt32Set(&val1, 0x0000, 0x1234);
  UInt32Set(&val2, 0x0000, 0x1235);
  UInt32Mod(&val3, &val4, &val1, &val2);
  assertEqual(val3.Hi, 0x0000);
  assertEqual(val3.Lo, 0x0000);
  assertEqual(val4.Hi, 0x0000);
  assertEqual(val4.Lo, 0x1234);

  UInt32Set(&val1, 0x0001, 0x2345);
  UInt32Set(&val2, 0x0000, 0x0015);
  UInt32Mod(&val3, &val4, &val1, &val2);
  assertEqual(val3.Hi, 0x0000);
  assertEqual(val3.Lo, 0x0dde);
  assertEqual(val4.Hi, 0x0000);
  assertEqual(val4.Lo, 0x000f);

  UInt32Set(&val1, 0x0001, 0x236d);
  UInt32Mod(&val3, &val4, &val1, &val2);
  assertEqual(val3.Hi, 0x0000);
  assertEqual(val3.Lo, 0x0de0);
  assertEqual(val4.Hi, 0x0000);
  assertEqual(val4.Lo, 0x000d);

  UInt32Set(&val1, 0x7654, 0x3210);
  UInt32Set(&val2, 0x0031, 0x2345);
  UInt32Mod(&val3, &val4, &val1, &val2);
  assertEqual(val3.Hi, 0x0000);
  assertEqual(val3.Lo, 0x0268);
  assertEqual(val4.Hi, 0x0017);
  assertEqual(val4.Lo, 0x5408);
}


void testFixedPointToA() {
  FixedPoint val1 = FixedPointInit(0x1234, 0x0000);
  FixedPoint val2 = FixedPointInit(0x0000, 0x1234);
  char b1[15], b2[15];
  FixedPointToA(b1, &val1);
  FixedPointToA(b2, &val2);
  assertEqualString("4660", b1)
  assertEqualString("0.0711", b2)
}


int main() {
  initCoCoSupport();
  width(80);

  // Test FixedPoint routines
  testFunctions(NF(testFixedPointToA),
                NULL, NULL);

  // Test UInt32 routines
  testFunctions(NF(testUInt32Init),
                NF(testUInt32Set),
                NF(testUInt32Add),
                NF(testUInt32Negate),
                NF(testUInt32Sub),
                NF(testUInt32Mul),
                NF(testUInt32Div),
                NF(testUInt32Mod),
                NULL, NULL);
  
  return 0;
}
