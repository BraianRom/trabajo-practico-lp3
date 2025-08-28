#include <pthread.h>
#include <iostream>
#include <unistd.h> // Para sleep()

class ThreadExitException {
public:
    ThreadExitException(void* return_value)
        : thread_return_value_(return_value) {}

    void* DoThreadExit() {
        pthread_exit(thread_return_value_);
        return nullptr; // nunca se ejecuta, pero evita warning
    }

private:
    void* thread_return_value_;
};

// Función ficticia que simula una condición para salir
bool should_exit_thread_immediately() {
    static int counter = 0;
    counter++;
    return (counter > 5); // después de 5 iteraciones, pide salir
}

void do_some_work() {
    while (1) {
        std::cout << "Trabajando en el hilo...\n";
        sleep(1);
        if (should_exit_thread_immediately())
            throw ThreadExitException(nullptr);
    }
}

void* thread_function(void*) {
    try {
        do_some_work();
    }
    catch (ThreadExitException ex) {
        std::cout << "Excepción atrapada, saliendo del hilo.\n";
        ex.DoThreadExit();
    }
    return nullptr;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, nullptr, thread_function, nullptr);

    // Esperar que termine
    pthread_join(thread, nullptr);

    std::cout << "Hilo finalizado correctamente.\n";
    return 0;
}
