#include <coco.h>

struct Vect {
  int x;
};

struct BigVect {
  struct Vect x;
};

void test1() {
  int vals[10];
  int ii;
  unsigned char bvals[10];
  for(ii=0; ii<sizeof(vals)/sizeof(vals[0]); ii++) {
    vals[ii] = 0;
    bvals[ii] = 0;
  }  
  vals[5] = 1;

  for(ii=0; ii<sizeof(vals)/sizeof(vals[0]); ii++) {
    if (vals[ii] == 1)
      break;
  }
  bvals[ii] = (unsigned char)ii;
  if (bvals[ii] != ii) {
    printf("test1 failed!\n");
  }
}

void test2() {
  int vals[10];
  for(int ii=0; ii<sizeof(vals)/sizeof(vals[0]); ii++) {
    vals[ii] = (ii == 2 || ii == 5) ? 10 : ii;
  }  

  if (vals[2] != 10 || vals[5] != 10) {
    printf("test2 failed!\n");
  }
}

void test3() {
  byte zero = 0;
  byte shift_amount = 1 + *(&zero);
  byte val = ~((byte)(0xff >> shift_amount) & (byte)(0xff << shift_amount));

  if (val != 0x81) {
    printf("test3 failed!\n");
  }
}

void crash_cmoc() {
  struct BigVect xx;
  if (!xx.x) {
  }
}


int main() {
  test1();
  test2();
  test3();
#if 0
  crash_cmoc();
#endif
  return 0;
}
