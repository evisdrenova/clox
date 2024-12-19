// this file defines our code representation
// chunk refers to a sequence of bytecode

#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "memory.h"
#include "value.h"

// defines our 1 byte opcodes
typedef enum
{
    OP_CONSTANT,
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_RETURN,
} OpCode;

// this struct is a wrapper around a dynamic array of bytes that represent our code
// it dynamically grows as we add and remove bytes from it
typedef struct
{
    int count;            // how many of those allocated entries are actually in use
    int capacity;         // the number of elements in the array we have allocated
    uint8_t *code;        // unsigned 8bit space for 1 byte opcodes
    int *lines;           // stores the line number to return in case of an error, mirrors the bytecode
    ValueArray constants; // stores all of the constants defined in the code

} Chunk;

void initChunk(Chunk *chunk);

void writeChunk(Chunk *chunk, uint8_t byte, int line);

int addConstant(Chunk *chunk, Value value);

void freeChunk(Chunk *chunk);

#endif