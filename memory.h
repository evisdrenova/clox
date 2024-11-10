#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

// macro that defines GROW_CAPACITY which is used to calculate a new capacity based on the current capacity
#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

// macro that defines GROW_ARRAY which is used to grow the array capacity
#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type *)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))

// macro that frees the dynamic array that holds the opcode bytes
#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

// handles all dynamic memory management
// if oldSize == 0 && newSize > 0: allocate new block
// if oldSize > 0 && newSize == 0; free allocation
// if oldSize > 0 && newSize < oldSize; shrink allocation
// if oldSize > 0 && newSize > oldSize; grow allocation
void *reallocate(void *pointer, size_t oldSize, size_t newSize);

#endif