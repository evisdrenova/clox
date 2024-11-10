#include <stdlib.h>
#include "memory.h"

// the pointer is a pointer to the first byte of memory
void *reallocate(void *pointer, size_t oldSize, size_t newSize)
{
    if (newSize == 0)
    {
        free(pointer);
        return NULL;
    }

    // when oldSize == 0, realloc is the same as malloc
    void *result = realloc(pointer, newSize);
    if (result == NULL)
        exit(1);
    return result;
}