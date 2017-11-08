#include <stdio.h>

#include "opcode.h"
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
    if (VM_T(op1) == INT && VM_T(op1) == VM_T(op2) && VM_INT(op2) != 0) {
        VM_INT(result) = VM_INT(op1) / VM_INT(op2);
    } else {
        UNIMPL;
    }
}

static void mod_impl(VM_HANDLER_ARGS)
{
    if (VM_T(op1) == INT && VM_T(op1) == VM_T(op2) && VM_INT(op2) != 0) {
        VM_INT(result) = VM_INT(op1) % VM_INT(op2);
    } else {
        UNIMPL;
    }
}

static void and_impl(VM_HANDLER_ARGS)
{
    if (VM_T(op1) == INT && VM_T(op2) == INT) {
        VM_INT(result) = VM_INT(op1) & VM_INT(op2);
    } else {
        UNIMPL;
    }
}

static void or_impl(VM_HANDLER_ARGS)
{
    if (VM_T(op1) == INT && VM_T(op2) == INT) {
        VM_INT(result) = VM_INT(op1) | VM_INT(op2);
    } else {
        UNIMPL;
    }
}

static void not_impl(VM_HANDLER_ARGS)
{
    if (VM_T(op1) == INT) {
        VM_INT(op2) = ~VM_INT(op1);
    } else {
        UNIMPL;
    }
}

static void xor_impl(VM_HANDLER_ARGS)
{
    if (VM_T(op1) == INT && VM_T(op2) == INT) {
        VM_INT(result) = VM_INT(op1) ^ VM_INT(op2);
    } else {
        UNIMPL;
    }
}

static void lsl_impl(VM_HANDLER_ARGS)
{
    if (VM_T(op1) == INT && VM_T(op2) == INT) {
        VM_INT(result) = VM_UINT(op1) << VM_UINT(op2);
    } else {
        UNIMPL;
    }
}

static void lsr_impl(VM_HANDLER_ARGS)
{
    if (VM_T(op1) == INT && VM_T(op2) == INT) {
        VM_INT(result) = VM_UINT(op1) >> VM_UINT(op2);
    } else {
        UNIMPL;
    }
}

static void asr_impl(VM_HANDLER_ARGS)
{
    if (VM_T(op1) == INT && VM_T(op2) == INT) {
        VM_INT(result) = VM_INT(op1) >> VM_UINT(op2);
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
    vm_hook_opcode_handler(env, OP_AND, and_impl);
    vm_hook_opcode_handler(env, OP_OR, or_impl);
    vm_hook_opcode_handler(env, OP_NOT, not_impl);
    vm_hook_opcode_handler(env, OP_XOR, xor_impl);
    vm_hook_opcode_handler(env, OP_LSL, lsl_impl);
    vm_hook_opcode_handler(env, OP_LSR, lsr_impl);
    vm_hook_opcode_handler(env, OP_ASR, asr_impl);
}
