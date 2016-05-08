#include <coco.h>
#include "tester.h"

#include "tester.c"


void testAssertionsWhenTrue() {
  assertFalse(0);
  assertTrue(1);
  assertEqual(10, 10);
  assertNotEqual(10, 11);
  assertGreaterThan(12, 11);
  assertLessThan(10, 11);
  assertGreaterThanOrEqualTo(13, 11);
  assertLessThanOrEqualTo(10, 11);
  assertEqualString("hello", "hello");
  assertNotEqualString("hello", "world");
}


void testAssertTrue() {
  assertTrue(1);
  assertTrue(0);
}


void testAssertFalse() {
  assertFalse(0);
  assertFalse(1);
}


void testAssertEqual() {
  assertEqual(12345, 12345);
  assertEqual(12345, 123);
}


void testAssertNotEqual() {
  assertNotEqual(12345, 123);
  assertNotEqual(12345, 12345);
}


void testAssertGreaterThan() {
  assertGreaterThan(1234, 1233);
  assertGreaterThan(1234, 1234);
}


void testAssertLessThan() {
  assertGreaterThan(1233, 1234);
  assertGreaterThan(1234, 1234);
}


void testAssertGreaterThanOrEqualTo() {
  assertGreaterThanOrEqualTo(1234, 1233);
  assertGreaterThanOrEqualTo(1234, 1234);
  assertGreaterThanOrEqualTo(1234, 1235);
}


void testAssertLessThanOrEqualTo() {
  assertLessThanOrEqualTo(1233, 1234);
  assertLessThanOrEqualTo(1234, 1234);
  assertLessThanOrEqualTo(1235, 1234);
}


void testAssertEqualString() {
  assertEqualString("12345", "12345");
  assertEqualString("12345", "123");
}


void testAssertNotEqualString() {
  assertNotEqualString("12345", "123");
  assertNotEqualString("12345", "12345");
}


int main() {
  initCoCoSupport();
  width(80);
  testFunctions(NF(testAssertionsWhenTrue),
                NF(testAssertTrue),
                NF(testAssertFalse),
                NF(testAssertEqual),
                NF(testAssertNotEqual),
                NF(testAssertGreaterThan),
                NF(testAssertLessThan),
                NF(testAssertGreaterThanOrEqualTo),
                NF(testAssertLessThanOrEqualTo),
                NF(testAssertEqualString),
                NF(testAssertNotEqualString),
                NULL, NULL);
  
  return 0;
}
