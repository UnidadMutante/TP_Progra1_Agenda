#include <stdio.h>
#include "agenda.h"

int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error de ejecución.\n"); 
        mostrarInstrucciones();
        return 1;
    };
    ejecutarOpcion(argv[1]);
    return 0;
}