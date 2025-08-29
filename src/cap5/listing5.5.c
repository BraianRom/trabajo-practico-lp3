#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

/* Return a uniformly random number in the range [low, high]. */
int random_range(unsigned const low, unsigned const high) {
    unsigned const range = high - low + 1;
    return low + (int) (((double) range) * rand() / (RAND_MAX + 1.0));
}

int main(int argc, char* const argv[]) {
    int fd;
    void* file_memory;

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    /* Seed the random number generator. */
    srand(time(NULL));

    /* Prepare a file large enough to hold an unsigned integer. */
    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    lseek(fd, FILE_LENGTH + 1, SEEK_SET);
    write(fd, "", 1);
    lseek(fd, 0, SEEK_SET);

    /* Create the memory mapping. */
    file_memory = mmap(0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_memory == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    close(fd);

    /* Write a random integer to memory-mapped area. */
    sprintf((char*) file_memory, "%d\n", random_range(-100, 100));

    /* Release the memory (unnecessary because the program exits). */
    munmap(file_memory, FILE_LENGTH);

    return 0;
}
