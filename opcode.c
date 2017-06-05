#include <stdio.h>

#include "vm.h"
#include "opcode.h"

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

static void mul_impl(VM_HANDLER_ARGS)
{
    if (VM_T(op1) == INT && VM_T(op1) == VM_T(op2)) {
        VM_INT(result) = VM_INT(op1) * VM_INT(op2);
    } else {
        UNIMPL;
    }
}

static void div_impl(VM_HANDLER_ARGS)
{
    if (VM_T(op1) == INT && VM_T(op1) == VM_T(op2)) {
        VM_INT(result) = VM_INT(op1) / VM_INT(op2);
    } else {
        UNIMPL;
    }
}

static void mod_impl(VM_HANDLER_ARGS)
{
    if (VM_T(op1) == INT && VM_T(op1) == VM_T(op2)) {
        VM_INT(result) = VM_INT(op1) % VM_INT(op2);
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

void hook_opcodes(vm_env *env)
{
    vm_hook_opcode_handler(env, OP_PRINT, print_impl);
    vm_hook_opcode_handler(env, OP_ADD, add_impl);
    vm_hook_opcode_handler(env, OP_SUB, sub_impl);
    vm_hook_opcode_handler(env, OP_MUL, mul_impl);
    vm_hook_opcode_handler(env, OP_DIV, div_impl);
    vm_hook_opcode_handler(env, OP_MOD, mod_impl);
}
