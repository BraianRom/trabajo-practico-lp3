#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Estructura de un trabajo en la cola */
struct job {
    struct job* next;
    int id;  // identificador del trabajo
};

/* Cola de trabajos compartida */
struct job* job_queue = NULL;
/* Mutex que protege la cola */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Función que procesa cada trabajo */
void process_job(struct job* j) {
    printf("Procesando trabajo %d en hilo %lu\n", j->id, pthread_self());
    sleep(1); // simula trabajo pesado
}

/* Función que ejecutan los hilos */
void* thread_function(void* arg) {
    while (1) {
        struct job* next_job;

        /* Bloquear acceso a la cola */
        pthread_mutex_lock(&job_queue_mutex);
        if (job_queue == NULL)
            next_job = NULL;
        else {
            next_job = job_queue;
            job_queue = job_queue->next;
        }
        pthread_mutex_unlock(&job_queue_mutex);

        /* Si no hay más trabajos, salir */
        if (next_job == NULL)
            break;

        /* Procesar trabajo */
        process_job(next_job);
        free(next_job);
    }
    return NULL;
}

int main() {
    pthread_t threads[3];

    /* Crear una cola de 5 trabajos */
    for (int i = 0; i < 5; i++) {
        struct job* new_job = malloc(sizeof(struct job));
        new_job->id = i + 1;
        new_job->next = job_queue;
        job_queue = new_job;
    }

    /* Lanzar 3 hilos */
    for (int i = 0; i < 3; i++)
        pthread_create(&threads[i], NULL, thread_function, NULL);

    /* Esperar a que terminen */
    for (int i = 0; i < 3; i++)
        pthread_join(threads[i], NULL);

    printf("Todos los trabajos han sido procesados.\n");
    return 0;
}
