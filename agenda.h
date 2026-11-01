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

//Funciones llamadas desde el main
// @post: muestra por pantalla las instrucciones de uso del programa.
void mostrarInstrucciones(void);
size_t contarContactos(void);
int cargarContactos(Contacto *contactos, size_t cantidad);

//OPCIONES
//Mostrar
void mostrarContacto(Contacto *contacto);
int mostrarAgenda(void);

//Agregar
// @param contactos: arreglo actual de contactos.
// @param cantidad: cantidad actual de contactos.
// @pre: una estructura de contactos inicializada. Puede estar vacía. 
// @post: devuelve un puntero a la memoria reservada para la cantidad de contactos que le haya pasado por size_t cantidad
Contacto *reservarMemoria(Contacto *contactos, size_t cantidad);
int ingresarDatos(Contacto *nuevo, int id);
int guardarContacto(FILE *archivo, Contacto *nuevo);
int agregarContacto(Contacto **contactos, size_t *cantidad);

//Opcion buscar
int buscarContacto(Contacto *contactos, size_t cantidad);

//Opcion eliminar
int reescribirArchivo(Contacto *contactos, size_t cantidad);
int eliminarContacto(Contacto *contactos, size_t *cantidad);

#endif