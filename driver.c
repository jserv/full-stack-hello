#include <stdio.h>
#include <stdlib.h>

#include "vm.h"

#define UNIMPL                                       \
    do {                                             \
        printf("Error: operation not implemented!"); \
    } while (0)

static void add_impl(VM_HANDLER_ARGS)
{
    if (VM_T(op1) == INT && VM_T(op1) == VM_T(op2)) {
        VM_INT(result) = VM_INT(op1) + VM_INT(op2);
    } else {
        UNIMPL;
    }
}

static void sub_impl(VM_HANDLER_ARGS)
{
    if (VM_T(op1) == INT && VM_T(op1) == VM_T(op2)) {
        VM_INT(result) = VM_INT(op1) - VM_INT(op2);
    } else {
        UNIMPL;
    }
}

static void print_impl(VM_HANDLER_ARGS)
{
    switch (VM_T(op1)) {
    case INT:
        printf("%d\n", op1->value.vint);
        break;
    case STR:
        printf("%s\n", op1->value.vstr);
        break;
    default:
        break;
    }
}

void assemble_from_file(vm_env *, const char *);

int main(int argc, char **argv)
{
    vm_env *env = vm_new();

    assemble_from_file(env, argv[1]);

    vm_add_inst(env, (vm_inst){.opcode = OP_HALT});
    vm_hook_opcode_impl(env, OP_PRINT, print_impl);
    vm_hook_opcode_impl(env, OP_ADD, add_impl);
    vm_hook_opcode_impl(env, OP_SUB, sub_impl);


    vm_run(env);

    vm_free(env);

    return 0;
}
