#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

sig_atomic_t child_exit_status;

/* Handler para SIGCHLD */
void clean_up_child_process(int signal_number) {
    int status;
    /* Recolecta el proceso hijo terminado */
    wait(&status);
    /* Guarda el estado de salida en variable global */
    child_exit_status = status;
    printf("Child process cleaned up with status %d\n", status);
}

int main() {
    /* Configurar sigaction */
    struct sigaction sigchld_action;
    memset(&sigchld_action, 0, sizeof(sigchld_action));
    sigchld_action.sa_handler = &clean_up_child_process;
    sigaction(SIGCHLD, &sigchld_action, NULL);

    /* Crear un proceso hijo de ejemplo */
    pid_t pid = fork();

    if (pid == 0) {
        /* Proceso hijo */
        printf("Child process exiting...\n");
        _exit(42); // salir con código 42
    } else {
        /* Proceso padre */
        printf("Parent is waiting... (child PID=%d)\n", pid);
        sleep(2); // esperar para que hijo termine
        printf("Parent continues. Child exit status = %d\n", child_exit_status);
    }

    return 0;
}
