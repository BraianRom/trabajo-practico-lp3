#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Allocate a temporary buffer.  */
void* allocate_buffer(size_t size) {
    return malloc(size);
}

/* Deallocate a temporary buffer. */
void deallocate_buffer(void* buffer) {
    printf("Liberando buffer en cleanup handler...\n");
    free(buffer);
}

/* Simulación de trabajo de un hilo */
void do_some_work() {
    /* Allocate a temporary buffer. */
    void* temp_buffer = allocate_buffer(1024);

    /* Register a cleanup handler for this buffer, to deallocate it in
       case the thread exits or is cancelled.  */
    pthread_cleanup_push(deallocate_buffer, temp_buffer);

    /* Simulamos trabajo que tarda un poco */
    for (int i = 0; i < 5; i++) {
        printf("Hilo trabajando... (%d)\n", i + 1);
        sleep(1);
    }

    /* Unregister the cleanup handler.
       Because we pass 1, this actually performs the cleanup. */
    pthread_cleanup_pop(1);
}

/* Función del hilo */
void* thread_function(void* arg) {
    do_some_work();
    return NULL;
}

/* Programa principal */
int main() {
    pthread_t t;
    pthread_create(&t, NULL, thread_function, NULL);

    /* Esperamos un poco y luego cancelamos el hilo */
    sleep(2);
    printf("Cancelando el hilo...\n");
    pthread_cancel(t);

    /* Esperamos que el hilo termine */
    pthread_join(t, NULL);

    printf("Thread terminado y recursos liberados correctamente.\n");
    return 0;
}
