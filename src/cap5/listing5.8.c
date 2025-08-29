#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
        close(fds[1]);                  // cerrar extremo de escritura
        dup2(fds[0], STDIN_FILENO);     // redirigir stdin al extremo de lectura
        close(fds[0]);                  // cerrar descriptor original
        execlp("sort", "sort", NULL);   // ejecutar "sort"
        perror("execlp");               // en caso de error
        return 1;
    } else {
        // Proceso padre
        FILE* stream;
        close(fds[0]);                  // cerrar extremo de lectura
        stream = fdopen(fds[1], "w");   // convertir extremo de escritura a FILE*

        // Escribir líneas
        fprintf(stream, "This is a test.\n");
        fprintf(stream, "Hello, world.\n");
        fprintf(stream, "My dog has fleas.\n");
        fprintf(stream, "This program is great.\n");
        fprintf(stream, "One fish, two fish.\n");

        fflush(stream);
        fclose(stream);                 // cerrar pipe de escritura

        // Esperar al hijo
        waitpid(pid, NULL, 0);
    }

    return 0;
}
