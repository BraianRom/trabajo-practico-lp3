#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* We must define union semun ourselves. */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

/* 
 * Initialize a binary semaphore with a value of 1.
 * Returns 0 on success, -1 on failure.
 */
int binary_semaphore_initialize(int semid) {
    union semun argument;
    unsigned short values[1];

    values[0] = 1;
    argument.array = values;

    return semctl(semid, 0, SETALL, argument);
}

/* Función que ejecutará el hilo */
void* print_message(void* arg) {
    char* message = (char*) arg;
    printf("Mensaje desde el hilo: %s\n", message);
    pthread_exit(NULL);
}

/* Main de prueba */
int main() {
    pthread_t thread;
    const char* message = "Hola desde un hilo!";
    
    /* Crear el hilo */
    if (pthread_create(&thread, NULL, print_message, (void*) message)) {
        fprintf(stderr, "Error creando el hilo\n");
        return 1;
    }

    /* Esperar a que termine */
    if (pthread_join(thread, NULL)) {
        fprintf(stderr, "Error uniendo el hilo\n");
        return 2;
    }

    printf("Hilo terminado correctamente\n");
    return 0;
}
