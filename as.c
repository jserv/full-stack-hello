#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"
#include "opcode.h"

static const struct instruction *find_inst(const char *name)
{
    for (int i = 0; instrs[i].name; ++i)
        if (!strcmp(name, instrs[i].name))
            return &instrs[i];
    return NULL;
}

static inline vm_operand make_operand(vm_env *env, char *line, const char *data)
{
    vm_operand op;

    if (data == NULL || data[0] == ';') {
        printf("Error: missing operand in the following line\n");
        printf("       %s\n", line);
        free(line);
        exit(-1);
    }

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
    case ':':
        /*
         * This will not fill the label's next pc to op.value.id
         * when calling `make_operand`.
         * When `vm_add_inst`, it will add this label reference into env, then
         * call `vm_hook_label` to get the actuall label's next_pc
         */
        op.type = LABEL;
        op.value.id = -1;  /* Label not found */
        op.label = strdup(data + 1);
        break;
    default:
        printf("Error: please specify operand type for '%s' in the following line\n"
               "       %s\n\n"
               "Supported types:\n"
               "       $ (constant integer)\n"
               "       # (temp integer)\n"
               "       ^ (constant string)\n"
               "       : (label)\n", data, line);
        free(line);
        exit(-1);
        break;
    }
    return op;
}

static inline int make_result(vm_env *env, char *line, const char *data)
{
    if (data == NULL || data[0] == ';') {
        printf("Error: missing result in the following line\n"
               "       %s\n", line);
        free(line);
        exit(-1);
    }

    if (data[0] != '#') {
        printf("Error: please specify result for '%s' in the following line\n"
               "       %s\n\n"
               "Supported types:\n"
               "       # (temp integer)\n", data, line);
        free(line);
        exit(-1);
    }

    return atoi(data + 1);
}

static void assemble_line(vm_env *env, char *line)
{
    char *line_backup = strdup(line);
    char *mnemonic = strtok(line, " ");
    char *op1 = strtok(NULL, " ");
    char *op2 = strtok(NULL, " ");
    char *result = strtok(NULL, " ");
    vm_inst new_inst;
    const struct instruction *inst = find_inst(mnemonic);

    if (!inst) {
        /* Check label */
        int mlen = strlen(mnemonic);
        if (mlen > 1 && mnemonic[mlen - 1] == ':') {
            mnemonic[mlen - 1] = '\0';
            vm_add_label(env, isdigit(mnemonic[0]), mnemonic);
            goto cleanup;
        }
        printf("Error: instruction `%s' not found\n", mnemonic);
        exit(1);
    }

    memset(&new_inst, 0, sizeof(vm_inst));

    new_inst.opcode = inst->opcode;

    if (inst->has_op1)
        new_inst.op1 = make_operand(env, line_backup, op1);
    if (inst->has_op2)
        new_inst.op2 = make_operand(env, line_backup, op2);
    if (inst->has_result)
        new_inst.result = make_result(env, line_backup, result);

    vm_add_inst(env, new_inst);

cleanup:
    free(line_backup);
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
        /* Remove trailing newline feed */
        line[strcspn(line, "\r\n")] = 0;
        assemble_line(env, line);
    }
    free(line);

    fclose(fp);
}
