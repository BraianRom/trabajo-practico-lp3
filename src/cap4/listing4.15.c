#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_function(void* arg) {
    fprintf(stderr, "child thread pid is %d, pthread_self = %lu\n",
            (int)getpid(), (unsigned long)pthread_self());
    while (1); // spin forever
    return NULL;
}

int main() {
    pthread_t thread;
    fprintf(stderr, "main thread pid is %d, pthread_self = %lu\n",
            (int)getpid(), (unsigned long)pthread_self());

    pthread_create(&thread, NULL, &thread_function, NULL);

    while (1); // spin forever
    return 0;
}
