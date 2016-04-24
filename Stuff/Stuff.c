/*
  Stuff.c
  
  This software is for demonstration purposes only. Use at your own risk.
*/

#pragma org 0x4000
#include <cmoc.h>
#include <coco.h>

int main() {
  int val;
  asm {
    clr $e00
    clr $e01
    ldd $e00
    orb #32
    std $e00
    std val
  }

  printf("%d\n", val);

  return 0;
}
