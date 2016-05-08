#ifndef _TESTER_C
#define _TESTER_C

#include "tester.h"

#include "tester.c"
#include "setjmp.c"


void testFunctions(char *name, void *f, ...) {
  if (!name) {
    return;
  }
  va_list ap; 
  va_start(ap, f);
  while(f) {
    if (!setjmp(&testJumpBuffer)) {
      printf("Testing %s...\n", name);
      (*f)();
    }

    name = va_arg(ap, char *); 
    if (!name) {
      va_end(ap);
      return;
    }
    f = va_arg(ap, void *);
  }
  va_end(ap);
}

#endif
