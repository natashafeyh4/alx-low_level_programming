#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include <string.h>

/**
 * print_elf_header - Prints information from the ELF header.
 * @header: Pointer to the ELF header.
 */
void print_elf_header(Elf64_Ehdr *header) {
    printf("ELF Header:\n");
    printf("  Magic:   ");
    for (int i = 0; i < EI_NIDENT; i++) {
        printf("%02x ", header->e_ident[i]);
    }
    printf("\n");
    printf("  Class:                             %s\n",
           header->e_ident[EI_CLASS] == ELFCLASS64 ? "ELF64" :
           header->e_ident[EI_CLASS] == ELFCLASS32 ? "ELF32" : "Invalid class");
    printf("  Data:                              %s\n",
           header->e_ident[EI_DATA] == ELFDATA2LSB ? "2's complement, little endian" :
           header->e_ident[EI_DATA] == ELFDATA2MSB ? "2's complement, big endian" : "Invalid data encoding");
    printf("  Version:                           %d (current)\n", header->e_ident[EI_VERSION]);
    printf("  OS/ABI:                            ");
    switch (header->e_ident[EI_OSABI]) {
        case ELFOSABI_SYSV:
            printf("UNIX - System V\n");
            break;
        case ELFOSABI_HPUX:
            printf("UNIX - HP-UX\n");
            break;
        case ELFOSABI_NETBSD:
            printf("UNIX - NetBSD\n");
            break;
        case ELFOSABI_LINUX:
            printf("UNIX - Linux\n");
            break;
        case ELFOSABI_SOLARIS:
            printf("UNIX - Solaris\n");
            break;
        case ELFOSABI_AIX:
            printf("UNIX - AIX\n");
            break;
        case ELFOSABI_IRIX:
            printf("UNIX - IRIX\n");
            break;
        case ELFOSABI_FREEBSD:
            printf("UNIX - FreeBSD\n");
            break;
        case ELFOSABI_TRU64:
            printf("UNIX - TRU64\n");
            break;
        case ELFOSABI_MODESTO:
            printf("Novell - Modesto\n");
            break;
        case ELFOSABI_OPENBSD:
            printf("UNIX - OpenBSD\n");
            break;
        case ELFOSABI_ARM_AEABI:
            printf("ARM - EABI\n");
            break;
        case ELFOSABI_ARM:
            printf("ARM - \n");
            break;
        case ELFOSABI_STANDALONE:
            printf("Standalone App\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }
    printf("  ABI Version:                       %d\n", header->e_ident[EI_ABIVERSION]);
    printf("  Type:                              ");
    switch (header->e_type) {
        case ET_NONE:
            printf("No file type\n");
            break;
        case ET_REL:
            printf("Relocatable file\n");
            break;
        case ET_EXEC:
            printf("Executable file\n");
            break;
        case ET_DYN:
            printf("Shared object file\n");
            break;
        case ET_CORE:
            printf("Core file\n");
            break;
        default:
            printf("Unknown\n");
            break;
    }
    printf("  Entry point address:               %lx\n", (unsigned long)header->e_entry);
}

/**
 * main - Entry point
 * @argc: Argument count
 * @argv: Argument vector
 * Return: 0 on success, 98 on failure
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
        return (EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error");
        return (EXIT_FAILURE);
    }

    Elf64_Ehdr elf_header;
    ssize_t read_bytes = read(fd, &elf_header, sizeof(Elf64_Ehdr));
    if (read_bytes == -1) {
        perror("Error");
        close(fd);
        return (EXIT_FAILURE);
    }

    if (read_bytes != sizeof(Elf64_Ehdr) ||
        memcmp(elf_header.e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Error: Not an ELF file\n");
        close(fd);
        return (EXIT_FAILURE);
    }

    print_elf_header(&elf_header);
    close(fd);
    return (EXIT_SUCCESS);
}
