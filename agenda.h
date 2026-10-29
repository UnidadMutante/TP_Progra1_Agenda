#ifndef __AGENDA_H__
#define __AGENDA_H__
#include <stddef.h>
#define MAX_CHARS 30
#define PHONE_LENGTH 17

/*
1. **Estructuras de datos:**
   - Define una estructura para representar los contactos en la agenda. 
   Puedes incluir campos como nombre, número de teléfono, dirección, correo electrónico, etc.
*/
typedef struct {
    int id;
    char nombre[MAX_CHARS];
    char telefono_main[PHONE_LENGTH];
    char telefono_alt[PHONE_LENGTH];
    char direccion[MAX_CHARS];
    char email[MAX_CHARS];
} Contacto;

Contacto *reservarMemoria();
void mostrarInstrucciones(void);
int ejecutarOpcion(char *opcion);
void agregarContacto(Contacto *nuevo);
void guardarContacto(FILE *archivo, Contacto *nuevo);

// verListaContactos
// verDetalleContacto
// editarContacto
// eliminarContacto -> por nombre o por id
// buscarPorNombre


#endif