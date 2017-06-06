#ifndef AS_H
#define AS_H

#include "vm.h"

void assemble_from_fd(vm_env *env, int fd);

int write_to_elf(vm_env *env, int fd);

int load_from_elf(vm_env *env, int fd);

#endif /* AS_H */
