#ifndef __AGENDA_H__
#define __AGENDA_H__
#include <stdio.h>  // FILE
#include <stddef.h> // size_t
#define MAX_CHARS 60
#define PHONE_LENGTH 17

typedef struct {
    int id;
    char nombre[MAX_CHARS];
    char telefono_main[PHONE_LENGTH];
} Contacto;

Contacto *reservarMemoria(Contacto *contactos, size_t cantidad);
void mostrarInstrucciones(void);
int leerTexto(const char *mensaje, char *destino, size_t tamano);
void mostrarContacto(const Contacto *contacto);

size_t contarContactos(void);
int cargarContactos(Contacto *contactos, size_t cantidad);
int guardarContacto(FILE *archivo, const Contacto *nuevo);
int guardarContactos(const Contacto *contactos, size_t cantidad);

// Opciones
int mostrarAgenda(void);
int agregarContacto(Contacto **contactos, size_t *cantidad);
int ingresarDatos(Contacto *nuevo, int id);
int buscarContacto(const Contacto *contactos, size_t cantidad);
int eliminarContacto(Contacto *contactos, size_t *cantidad);

#endif