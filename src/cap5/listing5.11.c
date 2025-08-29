#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/* Write TEXT to the socket given by file descriptor SOCKET_FD. */
void write_text(int socket_fd, const char* text) {
    int length = strlen(text) + 1;  // Include NUL-termination
    if (write(socket_fd, &length, sizeof(length)) < 0) {
        perror("write length");
        return;
    }
    if (write(socket_fd, text, length) < 0) {
        perror("write text");
        return;
    }
}

int main(int argc, char* const argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <socket_name> <message>\n", argv[0]);
        return 1;
    }

    const char* const socket_name = argv[1];
    const char* const message = argv[2];
    int socket_fd;
    struct sockaddr_un name;

    // Create the socket
    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("socket");
        return 1;
    }

    // Set up the server address
    name.sun_family = AF_LOCAL;
    strncpy(name.sun_path, socket_name, sizeof(name.sun_path) - 1);
    name.sun_path[sizeof(name.sun_path) - 1] = '\0';

    // Connect to the server
    if (connect(socket_fd, (struct sockaddr*)&name, SUN_LEN(&name)) < 0) {
        perror("connect");
        close(socket_fd);
        return 1;
    }

    // Send the message
    write_text(socket_fd, message);

    close(socket_fd);
    return 0;
}
