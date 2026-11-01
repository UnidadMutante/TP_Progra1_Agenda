#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include "agenda.h"

int main (int argc, char *argv[]) {

    if (argc != 2) {
        printf("Error de ejecución.\n"); 
        mostrarInstrucciones();
        return 1;
    }
    Contacto *contactos = NULL;
    size_t cantidad = 0;

    // analizo los contactos que tengo en el archivo
    cantidad = contarContactos();
    if (cantidad > 0) {
        contactos = malloc(cantidad * sizeof(Contacto));
        if (contactos == NULL) {
            printf("No se pudo reservar memoria.\n");
            return 1;
        }
//Analizo si cargarContactos falló para liberar memoria != 0 es igual a fallo
        if (cargarContactos(contactos, cantidad) != 0) {
            free(contactos);
            return 1;
        }
    }

    if (strcmp(argv[1], "mostrar") == 0) {
        mostrarAgenda();
    } else if (strcmp(argv[1], "agregar") == 0) {
        agregarContacto(&contactos, &cantidad);
    } else if (strcmp(argv[1], "buscar") == 0) {
        buscarContacto(contactos, cantidad);
    } else if (strcmp(argv[1], "eliminar") == 0) {
        eliminarContacto(contactos, &cantidad);
    } else {
        printf("Opción inválida: %s\n", argv[1]);
        mostrarInstrucciones();
        return 1;
    }

    free(contactos);
    return 0; 
}
