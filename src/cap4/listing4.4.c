#include <pthread.h>
#include <stdio.h>

/* 
   Calcula números primos sucesivos (muy ineficientemente). 
   Retorna el N-ésimo número primo, donde N es el valor apuntado por *arg.  
*/
void* compute_prime(void* arg) {
    int candidate = 2;
    int n = *((int*) arg);

    while (1) {
        int factor;
        int is_prime = 1;

        /* Probar primalidad mediante divisiones sucesivas. */
        for (factor = 2; factor < candidate; ++factor) {
            if (candidate % factor == 0) {
                is_prime = 0;
                break;
            }
        }

        /* ¿Es este el número primo que buscamos? */
        if (is_prime) {
            if (--n == 0) {
                /* Retornar el número primo encontrado como valor de retorno del hilo. */
                return (void*)(long) candidate; 
                /* usamos (long) para evitar warnings al castear a void* */
            }
        }

        ++candidate;
    }

    return NULL;
}

int main() {
    pthread_t thread;
    int which_prime = 5000;
    int prime;

    /* Crear un hilo para calcular hasta el primo número 5000. */
    pthread_create(&thread, NULL, &compute_prime, &which_prime);

    /* Aquí podríamos hacer otro trabajo en paralelo... */

    /* Esperar a que el hilo termine y obtener el resultado. */
    pthread_join(thread, (void**) &prime);

    /* Mostrar el resultado. */
    printf("The %dth prime number is %d.\n", which_prime, prime);

    return 0;
}
