#include <stdio.h>

#include "debug.h"

// Add the static forward declaration at the top
static int simpleInstruction(const char *name, int offset);

void disassembleChunk(Chunk *chunk, const char *name)
{
    printf("== %s ==\n", name);

    // we don't increment the offset in the loop, we let the disassembleInstruction function do it for us
    // we do this because op_codes can have different sizes, so we let the disasssembleInstruction function handle it for us
    for (int offset = 0; offset < chunk->count;)
    {
        offset = disassembleInstruction(chunk, offset);
    }
}

// prints our opCode and then returns the offset + 1
static int simpleInstruction(const char *name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}

int disassembleInstruction(Chunk *chunk, int offset)
{
    // prints byte offset to tell us where in the chunk we are
    printf("%04d ", offset);

    // key into the code array at index offset
    uint8_t instruction = chunk->code[offset];

    switch (instruction)
    {
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}
