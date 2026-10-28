#ifndef AGENDA_H
#define AGENDA_H

#define MAX_CHARS 30
#define PHONE_LENGTH 17

typedef struct {
    int id;
    char nombre[MAX_CHARS];
    char telefono_main[PHONE_LENGTH];
    char telefono_alt[PHONE_LENGTH];
    char direccion[MAX_CHARS];
    char email[MAX_CHARS];
} Contacto;


// agregarContacto
// verListaContactos
// verDetalleContacto
// editarContacto
// eliminarContacto -> por nombre o por id
// buscarPorNombre


#endif