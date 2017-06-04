#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "elf.h"

#if __SIZEOF_POINTER__ == __SIZEOF_INT__

struct __attribute__((__packed__)) _elf32_hdr_entry {
    uint8_t e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

typedef struct _elf32_hdr_entry elf_hdr_entry;

static const char elf_hdr_default[] = {
    /* 00 */
    0x7f, 0x45, 0x4c, 0x46, /* magic */
    0x01,                   /* 32-bit */
    0x01,                   /* little-endian */
    0x01,                   /* original version of ELF */
    0x00,                   /* System V */
    /* 08 */
    0x00,                                     /* ABI Version */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* Unused */
    /* 10 */
    0x02, 0x00,             /* Executable */
    0x00, 0x00,             /* No specific instruction set */
    0x01, 0x00, 0x00, 0x00, /* original version of ELF */
    /* 18 */
    0x00, 0x00, 0x00, 0x00, /* entry point */
    0x34, 0x00, 0x00, 0x00, /* start of the program header table */
    /* 20 */
    0x00, 0x00, 0x00, 0x00, /* start of the section header table */
    0x00, 0x00, 0x00, 0x00, /* not used by our VM currently */
    /* 28 */
    0x34, 0x00, /* size of this header */
    0x00, 0x00, /* size of a program header table entry */
    0x00, 0x00, /* number of entries in the program header table */
    0x00, 0x00, /* size of a section header table entry */
    /* 30 */
    0x00, 0x00, /* number of entries in the section header table */
    0x00, 0x00  /* index of the section header table entry that contains the
                   section names */
};

#else

struct __attribute__((__packed__)) _elf64_hdr_entry {
    uint8_t e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

typedef struct _elf64_hdr_entry elf_hdr_entry;

static const char elf_hdr_default[] = {
    /* 00 */
    0x7f, 0x45, 0x4c, 0x46, /* magic */
    0x02,                   /* 64-bit */
    0x01,                   /* little-endian */
    0x01,                   /* original version of ELF */
    0x00,                   /* System V */
    /* 08 */
    0x00,                                     /* ABI Version */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* Unused */
    /* 10 */
    0x02, 0x00,             /* Executable */
    0x00, 0x00,             /* No specific instruction set */
    0x01, 0x00, 0x00, 0x00, /* original version of ELF */
    /* 18 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* 20 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, /* start of the section header table */
    /* 28 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, /* not used by our VM currently */
    /* 30 */
    0x00, 0x00, 0x00, 0x00, /* not used by our VM currently */
    0x34, 0x00,             /* size of this header */
    0x00, 0x00,             /* size of a program header table entry */
    /* 38 */
    0x00, 0x00, /* number of entries in the program header table */
    0x00, 0x00, /* size of a section header table entry */
    0x00, 0x00, /* number of entries in the section header table */
    0x00, 0x00  /* index of the section header table entry that contains the
                   section names */
};

#endif

#if __SIZEOF_POINTER__ == __SIZEOF_INT__

struct __attribute__((__packed__)) _elf32_prog_hdr_entry {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
};

typedef struct _elf32_prog_hdr_entry elf_prog_hdr_entry;

static const elf_prog_hdr_entry elf_prog_hdr_default = {.p_type = 1,
                                                        .p_offset = 0,
                                                        .p_vaddr = 0,
                                                        .p_paddr = 0,
                                                        .p_filesz = 0,
                                                        .p_memsz = 0,
                                                        .p_flags = 0,
                                                        .p_align = 32};

#else

struct __attribute__((__packed__)) _elf64_prog_hdr_entry {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
};

typedef struct _elf64_prog_hdr_entry elf_prog_hdr_entry;

static const elf_prog_hdr_entry elf_prog_hdr_default = {.p_type = 1,
                                                        .p_flags = 0,
                                                        .p_offset = 0,
                                                        .p_vaddr = 0,
                                                        .p_paddr = 0,
                                                        .p_filesz = 0,
                                                        .p_memsz = 0,
                                                        .p_align = 32};

#endif

elf *elf_init(void)
{
    elf *p;

    assert(sizeof(elf_hdr_default) == ELF_HDR_SIZE);

    p = malloc(sizeof(elf));
    memcpy(p, elf_hdr_default, sizeof(elf_hdr_default));

    return p;
}

void elf_new_prog_hdrs(elf *elf_info, unsigned short count)
{
    elf_hdr_entry *file_hdr;

    file_hdr = (elf_hdr_entry *) &elf_info->elf_hdr[0];

    file_hdr->e_phnum = count;
    file_hdr->e_phentsize = sizeof(elf_prog_hdr_entry);

    elf_info->prog_hdrs = malloc(count * sizeof(elf_prog_hdr_entry));

    assert(elf_info->prog_hdrs != NULL);
}

void elf_set_prog_hdr(elf *elf_info,
                      unsigned short idx,
                      unsigned int mem_size,
                      unsigned int file_size)
{
    elf_hdr_entry *file_hdr;
    elf_prog_hdr_entry *p;

    file_hdr = (elf_hdr_entry *) &elf_info->elf_hdr[0];

    assert(idx < file_hdr->e_phnum);

    p = &(((elf_prog_hdr_entry *) (elf_info->prog_hdrs))[idx]);
    memcpy(p, &elf_prog_hdr_default, sizeof(*p));

    if (idx == 0) {
        p->p_offset =
            sizeof(elf_hdr_entry) + file_hdr->e_phnum * file_hdr->e_phentsize;
    } else {
        elf_prog_hdr_entry *last = p - 1;
        p->p_offset = last->p_filesz + last->p_offset;
    }
    p->p_memsz = mem_size;
    p->p_filesz = file_size;
}

int elf_write_file_hdr(int fd, elf *elf_info)
{
    elf_hdr_entry *file_hdr;
    int len;
    int sz;

    len = sz = write(fd, &elf_info->elf_hdr[0], ELF_HDR_SIZE);
    if (sz < 0)
        return sz;

    file_hdr = (elf_hdr_entry *) &elf_info->elf_hdr[0];
    if (file_hdr->e_phnum) {
        sz = write(fd, elf_info->prog_hdrs,
                   file_hdr->e_phnum * file_hdr->e_phentsize);
        if (sz < 0)
            return sz;
        len += sz;
    }

    return len;
}

int elf_write_prog_seg(elf *elf_info, int fd, unsigned short idx, void *mem)
{
    elf_prog_hdr_entry *p;
    int len = 0;

    p = (elf_prog_hdr_entry *) elf_info->prog_hdrs;

    if (p[idx].p_filesz)
        len = write(fd, mem, p[idx].p_filesz);

    return len;
}

elf *elf_read_file_hdr(int fd)
{
    elf *e;

    e = malloc(sizeof(*e));
    if (!e)
        return NULL;

    if (read(fd, &e->elf_hdr[0], sizeof(e->elf_hdr)) < sizeof(e->elf_hdr)) {
        free(e);
        return NULL;
    }

    if (memcmp(&e->elf_hdr[0], &elf_hdr_default, 4)) {
        free(e);
        return NULL;
    }

    return e;
}

int elf_read_prog_hdrs(elf *e, int fd)
{
    elf_hdr_entry *file_hdr;
    int prog_hdrs_sz;

    file_hdr = (elf_hdr_entry *) &e->elf_hdr[0];

    if (file_hdr->e_phentsize != sizeof(elf_prog_hdr_entry))
        return -1;

    if (!file_hdr->e_phnum)
        return 0;

    prog_hdrs_sz = file_hdr->e_phnum * file_hdr->e_phentsize;

    e->prog_hdrs = malloc(prog_hdrs_sz);

    if (read(fd, e->prog_hdrs, prog_hdrs_sz) != prog_hdrs_sz) {
        free(e->prog_hdrs);
        e->prog_hdrs = NULL;
    }

    return file_hdr->e_phnum;
}

int elf_read_prog_seg(elf *e, int fd, int idx, char **mem, size_t *size)
{
    elf_hdr_entry *file_hdr;
    elf_prog_hdr_entry *p;

    file_hdr = (elf_hdr_entry *) &e->elf_hdr[0];

    if (idx >= file_hdr->e_phnum || !e->prog_hdrs)
        return -1;

    p = &((elf_prog_hdr_entry *) e->prog_hdrs)[idx];

    if (!p->p_filesz)
        return 0;

    if (!(*mem = realloc(*mem, p->p_filesz)))
        return -1;

    if (read(fd, *mem, p->p_filesz) != p->p_filesz)
        return -1;

    *size = p->p_filesz;

    return (int)p->p_filesz;
}
