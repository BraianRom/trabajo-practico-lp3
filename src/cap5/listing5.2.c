#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

/* Reutilizamos el union semun */
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

/* Funciones del listing 5.2 */
int binary_semaphore_allocation(key_t key, int sem_flags) {
    return semget(key, 1, sem_flags);
}

int binary_semaphore_deallocate(int semid) {
    union semun ignored_argument;
    return semctl(semid, 0, IPC_RMID, ignored_argument);
}

/* Main de prueba */
int main() {
    key_t key = 1234;  // clave arbitraria
    int semid;

    /* Crear el semáforo */
    semid = binary_semaphore_allocation(key, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("Error creando semáforo");
        return 1;
    }
    printf("Semáforo creado con ID: %d\n", semid);

    /* Eliminar el semáforo */
    if (binary_semaphore_deallocate(semid) == -1) {
        perror("Error eliminando semáforo");
        return 1;
    }
    printf("Semáforo eliminado correctamente\n");

    return 0;
}

