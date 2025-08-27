/* src/cap4/listing4.7.c  - Thread-specific data example (mejorado) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <inttypes.h> /* para uintptr_t cast seguro */

static pthread_key_t thread_log_key;

void close_thread_log(void* thread_log) {
    if (thread_log != NULL) {
        fclose((FILE*)thread_log);
    }
}

void write_to_thread_log(const char* message) {
    FILE* thread_log = (FILE*) pthread_getspecific(thread_log_key);
    if (thread_log) {
        fprintf(thread_log, "%s\n", message);
        fflush(thread_log);
    } else {
        /* Fallback: si no hay log, escribir a stderr */
        fprintf(stderr, "No thread log: %s\n", message);
    }
}

void* thread_function(void* args) {
    char thread_log_filename[64];
    FILE* thread_log;

    /* Usar uintptr_t para imprimir pthread_t de forma segura */
    uintptr_t tid = (uintptr_t) pthread_self();
    snprintf(thread_log_filename, sizeof(thread_log_filename), "thread%" PRIuPTR ".log", tid);

    thread_log = fopen(thread_log_filename, "w");
    if (!thread_log) {
        perror("fopen");
        /* No seteamos especifico; write_to_thread_log usará stderr como fallback */
        pthread_setspecific(thread_log_key, NULL);
    } else {
        pthread_setspecific(thread_log_key, thread_log);
    }

    write_to_thread_log("Thread starting.");
    /* Simular trabajo */
    for (int i = 0; i < 3; ++i) {
        write_to_thread_log("Doing work...");
    }
    write_to_thread_log("Thread finishing.");

    /* Cuando el hilo termine, el destructor close_thread_log se ejecutará y cerrará el FILE* */
    return NULL;
}

int main(void) {
    const int N = 5;
    pthread_t threads[N];

    if (pthread_key_create(&thread_log_key, close_thread_log) != 0) {
        fprintf(stderr, "Error creating thread key\n");
        return 1;
    }

    for (int i = 0; i < N; ++i) {
        if (pthread_create(&threads[i], NULL, thread_function, NULL) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
        }
    }

    for (int i = 0; i < N; ++i) {
        pthread_join(threads[i], NULL);
    }

    /* Opcional: eliminar la clave (las destructors ya fueron llamados a la salida de cada hilo) */
    pthread_key_delete(thread_log_key);

    return 0;
}
