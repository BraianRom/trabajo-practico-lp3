#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int thread_flag;
pthread_mutex_t thread_flag_mutex;

void initialize_flag() {
    pthread_mutex_init(&thread_flag_mutex, NULL);
    thread_flag = 0;
}

void do_work() {
    printf("Hilo %lu haciendo trabajo...\n", pthread_self());
    usleep(200000); // 0.2 segundos
}

void* thread_function(void* thread_arg) {
    while (1) {
        int flag_is_set;
        pthread_mutex_lock(&thread_flag_mutex);
        flag_is_set = thread_flag;
        pthread_mutex_unlock(&thread_flag_mutex);

        if (flag_is_set)
            do_work();
    }
    return NULL;
}

void set_thread_flag(int flag_value) {
    pthread_mutex_lock(&thread_flag_mutex);
    thread_flag = flag_value;
    pthread_mutex_unlock(&thread_flag_mutex);
}

int main() {
    pthread_t thread;
    initialize_flag();

    pthread_create(&thread, NULL, thread_function, NULL);

    // Activamos el hilo por 2 segundos
    set_thread_flag(1);
    sleep(2);

    // Desactivamos el hilo
    set_thread_flag(0);
    printf("Hilo detenido.\n");

    // Cancelamos y esperamos al hilo
    pthread_cancel(thread);
    pthread_join(thread, NULL);

    return 0;
}
