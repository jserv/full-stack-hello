#include <stdlib.h>
#include <string.h>

#include "vm.h"
#include "opcode.h"

#if !defined(__GNUC__)
#error "Only gcc is supported at present"
#endif

#define OPCODE env->insts[pc]
#define OPCODE_IMPL(inst) env->impl[inst.opcode]
#define HANDLER OPCODE.handler

#define OP(name) OP_##name

#define BEGIN_OPCODES                          \
    const static void *labels[] = {OP_LABELS}; \
    goto *labels[OPCODE.opcode]

#define DISPATCH \
    ++pc;        \
    goto *labels[OPCODE.opcode]

#define END_OPCODES

#define VM_GOTO(n) \
    pc = n;        \
    goto *labels[OPCODE.opcode]

#define VM_CALL_HANDLER()                               \
    do {                                                \
        if (OPCODE_IMPL(OPCODE).handler)                \
            OPCODE_IMPL(OPCODE).handler(                \
                vm_get_op_value(env, &OPCODE.op1),      \
                vm_get_op_value(env, &OPCODE.op2),      \
                vm_get_temp_value(env, OPCODE.result)); \
        DISPATCH;                                       \
    } while (0)

/* Constant pool max size */
#define CPOOL_MAX_SIZE 100

/* Instruction max size */
#define INSTS_MAX_SIZE 200

/* Temporary storage max size */
#define TEMPS_MAX_SIZE 150

/* OPCODE impl max size */
#define OPCODE_IMPL_MAX_SIZE 256

typedef struct __vm_env {
    vm_inst insts[INSTS_MAX_SIZE];             /* Program instructions */
    vm_value cpool[CPOOL_MAX_SIZE];            /* Constant pool */
    vm_value temps[TEMPS_MAX_SIZE];            /* Temporary storage */
    vm_opcode_impl impl[OPCODE_IMPL_MAX_SIZE]; /* OPCODE impl */
    int insts_count;
    int cpool_count;
    int temps_count;
} vm_env;

vm_env *vm_new()
{
    vm_env *env = malloc(sizeof(vm_env));
    memset(env, 0, sizeof(vm_env));
    return env;
}

void vm_free(vm_env *env)
{
    free(env);
}

size_t vm_add_const(vm_env *env, int type, void *value)
{
    env->cpool[env->cpool_count] = (vm_value){.type = type};

    switch (type) {
    case INT:
        env->cpool[env->cpool_count].value.vint = *(int *) value;
        break;
    case STR:
        env->cpool[env->cpool_count].value.vstr = (char *) value;
        break;
    }

    return env->cpool_count++;
}

size_t vm_add_inst(vm_env *env, vm_inst inst)
{
    env->insts[env->insts_count] = inst;

    return env->insts_count++;
}

void vm_hook_opcode_handler(vm_env *env, int opcode, vm_handler handler)
{
    env->impl[opcode].opcode = opcode;
    env->impl[opcode].handler = handler;
}

static inline size_t vm_get_temp(vm_env *env)
{
    return env->temps_count++;
}

static inline vm_value *vm_get_temp_value(vm_env *env, int id)
{
    return &env->temps[id];
}

static inline vm_value *vm_get_op_value(vm_env *env, const vm_operand *op)
{
    switch (op->type) {
    case CONST:
        return &env->cpool[op->value.id];
    case TEMP:
        return &env->temps[op->value.id];
    default:
        break;
    }

    return NULL;
}

void vm_run(vm_env *env)
{
    size_t pc = 0;
    BEGIN_OPCODES;

    OP(ADD) : VM_CALL_HANDLER();
    OP(SUB) : VM_CALL_HANDLER();
    OP(PRINT) : VM_CALL_HANDLER();
    OP(JMP) : VM_GOTO(OPCODE.op1.value.id);

    OP(HALT) : goto terminate;

    END_OPCODES;
terminate:
    return;
}
