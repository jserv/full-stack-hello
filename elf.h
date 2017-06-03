#ifndef ELF_H
#define ELF_H

#if __SIZEOF_POINTER__ == __SIZEOF_INT__
#define ELF_HDR_SIZE (0x34)
#else
#define ELF_HDR_SIZE (0x40)
#endif

typedef struct _elf elf;

struct _elf {
    char elf_hdr[ELF_HDR_SIZE];
    void *prog_hdrs;
    void *sec_hdrs;
};

elf *elf_init(void);

void elf_new_prog_hdrs(elf *elf_info, unsigned short count);

void elf_set_prog_hdr(elf *elf_info,
                      unsigned short index,
                      unsigned int mem_size,
                      unsigned int file_size);

int elf_write_file_hdr(int fd, elf *hdr);

int elf_write_prog_seg(elf *elf_info, int fd, unsigned short index, void *mem);

elf *elf_read_file_hdr(int fd);

int elf_read_prog_hdrs(elf *e, int fd);

int elf_read_prog_seg(elf *e, int fd, int index, char **mem, size_t *size);

#endif /* ELF_H */
