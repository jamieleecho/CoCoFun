#include <cmoc.h>
#include "CoCoMisc.h"
#include "CoCo.h"

#include "CoCoMisc.c"

// This causes CMOC to crash on OS X
// char buffer[] = "HELLO";

typedef struct Bitmap {
  byte bytes[10];
} Bitmap;


char buffer[] = "Hello";

void foo(Bitmap *b) {
  memset(b, 1, sizeof(*b));
}

int sum(int start, int end) {
  int val = 0;
  for(int ii = start; ii <= end; ii++) {
    val = val + ii;
  }
  return val;
}

void erase(byte *start, int n, byte val) {
  for(int ii=0; ii<n; ii++) {
    *start++ = val;
  }
}

void move(byte *dst, byte *src, int n) {
  for(int ii=0; ii<n; ii++) {
    *dst++ = *src++;
  }
}


int main() {
  printf("The sum is %d\n", sum(10, 20));
  erase((byte *)0x400, 512, ' ');
  move((byte *)0x500, (byte *)0x400, 256);

  Bitmap bb;

  // This should be all 1
  foo(&bb);
  for(byte ii=0; ii<10; ii++)
	printf("%d\n", bb.bytes[ii]);

  while(1)
    printf("%d\n", inkey());
  
  return 0;
}
