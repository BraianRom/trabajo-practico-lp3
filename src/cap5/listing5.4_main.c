#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>

/* Incluimos las funciones del listing 5.4 */
int binary_semaphore_wait(int semid);
int binary_semaphore_post(int semid);

/* Necesitamos union semun para usar semctl */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

int main() {
    key_t key = 1234;  // clave arbitraria para el semáforo
    int semid;

    /* Crear un semáforo binario (1 semáforo) */
    semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    /* Inicializar el semáforo con valor 1 */
    union semun arg;
    unsigned short values[1];
    values[0] = 1;
    arg.array = values;

    if (semctl(semid, 0, SETALL, arg) == -1) {
        perror("semctl SETALL");
        exit(1);
    }

    printf("Semáforo creado e inicializado en 1\n");

    /* Esperar (decrementar) */
    printf("Haciendo WAIT...\n");
    if (binary_semaphore_wait(semid) == -1) {
        perror("binary_semaphore_wait");
        exit(1);
    }
    printf("WAIT exitoso, semáforo ahora en 0\n");

    /* Señalar (incrementar) */
    printf("Haciendo POST...\n");
    if (binary_semaphore_post(semid) == -1) {
        perror("binary_semaphore_post");
        exit(1);
    }
    printf("POST exitoso, semáforo ahora en 1\n");

    /* Eliminar el semáforo */
    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl IPC_RMID");
        exit(1);
    }
    printf("Semáforo eliminado.\n");

    return 0;
}
