#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agenda.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error de ejecución.\n");
        mostrarInstrucciones();
        return 1;
    }

    Contacto *contactos = NULL;
    size_t cantidad = 0;

    // 1. Pasamos los contactos del archivo al arreglo en memoria dinámica
    cantidad = contarContactos();
    if (cantidad > 0) {
        contactos = malloc(cantidad * sizeof(Contacto));
        if (contactos == NULL) {
            printf("No se pudo reservar memoria.\n");
            return 1;
        }
        if (cargarContactos(contactos, cantidad) != 0) {
            free(contactos);
            return 1;
        }
    }

    // 2. Ejecutamos la opción pedida sobre el arreglo
    int resultado;
    if (strcmp(argv[1], "mostrar") == 0) {
        resultado = mostrarContactos(contactos, cantidad);
    } else if (strcmp(argv[1], "agregar") == 0) {
        resultado = agregarContacto(&contactos, &cantidad);
    } else if (strcmp(argv[1], "buscar") == 0) {
        resultado = buscarContacto(contactos, cantidad);
    } else if (strcmp(argv[1], "eliminar") == 0) {
        resultado = eliminarContacto(contactos, &cantidad);
    } else {
        printf("Opción inválida: %s\n", argv[1]);
        mostrarInstrucciones();
        resultado = 1;
    }

    // 3. Liberamos la memoria
    free(contactos);
    return resultado;
}