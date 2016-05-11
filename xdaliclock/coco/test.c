#include <coco.h>
#include "c_stuff.h"
#include "tester.h"
#include "UInt32.h"

#include "c_stuff.c"
#include "tester.c"
#include "UInt32.c"


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


int main() {
  initCoCoSupport();
  width(80);

  // Test UInt32 routines
  testFunctions(NF(testUInt32Init),
                NF(testUInt32Set),
                NF(testUInt32Add),
                NF(testUInt32Negate),
                NF(testUInt32Sub),
                NF(testUInt32Mul),
                NULL, NULL);
  
  return 0;
}
