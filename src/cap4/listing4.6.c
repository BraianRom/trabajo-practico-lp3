#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ACCOUNTS 2

/* Array de balances en cuentas */
float* account_balances;

/* Transferir DOLLARS de FROM_ACCT a TO_ACCT.
   Devuelve 0 si la transacción fue exitosa,
   1 si el balance en FROM_ACCT es muy pequeño. */
int process_transaction(int from_acct, int to_acct, float dollars) {
    int old_cancel_state;

    /* Verificar si la cuenta tiene suficiente dinero */
    if (account_balances[from_acct] < dollars)
        return 1;

    /* --- Inicio sección crítica --- */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);

    /* Mover el dinero */
    account_balances[to_acct] += dollars;
    account_balances[from_acct] -= dollars;

    /* --- Fin sección crítica --- */
    pthread_setcancelstate(old_cancel_state, NULL);

    return 0;
}

int main() {
    /* Inicializar cuentas */
    account_balances = malloc(NUM_ACCOUNTS * sizeof(float));
    account_balances[0] = 1000.0;  // cuenta 0 con $1000
    account_balances[1] = 500.0;   // cuenta 1 con $500

    printf("Balances iniciales:\n");
    printf("Cuenta 0: %.2f\n", account_balances[0]);
    printf("Cuenta 1: %.2f\n", account_balances[1]);

    /* Intentamos transferir $200 de cuenta 0 a cuenta 1 */
    if (process_transaction(0, 1, 200.0) == 0) {
        printf("\nTransferencia exitosa de $200 de cuenta 0 a cuenta 1.\n");
    } else {
        printf("\nLa transferencia falló por fondos insuficientes.\n");
    }

    printf("\nBalances finales:\n");
    printf("Cuenta 0: %.2f\n", account_balances[0]);
    printf("Cuenta 1: %.2f\n", account_balances[1]);

    free(account_balances);
    return 0;
}
