#include <stdio.h>
#include <stdlib.h>
#include "listing2.5.c"  // Incluimos directamente las funciones

int main() {
    char data[] = "Hola, este es un test de archivo temporal!";
    size_t length = sizeof(data);

    // Escribimos en un archivo temporal
    temp_file_handle temp = write_temp_file(data, length);

    // Leemos desde el archivo temporal
    size_t read_length;
    char* buffer = read_temp_file(temp, &read_length);

    // Mostramos el contenido leído
    printf("Contenido leído (%zu bytes): %s\n", read_length, buffer);

    // Liberamos memoria
    free(buffer);

    return 0;
}
