#include <stdio.h>

#include "vm.h"
#include "opcode.h"

extern void assemble_from_file(vm_env *, const char *);

int main(int argc, char **argv)
{
    /* always accept one argument */
    if (argc < 2)
        return -1;

    vm_env *env = vm_new();

    assemble_from_file(env, argv[1]);

    hook_opcodes(env);
    vm_hook_label(env);
    vm_run(env);

    vm_free(env);

    return 0;
}
