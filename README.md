# full-stack-hello

Implement minimal instruction set and assembler/compiler in order to
allow the execution of classic Hello World program.


## Instruction set

| Opcode  | Description |
| ------------- | ------------- |
| OP_ADD | sum two operands |
| OP_SUB | subtracts two operands |
| OP_PRINT | print |
| OP_JMP | jumps to specified address |
| OP_HALT | terminate VM |

## Build and Verify

```shell
$ make
$ make check
```

## Licensing
`full-stack-hello` is freely redistributable under the two-clause BSD License.
Use of this source code is governed by a BSD-style license that can be found
in the `LICENSE` file.
