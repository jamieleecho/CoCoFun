#ifndef _tester_h
#define _tester_h

#include <coco.h>
#include <stdarg.h>
#include "setjmp.h"
#include "c_stuff.h"

#include "setjmp.c"


jmp_buf testJumpBuffer;

#define assertTrue(val) { \
  if (!(val)) { \
    attr(3, 0, 0, 0); \
    printf("  %s:%d - %d is not TRUE\n", __FILE__, __LINE__, #val); \
    attr(0, 0, 0, 0); \
    longjmp(&testJumpBuffer, 1); \
  } \
}

#define assertFalse(val) { \
  if (val) { \
    attr(3, 0, 0, 0); \
    printf("  %s:%d - %d is not FALSE\n", __FILE__, __LINE__, #val); \
    attr(0, 0, 0, 0); \
    longjmp(&testJumpBuffer, 1); \
  } \
}

#define assertEqual(val1, val2) { \
  if ((val1) != (val2)) { \
    attr(3, 0, 0, 0); \
    printf("  %s:%d - %d does not equal %d\n", __FILE__, __LINE__, \
           val1, val2); \
    attr(0, 0, 0, 0); \
    longjmp(&testJumpBuffer, 1); \
  } \
}

#define assertNotEqual(val1, val2) { \
  if ((val1) == (val2)) { \
    attr(3, 0, 0, 0); \
    printf("  %s:%d - %d does equal %d\n", __FILE__, __LINE__, \
           val1, val2); \
    attr(0, 0, 0, 0); \
    longjmp(&testJumpBuffer, 1); \
  } \
}

#define assertGreaterThan(val1, val2) { \
  if ((val1) <= (val2)) { \
    attr(3, 0, 0, 0); \
    printf("  %s:%d - %d is not greater than %d\n", __FILE__, __LINE__, \
           val1, val2); \
    attr(0, 0, 0, 0); \
    longjmp(&testJumpBuffer, 1); \
  } \
}


#define assertLessThan(val1, val2) { \
  if ((val1) >= (val2)) { \
    attr(3, 0, 0, 0); \
    printf("  %s:%d - %d is not less than %d\n", __FILE__, __LINE__, \
           val1, val2); \
    attr(0, 0, 0, 0); \
    longjmp(&testJumpBuffer, 1); \
  } \
}


#define assertGreaterThanOrEqualTo(val1, val2) { \
  if ((val1) < (val2)) { \
    attr(3, 0, 0, 0); \
    printf("  %s:%d - %d is not greater than or equal to %d\n", __FILE__, \
           __LINE__, val1, val2); \
    attr(0, 0, 0, 0); \
    longjmp(&testJumpBuffer, 1); \
  } \
}


#define assertLessThanOrEqualTo(val1, val2) { \
  if ((val1) > (val2)) { \
    attr(3, 0, 0, 0); \
    printf("  %s:%d - %d is not less than or equal to %d\n", __FILE__, \
           __LINE__, val1, val2); \
    attr(0, 0, 0, 0); \
    longjmp(&testJumpBuffer, 1); \
  } \
}


#define assertEqualString(val1, val2) { \
  if (strcmp((val1), (val2))) { \
    attr(3, 0, 0, 0); \
    printf("  %s:%d - %s does not equal %s\n", __FILE__, __LINE__, \
           val1, val2); \
    attr(0, 0, 0, 0); \
    longjmp(&testJumpBuffer, 1); \
  } \
}

#define assertNotEqualString(val1, val2) { \
  if (!strcmp((val1), (val2))) { \
    attr(3, 0, 0, 0); \
    printf("  %s:%d - %s equals %s\n", __FILE__, __LINE__, \
	   val1, val2); \
    attr(0, 0, 0, 0); \
    longjmp(&testJumpBuffer, 1); \
  } \
}

#define NF(name) #name, name

/**
 * Executes test functions with the given name, f combinations.
 */
void testFunctions(char *name, void *f, ...);

#endif


