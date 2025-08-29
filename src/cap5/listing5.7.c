#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* Write COUNT copies of MESSAGE to STREAM, pausing 1 segundo entre cada uno. */
void writer(const char* message, int count, FILE* stream) {
    for (; count > 0; --count) {
        fprintf(stream, "%s\n", message);
        fflush(stream);  // enviar inmediatamente
        sleep(1);        // pausar un segundo
    }
}

/* Leer cadenas del stream mientras sea posible */
void reader(FILE* stream) {
    char buffer[1024];
    while (!feof(stream) && !ferror(stream) && fgets(buffer, sizeof(buffer), stream) != NULL) {
        fputs(buffer, stdout);
    }
}

int main() {
    int fds[2];
    pid_t pid;

    // Crear la pipe
    if (pipe(fds) == -1) {
        perror("pipe");
        return 1;
    }

    // Crear proceso hijo
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Proceso hijo
        FILE* stream;
        close(fds[1]);                 // cerrar extremo de escritura
        stream = fdopen(fds[0], "r");  // convertir a FILE*
        reader(stream);
        fclose(stream);
    } else {
        // Proceso padre
        FILE* stream;
        close(fds[0]);                 // cerrar extremo de lectura
        stream = fdopen(fds[1], "w");  // convertir a FILE*
        writer("Hello, world.", 5, stream);
        fclose(stream);
    }

    return 0;
}
