#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Reads LENGTH bytes from FILENAME, returning a newly allocated buffer.
   Frees resources on error. */
char* read_from_file(const char* filename, size_t length) {
    char* buffer;
    int fd;
    ssize_t bytes_read;

    /* Allocate the buffer. */
    buffer = (char*) malloc(length);
    if (buffer == NULL)
        return NULL;

    /* Open the file. */
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        /* open failed. Deallocate buffer before returning. */
        free(buffer);
        return NULL;
    }

    /* Read the data. */
    bytes_read = read(fd, buffer, length);
    if (bytes_read != length) {
        /* read failed. Deallocate buffer and close fd before returning. */
        free(buffer);
        close(fd);
        return NULL;
    }

    /* Everything's fine. Close the file and return the buffer. */
    close(fd);
    return buffer;
}
#ifdef TEST_READFILE
#include <stdio.h>

int main() {
    size_t length = 20;
    char* content = read_from_file("archivo.txt", length);
    if(content) {
        printf("Leído: %.*s\n", (int)length, content);
        free(content);
    } else {
        printf("Error leyendo archivo.\n");
    }
    return 0;
}
#endif
