[![Build Status](https://travis-ci.org/jserv/full-stack-hello.svg?branch=master)](https://travis-ci.org/jserv/full-stack-hello)

# full-stack-hello

Implement minimal instruction set and assembler/compiler in order to
allow the execution of classic Hello World program.


## Instruction set

| Opcode  | Description |
| ------------- | ------------- |
| OP_ADD | sum two operands |
| OP_SUB | subtract two operands |
| OP_PRINT | print integer or string |
| OP_JLT | jump to specified address (in operand 2) if operand 1 is less than 0 |
| OP_JLE | jump to specified address (in operand 2) if operand 1 is less than or equal to 0 |
| OP_JZ | jump to specified address (in operand 2) if operand 1 is equal to 0 |
| OP_JGE | jump to specified address (in operand 2) if operand 1 is greater than or equal to 0 |
| OP_JGT | jump to specified address (in operand 2) if operand 1 is greater than 0 |
| OP_JNZ | jump to specified address (in operand 2) if operand 1 is not equal to 0 |
| OP_JMP | jump to specified address |
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

## Licensing
`full-stack-hello` is freely redistributable under the two-clause BSD License.
Use of this source code is governed by a BSD-style license that can be found
in the `LICENSE` file.
