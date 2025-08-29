#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

int main(int argc, char* const argv[]) {
    int fd;
    void* file_memory;
    int integer;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    /* Abrir el archivo */
    fd = open(argv[1], O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    /* Crear el memory mapping */
    file_memory = mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_memory == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    close(fd);

    /* Leer el entero, imprimirlo y duplicarlo */
    sscanf((char*)file_memory, "%d", &integer);
    printf("Valor leído: %d\n", integer);
    sprintf((char*)file_memory, "%d\n", 2 * integer);

    /* Liberar el memory mapping */
    munmap(file_memory, FILE_LENGTH);

    return 0;
}
