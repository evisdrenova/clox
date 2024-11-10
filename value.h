#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef double Value;

// struct is a wrapper around the array of values with a few constants to help manage the memory and size
typedef struct
{
    int capacity;
    int count;
    Value *values;
} ValueArray;

void initValueArray(ValueArray *array);
void writeValueArray(ValueArray *array, Value value);
void freeValueArray(ValueArray *array);
void printValue(Value value);

#endif