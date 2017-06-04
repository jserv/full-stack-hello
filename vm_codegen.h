#ifndef VM_CODEGEN_H
#define VM_CODEGEN_H

#include "vm.h"

vm_seg_info *vm_new_seg_info(char *mem, size_t sz);

vm_seg_info *vm_get_next_seg_info(vm_seg_info *info);

void vm_append_seg_info(vm_seg_info *head, vm_seg_info *info);

size_t vm_get_seg_size(vm_seg_info *info);
void *vm_get_seg_mem(vm_seg_info *info);
unsigned short vm_get_seg_info(vm_env *env, vm_seg_info **seg_info);

void vm_free_seg_info(vm_seg_info *seg_info);

int vm_set_seg_info(vm_env *env, const vm_seg_info *seg_info);

void vm_seg_info_free_list(vm_seg_info *p);

#endif
