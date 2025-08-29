#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/* Read text from the socket and print it out. Continue until the
   socket closes. Return nonzero if the client sent a "quit"
   message, zero otherwise. */
int server(int client_socket) {
    while (1) {
        int length;
        char* text;

        // Read the length of the text message
        if (read(client_socket, &length, sizeof(length)) == 0)
            return 0;  // client closed connection

        // Allocate buffer for the message
        text = (char*) malloc(length);
        if (!text) {
            perror("malloc");
            return 0;
        }

        // Read the text itself
        read(client_socket, text, length);
        printf("%s\n", text);

        // Check for "quit" message
        if (!strcmp(text, "quit")) {
            free(text);
            return 1;
        }

        // Free the buffer
        free(text);
    }
}

int main(int argc, char* const argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <socket_name>\n", argv[0]);
        return 1;
    }

    const char* const socket_name = argv[1];
    int socket_fd;
    struct sockaddr_un name;
    int client_sent_quit_message;

    // Create the socket
    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("socket");
        return 1;
    }

    // Bind the socket
    name.sun_family = AF_LOCAL;
    strncpy(name.sun_path, socket_name, sizeof(name.sun_path) - 1);
    name.sun_path[sizeof(name.sun_path) - 1] = '\0';

    if (bind(socket_fd, (struct sockaddr*)&name, SUN_LEN(&name)) < 0) {
        perror("bind");
        close(socket_fd);
        return 1;
    }

    // Listen for connections
    if (listen(socket_fd, 5) < 0) {
        perror("listen");
        close(socket_fd);
        unlink(socket_name);
        return 1;
    }

    // Accept and handle clients until "quit"
    do {
        struct sockaddr_un client_name;
        socklen_t client_name_len = sizeof(client_name);
        int client_socket_fd;

        client_socket_fd = accept(socket_fd, (struct sockaddr*)&client_name, &client_name_len);
        if (client_socket_fd < 0) {
            perror("accept");
            continue;
        }

        client_sent_quit_message = server(client_socket_fd);

        close(client_socket_fd);
    } while (!client_sent_quit_message);

    // Clean up
    close(socket_fd);
    unlink(socket_name);

    return 0;
}
