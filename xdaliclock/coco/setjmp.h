#ifndef SETJMP_H
#define SETJMP_H

#include <coco.h>

struct jmp_buf {
  unsigned pc, s, u;
};

typedef struct jmp_buf jmp_buf;

int setjmp(jmp_buf *env);
void longjmp(jmp_buf *env, int value);

#endif
