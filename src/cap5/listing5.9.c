#include <stdio.h>
#include <unistd.h>

int main() {
    FILE* stream;

    // Abrir el comando "sort" para escribir
    stream = popen("sort", "w");
    if (stream == NULL) {
        perror("popen");
        return 1;
    }

    // Escribir varias líneas
    fprintf(stream, "This is a test.\n");
    fprintf(stream, "Hello, world.\n");
    fprintf(stream, "My dog has fleas.\n");
    fprintf(stream, "This program is great.\n");
    fprintf(stream, "One fish, two fish.\n");

    // Cerrar la tubería y obtener el estado
    int status = pclose(stream);
    if (status == -1) {
        perror("pclose");
        return 1;
    }

    return 0;
}
