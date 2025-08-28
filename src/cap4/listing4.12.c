#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Estructura de un trabajo */
struct job {
    struct job* next;
    int id; // identificador del trabajo
};

/* Cola de trabajos compartida */
struct job* job_queue = NULL;

/* Mutex para proteger la cola */
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Semáforo que indica la cantidad de trabajos disponibles */
sem_t job_queue_count;

/* Procesa un trabajo */
void process_job(struct job* j) {
    printf("Procesando trabajo %d en hilo %lu\n", j->id, pthread_self());
    sleep(1); // simula trabajo
}

/* Inicializa la cola y el semáforo */
void initialize_job_queue() {
    job_queue = NULL;
    sem_init(&job_queue_count, 0, 0); // valor inicial = 0
}

/* Función que ejecutan los hilos */
void* thread_function(void* arg) {
    while (1) {
        struct job* next_job;

        /* Esperar a que haya trabajos disponibles */
        sem_wait(&job_queue_count);

        /* Bloquear mutex para acceder a la cola */
        pthread_mutex_lock(&job_queue_mutex);
        next_job = job_queue;
        job_queue = job_queue->next;
        pthread_mutex_unlock(&job_queue_mutex);

        /* Procesar trabajo */
        process_job(next_job);
        free(next_job);
    }
    return NULL;
}

/* Agrega un trabajo a la cola */
void enqueue_job(int id) {
    struct job* new_job = malloc(sizeof(struct job));
    new_job->id = id;

    pthread_mutex_lock(&job_queue_mutex);
    new_job->next = job_queue;
    job_queue = new_job;
    pthread_mutex_unlock(&job_queue_mutex);

    /* Indicar que hay un trabajo disponible */
    sem_post(&job_queue_count);
}

/* Main de prueba */
int main() {
    pthread_t threads[3];

    /* Inicializar la cola y semáforo */
    initialize_job_queue();

    /* Lanzar 3 hilos */
    for (int i = 0; i < 3; i++)
        pthread_create(&threads[i], NULL, thread_function, NULL);

    /* Agregar 5 trabajos */
    for (int i = 0; i < 5; i++)
        enqueue_job(i + 1);

    /* Esperar a que los hilos terminen (en este caso, se quedan esperando por más trabajos)
       Para propósitos de prueba, dormimos 6 segundos y luego cancelamos los hilos. */
    sleep(6);
    for (int i = 0; i < 3; i++)
        pthread_cancel(threads[i]);

    for (int i = 0; i < 3; i++)
        pthread_join(threads[i], NULL);

    printf("Todos los trabajos han sido procesados.\n");
    return 0;
}
