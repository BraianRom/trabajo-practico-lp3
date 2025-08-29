#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

/* Print the contents of the home page for the server's socket. */
void get_home_page(int socket_fd) {
    char buffer[10000];
    ssize_t number_characters_read;

    /* Send the HTTP GET command for the home page */
    sprintf(buffer, "GET /\n");
    write(socket_fd, buffer, strlen(buffer));

    /* Read from the socket. Keep reading until no more data */
    while (1) {
        number_characters_read = read(socket_fd, buffer, sizeof(buffer));
        if (number_characters_read == 0)
            return;

        /* Write the data to standard output */
        fwrite(buffer, sizeof(char), number_characters_read, stdout);
    }
}

int main(int argc, char* const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        return 1;
    }

    int socket_fd;
    struct sockaddr_in name;
    struct hostent* hostinfo;

    /* Create the socket */
    socket_fd = socket(PF_INET, SOCK_STREAM, 0);

    /* Store the server's name in the socket address */
    name.sin_family = AF_INET;

    /* Resolve the hostname */
    hostinfo = gethostbyname(argv[1]);
    if (hostinfo == NULL) {
        fprintf(stderr, "Error: unknown host %s\n", argv[1]);
        return 1;
    } else {
        name.sin_addr = *((struct in_addr *) hostinfo->h_addr);
    }

    /* Web servers use port 80 */
    name.sin_port = htons(80);

    /* Connect to the Web server */
    if (connect(socket_fd, (struct sockaddr*)&name, sizeof(struct sockaddr_in)) == -1) {
        perror("connect");
        return 1;
    }

    /* Retrieve and print the server's home page */
    get_home_page(socket_fd);

    close(socket_fd);
    return 0;
}
