#include <pthread.h>
#include <stdio.h>
#include <unistd.h>  // Para sleep

// Función que ejecutará el hilo detached
void* thread_function(void* thread_arg) {
    for (int i = 0; i < 5; ++i) {
        printf("Hilo detached ejecutando: iteración %d\n", i+1);
        sleep(1); // simulamos trabajo
    }
    printf("Hilo detached finalizado\n");
    return NULL;
}

int main() {
    pthread_attr_t attr;
    pthread_t thread;

    // Inicializamos atributos y configuramos como detached
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // Creamos el hilo
    pthread_create(&thread, &attr, &thread_function, NULL);

    // Destruimos atributos
    pthread_attr_destroy(&attr);

    // Hilo principal continúa haciendo su trabajo
    for (int i = 0; i < 3; ++i) {
        printf("Hilo principal ejecutando: iteración %d\n", i+1);
        sleep(1);
    }

    printf("Hilo principal finalizado\n");
    // No hacemos pthread_join porque el hilo es detached
    return 0;
}
