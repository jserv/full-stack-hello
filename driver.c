#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "as.h"
#include "opcode.h"
#include "vm.h"
#include "private.h"

typedef enum {
    ASSEMBLE_AND_EVAL,      /* default */
    ASSEMBLE_AND_WRITE_ELF, /* -w */
    LOAD_ELF_AND_EVAL       /* -x */
} req_t;

#define help_text                                                           \
    "Usage: as_exec [-w] [-x] [-o <out_file>] <in_file>\n"                  \
    "       -w Assemble <in_file> and write to an ELF file, see -o below\n" \
    "       -o if -w is specifed, <out_file> is used to store the object "  \
    "code\n"                                                                \
    "       -x Load <in_file> and execute it\n"                             \
    "\n"                                                                    \
    "       <in_file> the file name to be used by commands above"

int main(int argc, char **argv)
{
    req_t req = ASSEMBLE_AND_EVAL;
    char *in_file = NULL;
    char *out_file = NULL;
    int ignore_option = 0;
    int out_fd = -1;
    int in_fd = -1;

    for (int i = 1; i < argc; i++) {
        if (ignore_option)
            in_file = argv[i];
        else if (!strcmp(argv[i], "--")) /* support filename begin with '-' */
            ignore_option = 1;
        else if (!strcmp(argv[i], "-h")) {
            printf("%s\n", help_text);
            return 0;
        } else if (!strcmp(argv[i], "-o")) {
            if (i++ == argc - 1)
                FATAL(-1, "Missing output file name, see -h\n");
            out_file = argv[i++];
            req = ASSEMBLE_AND_WRITE_ELF;
        } else if (!strcmp(argv[i], "-w")) {
            if (req == LOAD_ELF_AND_EVAL)
                FATAL(-1, "-w and -x used together, see -h\n");
            req = ASSEMBLE_AND_WRITE_ELF;
        } else if (!strcmp(argv[i], "-x")) {
            if (req == ASSEMBLE_AND_WRITE_ELF)
                FATAL(-1, "-w and -x used together, see -h\n");
            req = LOAD_ELF_AND_EVAL;
        } else if (!strcmp(argv[i], "-")) {
            if (in_file)
                FATAL(-1, "more than one input file, see -h\n");
            in_file = argv[i];
        } else if (argv[i][0] == '-')
            FATAL(-1, "unsupported argument '%s', see -h\n", argv[i]);
        else if (in_file)
            FATAL(-1, "specified more than one input file, see -h\n");
        else
            in_file = argv[i];
    }

    if (!in_file)
        FATAL(-1, "must specify an input file\n");
    else {
        if (!strcmp(in_file, "-"))
            in_fd = fileno(stdin);
        else
            in_fd = open(in_file, O_RDONLY);
    }

    if (req == ASSEMBLE_AND_WRITE_ELF) {
        if (!out_file) {
            int i = strlen(in_file);

            if (!strcmp(in_file, "-"))
                FATAL(-1,
                      "must specify output file when stdin is input\n");

            if (strcmp(&in_file[i - 2], ".s") && strcmp(&in_file[i - 2], ".S"))
                FATAL(-1, "unsupported source file extension: %s\n",
                      in_file);

            out_file = strdup(in_file);
            out_file[i - 1] = 'o';
        }

        if (!strcmp(out_file, "-")) {
            out_fd = fileno(stdout);
        } else {
            out_fd =
                open(out_file, O_TRUNC | O_CREAT | O_RDWR,
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        }

        if (out_fd < 0)
            FATAL(-1, "output file opening error (%s)\n",
                  strerror(errno));
    }

    switch (req) {
    case ASSEMBLE_AND_EVAL: {
        vm_env *env = vm_new();
        assemble_from_fd(env, in_fd);
        hook_opcodes(env);
        vm_run(env);
        vm_free(env);
        break;
    }
    case ASSEMBLE_AND_WRITE_ELF: {
        int len;

        vm_env *env = vm_new();
        assemble_from_fd(env, in_fd);
        len = write_to_elf(env, out_fd);
        vm_free(env);
        if (len < 0)
            FATAL(-1, "write ELF file %s failed (%s)\n", out_file,
                  strerror(errno));
        break;
    }
    case LOAD_ELF_AND_EVAL: {
        vm_env *env = vm_new();
        load_from_elf(env, in_fd);
        hook_opcodes(env);
        vm_run(env);
        vm_free(env);
        break;
    }
    default:
        FATAL(-1, "unknown request: %d\n", req);
        break;
    }

    return 0;
}
