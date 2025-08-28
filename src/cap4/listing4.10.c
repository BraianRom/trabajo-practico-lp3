#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/* Definición de la estructura de un trabajo */
struct job {
    int id;                 /* ID del trabajo (para identificarlo) */
    struct job* next;       /* Puntero al siguiente trabajo en la cola */
};

/* Cola global de trabajos (lista enlazada) */
struct job* job_queue = NULL;

/* Función que procesa un trabajo */
void process_job(struct job* j) {
    printf("Procesando trabajo #%d\n", j->id);
    /* Simular que tarda un poco */
    sleep(1);
}

/* Función que ejecutará el hilo trabajador */
void* thread_function(void* arg) {
    while (job_queue != NULL) {
        /* Obtener el siguiente trabajo */
        struct job* next_job = job_queue;

        /* Avanzar la cola */
        job_queue = job_queue->next;

        /* Procesar el trabajo */
        process_job(next_job);

        /* Liberar memoria */
        free(next_job);
    }
    return NULL;
}

int main() {
    pthread_t worker;

    /* Crear algunos trabajos manualmente */
    for (int i = 5; i > 0; --i) {
        struct job* new_job = malloc(sizeof(struct job));
        new_job->id = i;
        new_job->next = job_queue;  /* Insertar al inicio de la lista */
        job_queue = new_job;
    }

    /* Crear el hilo que procesará los trabajos */
    pthread_create(&worker, NULL, thread_function, NULL);

    /* Esperar a que el hilo termine */
    pthread_join(worker, NULL);

    printf("Todos los trabajos fueron procesados.\n");

    return 0;
}
