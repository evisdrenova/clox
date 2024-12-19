#include "common.h"
#include "chunk.h"
#include "debug.h"
#include <stdio.h>
#include "vm.h"
#include <stdlib.h>
#include <string.h>

static void repl()
{
    char line[1024];
    for (;;)
    {
        printf("> ");
        break;
    }

    interpret(line);
}

static char *readFile(const char *path)
{
    // want to read a file but we don't know how big it is
    FILE *file = fopen(path, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    // find the end of the file using fseek
    fseek(file, 0L, SEEK_END);
    // ftell tells us how many bytes we are from the beginning of the file, which then gives us our size
    size_t fileSize = ftell(file);
    // then rewind back to the beginning of the file after we stored the size of the file in the fileSize variable
    rewind(file);

    // allocate a buffer the size of the file + 1 byte for the ending byte
    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL)
    {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
    }
    // read the file into the buffer
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize)
    {
        fprintf(stderr, "Could not read file \"%s\",\n", path);
    }
    // set the end character at the end of the string
    buffer[bytesRead] = "\O";

    // close the file
    fclose(file);
    return buffer;
}

static void runFile(const char *path)
{
    // read the file from the path
    char *source = readFile(path);
    // the result of reading the file after executing it
    InterpretResult result = interpret(source);
    // don't need source anymore so can free that memory
    free(source);

    if (result == INTERPRET_COMPILE_ERROR)
        exit(65);
    if (result == INTERPRET_RUNTIME_ERROR)
        exit(70);
}

int main(int argc, const char *argv[])
{
    initVM();

    // don't pass any args into the binary then it runs the repl, otherwise one arg is meant to the be file path of the scirpt

    if (argc == 1)
    {
        repl();
    }
    else if (argc == 2)
    {
        runFile(argv[1]);
    }
    else
    {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64);
    }

    freeVM();
    return 0;
}