## Clox

A basic prototype language I built to play with C a little more and build a new language from scratch. Referenced from [Crafting Interpreters](https://craftinginterpreters.com/chunks-of-bytecode.html).

## Overall structure

1. Source code -> the code we write
2. Scanner -> scans the source code line by line
3. Tokens -> created by the scanner
4. Compiler -> compiles the tokens into bytecode
5. Bytecode chunk -> creates by the compiler
6. VM -> executes the bytecode
