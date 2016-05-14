#include <coco.h>
#include "c_stuff.h"
#include "tester.h"
#include "UInt32.h"

#include "c_stuff.c"
#include "tester.c"


void testCallocReturnsNewMem() {
  char *mem1 = (char *)calloc(1, 12);
  char *mem2 = (char *)calloc(1, 12);
  assertNotEqual(mem1, (char *)0);
  assertNotEqual(mem2, (char *)0);
  assertNotEqual(mem1, mem2);
  strcpy(mem1, "Hello World");
  strcpy(mem2, "Hello World");
  assertEqualString(mem1, mem2);
  free(mem2);
  free(mem1);
}


void testFreeFreesMem() {
  char *mem1 = (char *)calloc(1, 11);
  char *mem2 = (char *)calloc(1, 11);
  free(mem1);
  free(mem2);
  char *mem3 = (char *)calloc(1, 11);
  char *mem4 = (char *)calloc(1, 11);
  assertEqual(mem1, mem3);
  assertEqual(mem2, mem4);
  free(mem2);
  free(mem1);
  assertGreaterThan((unsigned)mem1, 0x100);
  assertGreaterThan((unsigned)mem2, 0x100);
  assertGreaterThan((unsigned)mem3, 0x100);
  assertGreaterThan((unsigned)mem4, 0x100);
}


void testFreeDefragsMem() {
  char *mem1 = (char *)calloc(1, 12);
  char *mem2 = (char *)calloc(1, 12);
  free(mem1);
  free(mem2);
  char *mem3 = (char *)calloc(1, 22);
  assertEqual(mem1, mem3);
  free(mem3);
  assertGreaterThan((unsigned)mem1, 0x100);
  assertGreaterThan((unsigned)mem2, 0x100);
  assertGreaterThan((unsigned)mem3, 0x100);
}


void testBigMemAlloc() {
  char *mem1 = (char *)calloc(1, 0x4000 - 5);
  assertNotEqual((int)mem1, 0);
  char *mem2 = (char *)calloc(1, 0x4000 - 5);
  assertEqual((int)mem2, 0);
  free(mem1);
  char *mem3 = (char *)calloc(1, 0x4000 - 11);
  assertEqual(mem1, mem3);
  char *mem4 = (char *)calloc(1, 1);
  assertGreaterThan(mem4, mem3);
  free(mem3);
  free(mem4);
}


void testMultiAlloc() {
  char *mem1 = (char *)calloc(10, 3);
  char *mem2 = (char *)calloc(30, 10);
  char *mem3 = (char *)calloc(25, 3);
  assertEqual((int)mem2 - (int)mem1, 35);
  assertEqual((int)mem3 - (int)mem2, 305);
  free(mem1);
  free(mem2);
  free(mem3);
}


void testHolyAlloc() {
  char *mem1 = (char *)calloc(1, 10);
  char *mem2 = (char *)calloc(1, 20);
  char *mem3 = (char *)calloc(1, 10);
  free(mem2);
  char *mem4 = (char *)calloc(1, 5);
  char *mem5 = (char *)calloc(1, 9);
  char *mem6 = (char *)calloc(1, 1);

  assertEqual(mem2, mem4);
  assertEqual(mem4 + 10, mem5);
  assertEqual(mem3 + 15, mem6);

  free(mem1);
  free(mem3);
  free(mem4);
  free(mem5);
  free(mem6);
}


int main() {
  initCoCoSupport();
  width(80);

  // Test memory allocation
  testFunctions(NF(testCallocReturnsNewMem),
                NF(testFreeFreesMem),
                NF(testFreeDefragsMem),
                NF(testBigMemAlloc),
                NF(testMultiAlloc),
                NF(testHolyAlloc),
                NULL, NULL);

  return 0;
}
