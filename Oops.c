#include <cmoc.h>
#include <legacy.h>

typedef struct Bitmap {
  byte x, y;
  byte bytes[10];
} Bitmap;

Bitmap bb;

void foo(Bitmap *b) {
  printf("%d %d %d", sizeof(Bitmap), sizeof(*b), sizeof(b->bytes));
  memset(b, sizeof(*b), 1);
}

int main() {
  foo(&bb);
  return 0;
}
