#include <stdlib.h>
#include <unistd.h>

/* A handle for a temporary file created with write_temp_file. */
typedef int temp_file_handle;

/* Writes LENGTH bytes from BUFFER into a temporary file. */
temp_file_handle write_temp_file(char* buffer, size_t length) {
    char temp_filename[] = "/tmp/temp_file.XXXXXX";
    int fd = mkstemp(temp_filename);

    /* Unlink immediately so the file is removed on close */
    unlink(temp_filename);

    /* Write the number of bytes first */
    write(fd, &length, sizeof(length));

    /* Write the actual data */
    write(fd, buffer, length);

    /* Return the file descriptor as the handle */
    return fd;
}

/* Reads the contents of a temporary file created with write_temp_file */
char* read_temp_file(temp_file_handle temp_file, size_t* length) {
    char* buffer;
    int fd = temp_file;

    /* Rewind to the beginning of the file */
    lseek(fd, 0, SEEK_SET);

    /* Read the size of the data */
    read(fd, length, sizeof(*length));

    /* Allocate buffer and read the data */
    buffer = (char*) malloc(*length);
    read(fd, buffer, *length);

    /* Close the file descriptor */
    close(fd);

    return buffer;
}
