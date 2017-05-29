#ifndef VM_H
#define VM_H

typedef struct {
    enum { INT, STR } type;
    union {
        int vint;
        char *vstr;
    } value;
} vm_value;

#define VM_HANDLER_ARGS vm_value *op1, vm_value *op2, vm_value *result
typedef void (*vm_handler)(VM_HANDLER_ARGS);

typedef struct {
    enum { CONST, TEMP, LABEL } type;
    char *label;
    union {
        int id;
    } value;
} vm_operand;

typedef struct {
    int opcode;
    vm_operand op1;
    vm_operand op2;
    int result;
} vm_inst;

typedef struct {
    int opcode;
    vm_handler handler;
} vm_opcode_impl;

typedef struct {
    enum { SYMBOLIC, NUMERIC } type;
    char *label;
    int next_pc;
} vm_label;

#define VM_T(_op) _op->type
#define VM_INT(_op) _op->value.vint

typedef struct __vm_env vm_env;

vm_env *vm_new();
void vm_free(vm_env *);

size_t vm_add_const(vm_env *, int, void *);
size_t vm_add_inst(vm_env *, vm_inst);
size_t vm_add_label(vm_env *, const int, char *);
int vm_find_label(vm_env *, const char *);
void vm_hook_label(vm_env *);

void vm_hook_opcode_handler(vm_env *, int, vm_handler);

void vm_run(vm_env *env);

#endif /* VM_H */
