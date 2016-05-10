#ifndef _c_stuff_h
#define _c_stuff_h

#define long int

#define NULL ((void *)0)

#define copy_struct(dst, src) memcpy((void *)&(dst), (void *)&(src), sizeof(src))

void abort();
void free(void *ptr);
void *calloc(unsigned num, unsigned sz);
void walkHeap();

#endif
