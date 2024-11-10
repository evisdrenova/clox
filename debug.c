#include <stdio.h>

#include "debug.h"
#include "value.h"

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

static int constantInstruction(const char *name, Chunk *chunk, int offset)
{
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");

    // we return 2 since the constant is 2 bytes - an opcode and a constant value
    return offset + +2;
}

int disassembleInstruction(Chunk *chunk, int offset)
{
    // prints byte offset to tell us where in the chunk we are
    printf("%04d ", offset);

    if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1])
    {
        printf("       |  ");
    }
    else
    {
        printf("%4d ", chunk->lines[offset]);
    }

    // key into the code array at index offset
    uint8_t instruction = chunk->code[offset];

    switch (instruction)
    {
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    case OP_CONSTANT:
        return constantInstruction("OP_CONSTANT", chunk, offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}
