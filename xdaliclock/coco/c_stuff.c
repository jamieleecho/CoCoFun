#ifndef _c_stuff_c
#include <coco.h>
#include "c_stuff.h"


unsigned _heapBufferSize;
struct HeapBlock {
  struct HeapBlock *prev;
  unsigned size;
  byte free;
};
struct HeapBlock *_heapBlocks;
void *_heapEnd;


byte heapInit() {
  _heapBufferSize = sbrkmax();
  _heapBlocks = (struct HeapBlock *)sbrk(_heapBufferSize);
  _heapBlocks->prev = (struct HeapBlock *)NULL;
  _heapBlocks->size = _heapBufferSize;
  _heapBlocks->free = TRUE;
  _heapEnd = (void *)((unsigned)_heapBlocks + _heapBufferSize - 
                      sizeof(struct HeapBlock));
  return TRUE;
}
byte _heapDidInit = heapInit();


void *calloc(unsigned num, unsigned sz) {
  sz = (num * sz) + sizeof(struct HeapBlock);
  struct HeapBlock *block = _heapBlocks;
  while(block < _heapEnd) {
    if (!block->free || block->size < sz) {
      block = (struct HeapBlock *)((byte *)block + block->size);
      continue;
    }

    if (block->size < sz + 1 + sizeof(struct HeapBlock)) {
       block->free = FALSE;
       return (void *)((byte *)block + sizeof(struct HeapBlock));
    }

    struct HeapBlock *newBlock = (struct HeapBlock *)((byte *)block + sz);
    newBlock->prev = block;
    newBlock->size = block->size - sz;
    newBlock->free = TRUE;
    block->size = sz;
    block->free = FALSE;
    return (void *)((byte *)block + sizeof(struct HeapBlock));
  }

  return NULL;
}


void free(void *ptr) {
  struct HeapBlock *block = 
    (struct HeapBlock*)(((byte *)ptr) - sizeof(struct HeapBlock));
  struct HeapBlock *prev = block->prev;
  struct HeapBlock *next = (struct HeapBlock*)(((byte *)block) + block->size);
  byte nextIsFree = next < _heapEnd && next->free;
  if (!prev || !prev->free) {
    block->free = TRUE;
    block->size += nextIsFree ? next->size : (unsigned)0;
  } else { 
    prev->size += block->size;
    if (nextIsFree) {
      prev->size += next->size;
      next->prev = prev;
    }
  }
}


void walkHeap() {
  struct HeapBlock *block = _heapBlocks;
  while(block < _heapEnd) {
    block = (struct HeapBlock *)((byte *)block + block->size);
  }
}


void abort() {
  exit(1);
}

#endif
