#include <pthread.h>
#include <stdio.h>

/* Parámetros para la función de impresión. */
struct char_print_parms {
    char character;  /* El carácter a imprimir */
    int count;       /* La cantidad de veces que debe imprimirse */
};

/* Función que imprime un número de caracteres a stderr. */
void* char_print(void* parameters) {
    struct char_print_parms* p = (struct char_print_parms*) parameters;
    for (int i = 0; i < p->count; ++i) {
        fputc(p->character, stderr);
    }
    return NULL;
}

/* Función principal. */
int main() {
    pthread_t thread1_id;
    pthread_t thread2_id;
    struct char_print_parms thread1_args;
    struct char_print_parms thread2_args;

    /* Crear un nuevo hilo para imprimir 30,000 'x'. */
    thread1_args.character = 'x';
    thread1_args.count = 30000;
    pthread_create(&thread1_id, NULL, &char_print, &thread1_args);

    /* Crear un nuevo hilo para imprimir 20,000 'o'. */
    thread2_args.character = 'o';
    thread2_args.count = 20000;
    pthread_create(&thread2_id, NULL, &char_print, &thread2_args);

    /* Esperar a que el primer hilo finalice. */
    pthread_join(thread1_id, NULL);

    /* Esperar a que el segundo hilo finalice. */
    pthread_join(thread2_id, NULL);

    /* Ahora podemos salir con seguridad. */
    return 0;
}
