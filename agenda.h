#ifndef __AGENDA_H__
#define __AGENDA_H__
#include <stdio.h>  // FILE
#include <stddef.h> // size_t
#define MAX_CHARS 30
#define PHONE_LENGTH 17

typedef struct {
    int id;
    char nombre[MAX_CHARS];
    char telefono[PHONE_LENGTH];
} Contacto;

// @post: muestra por pantalla las instrucciones de uso del programa.
void mostrarInstrucciones(void);

// @param contactos: arreglo actual de contactos.
// @param cantidad: cantidad actual de contactos.
// @pre: una estructura de contactos inicializada. Puede estar vacía. 
// @post: devuelve un puntero a la memoria reservada para la cantidad de contactos que le haya pasado por size_t cantidad
Contacto *reservarMemoria(Contacto *contactos, size_t cantidad);

void mostrarContacto(Contacto *contacto);

size_t contarContactos(void);
int cargarContactos(Contacto *contactos, size_t cantidad);
int guardarContacto(FILE *archivo, Contacto *nuevo);
int guardarContactos(Contacto *contactos, size_t cantidad);

// Opciones
int mostrarAgenda(void);
int agregarContacto(Contacto **contactos, size_t *cantidad);
int ingresarDatos(Contacto *nuevo, int id);
int buscarContacto(Contacto *contactos, size_t cantidad);
int eliminarContacto(Contacto *contactos, size_t *cantidad);

#endif