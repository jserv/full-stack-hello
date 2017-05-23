#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"

struct instruction {
    const char *name;
    int opcode;
    int has_op1;
    int has_op2;
    int has_result;
};

static const struct instruction instrs[] = {{"add", OP_ADD, 1, 1, 1},
                                            {"sub", OP_SUB, 1, 1, 1},
                                            {"jmp", OP_JMP, 1, 0, 0},
                                            {"print", OP_PRINT, 1, 0, 0},
                                            {NULL, 0}};

static const struct instruction *find_inst(const char *name)
{
    for (int i = 0; instrs[i].name; ++i)
        if (!strcmp(name, instrs[i].name))
            return &instrs[i];
    return NULL;
}

static inline vm_operand make_operand(vm_env *env, const char *data)
{
    vm_operand op;

    switch (data[0]) {
    case '$':
        op.type = CONST;
        int temp = atoi(data + 1);
        op.value.id = vm_add_const(env, INT, &temp);
        break;
    case '#':
        op.type = TEMP;
        op.value.id = atoi(data + 1);
        break;
    case '^':
        op.type = CONST;
        op.value.id = vm_add_const(env, STR, strdup(data + 1));
        break;
    }
    return op;
}

static void assemble_line(vm_env *env, char *line)
{
    char *mnemonic = strtok(line, " ");
    char *op1 = strtok(NULL, " ");
    char *op2 = strtok(NULL, " ");
    char *result = strtok(NULL, " ");
    vm_inst new_inst;
    const struct instruction *inst = find_inst(mnemonic);

    if (!inst) {
        printf("Error: instruction `%s' not found\n", mnemonic);
        exit(1);
    }

    memset(&new_inst, 0, sizeof(vm_inst));

    new_inst.opcode = inst->opcode;

    if (inst->has_op1)
        new_inst.op1 = make_operand(env, op1);
    if (inst->has_op2)
        new_inst.op2 = make_operand(env, op2);
    if (inst->has_result)
        new_inst.result = atoi(result + 1);

    vm_add_inst(env, new_inst);
}

void assemble_from_file(vm_env *env, const char *filename)
{
    char *line = NULL;
    size_t size = 0;
    FILE *fp = fopen(filename, "r");

    if (!fp) {
        printf("Error: %m\n");
        exit(1);
    }

    while (getline(&line, &size, fp) != -1) {
        if (line[0] == ';' || line[0] == '\n')
            continue;
        assemble_line(env, line);
    }
    free(line);

    fclose(fp);
}
