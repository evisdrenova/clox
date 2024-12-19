#include "common.h"
#include "vm.h"
#include <stdio.h>
#include "debug.h"
#include "compiler.h"

VM vm;

static void resetStack()
{
    // stack is empty at this point, so this effectively resets the stack
    vm.stackTop = vm.stack;
}

void initVM()
{
    resetStack();
}

void freeVM()
{
}

void push(Value value)
{
    // stack works left to right
    // store the value at the stackTop which is the value just past the most recently used index in the array
    *vm.stackTop = value;
    // then increment the stackTop pointer to the next empty space to the right
    vm.stackTop++;
}

Value pop()
{
    // decerement the stackTop pointer to back one space to the left
    vm.stackTop--;
    // return the value of the updated stackTop pointer
    // we don't need to actually remove the value from the array, simply moving the pointer handles that
    return *vm.stackTop;
}

// run the VM
static InterpretResult run()
{
    // define  a macro that deferences the ip pointer and then increments the pointer
#define READ_BYTE() (*vm.ip++)
// defines a macro that reads in constants from the values array
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

#define BINARY_OP(op)     \
    do                    \
    {                     \
        double b = pop(); \
        double a = pop(); \
        push(a op b);     \
    } while (false)

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
        case OP_NEGATE:
        {
            // pops the value, then negates it with the '-' then pushes it back on
            push(-pop());
            break;
        }
        case OP_ADD:
        {
            BINARY_OP(+);
            break;
        }
        case OP_SUBTRACT:
        {
            BINARY_OP(-);
            break;
        }
        case OP_MULTIPLY:
        {
            BINARY_OP(*);
            break;
        }
        case OP_DIVIDE:
        {
            BINARY_OP(/);
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
#undef BINARY_OP
}

InterpretResult interpret(const char *source)
{
    compile(source);
    return INTERPRET_OK;
}