#ifndef VM_PRIVATE_H
#define VM_PRIVATE_H

#include <stdio.h>

#define _ERROR_PREFIX "Error: "

#define FATAL(code, msg...)                 \
    do {                                    \
        fprintf(stderr, _ERROR_PREFIX msg); \
        return code;                        \
    } while (0)

#define FATALX(code, msg...)                \
    do {                                    \
        fprintf(stderr, _ERROR_PREFIX msg); \
        exit(code);                         \
    } while (0)

#endif
