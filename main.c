#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include "agenda.h"


/*
2. **Interfaz de línea de comandos:**
   - Diseña una interfaz de línea de comandos que permita al usuario realizar acciones como agregar contactos, eliminar contactos, ver todos los contactos y buscar contactos por nombre u información específica.

Esta interfaz debe ser desde el programa siempre que sea posible.
```sh
$> ./agenda datos.txt mostrar
```

*/
int main (int argc, char *argv[]) {

    if (argc != 2) {
        printf("Error de ejecución.\n"); 
        mostrarInstrucciones();
        return 1;
    };
    
    ejecutarOpcion(argv[1]);

      // cerrar archivo
    //fclose(archivo);

    // liberar memoria
    //free(contactos);
    
    // evitar dangling pointers
    //archivo = NULL;
    //productos = NULL;
    return 0;
}