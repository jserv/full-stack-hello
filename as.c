#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"
#include "opcode.h"

#define NOT_IN_QUOTE '\0'
#define IS_QUOTE(qs, c) ((qs == NOT_IN_QUOTE) ? (c == '"' || c == '\'') : c == qs)
#define SET_QUOTE(qs, c) \
    do {                 \
        qs = c;          \
    } while (0)
#define IS_ESC(c) (c == '\\')

/*
 * A customized version of strsep. It is different in that:
 *
 * 1. Takes care of quoting - when quote characters appear, it will skip the
 *    comparison to characters specified in 'sep'.
 * 2. Original strsep does not skip consecutive characters that all appear in
 *    'sep'. It would return one NULL for every appearing characters. This
 *    variant, instead, skips all of them but return only once. This is the
 *    more desired behavior of an assembly parser.
 *
 * See 'man strsep' for more information on original strsep().
 */
static char *quoted_strsep(char **str, const char *sep)
{
    char *p, *head;
    char quote_sym = NOT_IN_QUOTE;

    if (!str || *str == NULL || **str == '\0' || !sep)
        return NULL;

    p = *str;

    /* skip all prefix characters in 'sep' */
    while (*p != '\0') {
        if (strchr(sep, *p) == NULL)
            break;
        p++;
    }

    if (*p == '\0') {
        *str = p;
        return NULL;
    }

    head = p;

    /* run through all character and consider quotes */
    while (1) {
        if (*p == '\0') {
            *str = p;
            break;
        }

        if (quote_sym == NOT_IN_QUOTE) {
            if (strchr(sep, *p)) {
                *p++ = '\0';
                *str = p;
                break;
            }

            if (IS_QUOTE(quote_sym, *p))
                SET_QUOTE(quote_sym, *p);
        } else {
            if (IS_QUOTE(quote_sym, *p))
                SET_QUOTE(quote_sym, NOT_IN_QUOTE);

            if (IS_ESC(*p)) {
                p++;
                if (*p == '\0') {
                    *str = p;
                    break;
                }
            }
        }
        p++;
    }

    return head;
}

/*
 * Duplicate (and process escaping characters) the string and return it.
 * Return NULL if the opening and closing quotes are unbalanced.
 *
 * See 'man strdup' for more information on original strdup().
 */
static char *quoted_strdup(const char *data)
{
    int len;
    const char *p;
    char *dup;
    char *q;
    char quote_sym = NOT_IN_QUOTE;

    if (IS_QUOTE(quote_sym, data[0])) {
        SET_QUOTE(quote_sym, data[0]);
        len = 0;
        p = &data[1];

        while (*p != '\0' && !IS_QUOTE(quote_sym, *p)) {
            if (*p == '\\')
                p++;
            len++;
            p++;
        }

        if (*p == '\0') /* unbalanced quotes */
            return NULL;

        dup = malloc(len + 1);
        q = dup;
        p = &data[1];

        while (*p != data[0]) {
            if (*p != '\\')
                *q = *p;
            else {
                p++;
                switch (*p) {
                case 't':
                    *q = '\t';
                    break;
                case 'n':
                    *q = '\n';
                    break;
                case 'r':
                    *q = '\r';
                    break;
                case '0':
                    *q = '\0';
                    break;
                default:
                    *q = *p;
                    break;
                }
            }
            p++;
            q++;
        }

        *q = '\0';

        return dup;
    } else {
        return strdup(data);
    }
}

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
    case '"':
    case '\'':
        op.type = CONST;
        op.value.id = vm_add_const(env, STR, quoted_strdup(data));
        break;
    default:
        printf("Error: please specify operand type for '%s' in the following line\n"
               "       %s\n\n"
               "Supported types:\n"
               "       $ (constant integer)\n"
               "       # (temp integer)\n"
               "       ^ (constant string\n", data, line);
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
    char *mnemonic = quoted_strsep(&line, " ");
    char *op1 = quoted_strsep(&line, " ");
    char *op2 = quoted_strsep(&line, " ");
    char *result = quoted_strsep(&line, " ");
    vm_inst new_inst;
    const struct instruction *inst = find_inst(mnemonic);

    if (!inst) {
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
