#ifndef clox_vm_h
#define clox_vm_h


#include "chunk.h"

// defines our virtual machie which runs our chunk
typedef struct {
    Chunk* chunk; // the actual chunk being evaluated
    uint8_t* ip; // the instruction pointer
} VM;

typedef enum{ 
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void initVM();
void freeVM();

InterpretResult interpret(Chunk* chunk);



#endif