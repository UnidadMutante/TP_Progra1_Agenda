#ifndef __AGENDA_H__
#define __AGENDA_H__
#include <stddef.h>
#include <stdio.h> // FILE
#define MAX_CHARS 30
#define PHONE_LENGTH 17

typedef struct {
    int id;
    char nombre[MAX_CHARS];
    char telefono_main[PHONE_LENGTH];
} Contacto;

Contacto *reservarMemoria(void);
void mostrarInstrucciones(void);
int ejecutarOpcion(char *opcion);      
int agregarContacto(void);
int ingresarDatos(Contacto *nuevo, int id);   
int guardarContacto(FILE *archivo, const Contacto *nuevo);
int buscarContacto(void);                  
int eliminarContacto(void);    

#endif