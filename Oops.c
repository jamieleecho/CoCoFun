#include <cmoc.h>
#include <legacy.h>

// This causes CMOC to crash on OS X
// char buffer[] = "HELLO";

typedef struct Bitmap {
  byte bytes[10];
} Bitmap;


char buffer[] = "Hello";

void foo(Bitmap *b) {
  memset(b, 1, sizeof(*b));
}

int main() {
  Bitmap bb;

  // This should be all 1
  foo(&bb);
  for(byte ii=0; ii<10; ii++)
	printf("%d\n", bb.bytes[ii]);
  
  return 0;
}
