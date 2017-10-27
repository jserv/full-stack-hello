# full-stack-hello

Implement minimal instruction set and assembler/compiler in order to
allow the execution of classic Hello World program.


## Instruction set

| Opcode  | Description |
| ------------- | ------------- |
| OP_ADD | sum two operands |
| OP_SUB | subtract two operands |
| OP_MUL | multiply two operands |
| OP_DIV | divide two two operands |
| OP_MOD | Modulo, remainder of operand 1 divided by operand 2 |
| OP_PRINT | print integer or string |
| OP_JLT | jump to specified address (in operand 2) if operand 1 is less than 0 |
| OP_JLE | jump to specified address (in operand 2) if operand 1 is less than or equal to 0 |
| OP_JZ | jump to specified address (in operand 2) if operand 1 is equal to 0 |
| OP_JGE | jump to specified address (in operand 2) if operand 1 is greater than or equal to 0 |
| OP_JGT | jump to specified address (in operand 2) if operand 1 is greater than 0 |
| OP_JNZ | jump to specified address (in operand 2) if operand 1 is not equal to 0 |
| OP_JMP | jump to specified address |
| OP_CALL | call to specified address |
| OP_RET | return to where called from |
| OP_HALT | terminate VM |

## Build and Verify

```shell
$ make
cc -Wall -std=gnu99 -g -c -o vm.o -MMD -MF .vm.o.d vm.c
cc -Wall -std=gnu99 -g -c -o as.o -MMD -MF .as.o.d as.c
cc -Wall -std=gnu99 -g -c -o driver.o -MMD -MF .driver.o.d driver.c
cc -Wall -std=gnu99 -g -o as_exec vm.o as.o driver.o
$ make check
42
tests/halt.s pass

Hello World
tests/hello.s pass

42
50
150
tests/test.s pass

$ make test
python tests/run_tests.py
..
----------------------------------------------------------------------
Ran 4 tests in 0.002s

OK
```

## Assembler Usage
Specify -h to obtain the description of the assembler.
```
$ ./as_exec -h
Usage: as_exec [-w] [-x] [-o <out_file>] <in_file>
       -w Assemble <in_file> and write to an ELF file, see -o below
       -o if -w is specifed, <out_file> is used to store the object code
       -x Load <in_file> and execute it

       <in_file> the file name to be used by commands above
```
For example, if the assembly file in concern is tests/coverage.s
1. Assemble an assembly source file and evaluate (execute) it.
   ```
   ./as_exec tests/hello.s
   ```
2. Assemble an assembly source file and write to default ELF file tests/hello.o.
   ```
   ./as_exec -w tests/hello.s
   ```
3. Assemble an assembly source file and write to default ELF file tests/temp.o.
   ```
   ./as_exec -o tests/temp.o -w tests/hello.s
   ```
4. Laod an assembled ELF file tests/hello.o and evaluate (rexcute) it.
   ```
   ./as_exec -x tests/hello.o
   ```
The common ELF tools, such as objdump, can be applied to the output ELF file.
```
objdump -x tests/hello.o
```
Currently, the ELF support is very limited and could be improved when volunteer shows up.

## Licensing
`full-stack-hello` is freely redistributable under the two-clause BSD License.
Use of this source code is governed by a BSD-style license that can be found
in the `LICENSE` file.
