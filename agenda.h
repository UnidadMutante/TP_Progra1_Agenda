#ifndef AGENDA_H
#define AGENDA_H
#include <stdio.h>  // FILE
#include <stddef.h> // size_t
#define MAX_CHARS 30
#define PHONE_LENGTH 17

typedef struct {
    int id;
    char nombre[MAX_CHARS];
    char telefono_main[PHONE_LENGTH];
} Contacto;

// Auxiliares
void mostrarInstrucciones(void);
int leerTexto(const char *mensaje, char *destino, size_t tamano);
void mostrarContacto(const Contacto *contacto);

// Archivo
size_t contarContactos(void);
int cargarContactos(Contacto *contactos, size_t cantidad);
int guardarContacto(FILE *archivo, const Contacto *nuevo);
int guardarContactos(const Contacto *contactos, size_t cantidad);

// Opciones (trabajan sobre el arreglo de contactos que vive en main)
int mostrarContactos(const Contacto *contactos, size_t cantidad);
int agregarContacto(Contacto **contactos, size_t *cantidad);
int ingresarDatos(Contacto *nuevo, int id);
int buscarContacto(const Contacto *contactos, size_t cantidad);
int eliminarContacto(Contacto *contactos, size_t *cantidad);

#endif