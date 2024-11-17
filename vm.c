#include "common.h"
#include "vm.h"
#include <stdio.h>
#include "debug.h"

VM vm;

void initVM()
{
    resetStack();
}

void freeVM()
{
}

void push(Value value)
{
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop()
{
    vm.stackTop--;
    return *vm.stackTop;
}

// run the VM
static InterpretResult run()
{
    // define  a macro that deferences the ip pointer and then increments the pointer
#define READ_BYTE() (*vm.ip++)
// defines a macro that reads in constants from the values array
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    for (;;) // creates an infinite loop that will continue to run until terminated
    {
        // checks if the DTE flag is set and if code, it runs the function
#ifdef DEBUG_TRACE_EXECUTION
        printf("      ");
        for (Value *slot = vm.stack; slot < vm.stackTop; slot++)
        {
            printf("[");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE())
        {
        case OP_CONSTANT:
        {
            Value constant = READ_CONSTANT();
            push(constant);
            break;
        }
        case OP_RETURN:
        {
            printValue(pop());
            printf("\n");
            return INTERPRET_OK;
        }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(Chunk *chunk)
{
    vm.chunk = chunk;
    // initialize the ip by pointing it to the first chunk about to be executed since it hasn't been executed at first
    vm.ip = vm.chunk->code;
    return run();
}

static void resetStack()
{
    vm.stackTop = vm.stack;
}