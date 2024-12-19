#ifndef clox_vm_h
#define clox_vm_h

#include "value.h"
#include "chunk.h"

#define STACK_MAX 256

// defines our virtual machine which runs our chunk
typedef struct
{
    Chunk *chunk;           // the actual chunk being evaluated
    uint8_t *ip;            // the instruction pointer
    Value stack[STACK_MAX]; // stack for storing temporary variables while evaluating expressions, LIFO
    Value *stackTop;        // stores a pointer just past the last item in the array/the last item in the stack
} VM;

typedef enum
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void initVM();
void freeVM();

InterpretResult interpret(Chunk *chunk);

void push(Value value);
Value pop();

#endif