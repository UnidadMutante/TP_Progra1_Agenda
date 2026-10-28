#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#include "agenda.h"
#define NOMBRE_ARCHIVO "agenda.txt"

Agenda* inicializarAgenda() {
    Agenda * nuevo = malloc(sizeof(Contacto));
    if (nuevo==NULL) {
        return NULL;
    }
    return nuevo;
}

void mostrarInstrucciones(void)
{
    printf("Instrucciones:\n");
    printf("./agenda mostrar  -> ver la lista de contactos\n");
    printf("./agenda agregar  -> agregar un contacto\n");
    printf("./agenda buscar   -> buscar y visualizar un contacto\n");
    printf("./agenda editar   -> editar un contacto\n");
    printf("./agenda eliminar -> eliminar un contacto\n");
}

static FILE *lecturaArchivo(void) {
    return fopen(NOMBRE_ARCHIVO, "r");
}

static FILE *escrituraArchivo(void) {
    return fopen(NOMBRE_ARCHIVO, "a");
}

static int mostrarAgenda(void) {
    FILE *archivo = lecturaArchivo();
    if (archivo == NULL) {
        printf("Error al abrir el archivo para lectura.\n");
        return 1;
    }

    int caracter;
    printf("Contactos:\n");
    while ((caracter = fgetc(archivo)) != EOF) {
        putchar(caracter);
    }
    fclose(archivo);

    return 0;
}

static int agregarContacto(){

    FILE *archivo = escrituraArchivo();
  
    printf("Ingrese el nombre del contacto: ");
    scanf(" %29s", c.nombre);

}

int ejecutarOpcion(char* opcion){
    if (strcmp(opcion, "mostrar") == 0) {
        mostrarAgenda();
    } else if (strcmp(opcion, "agregar") == 0) {
       agregarContacto();
    } else if (strcmp(opcion, "buscar") == 0) {
        printf("Se seleccionó la opción buscar.\n");
    } else if (strcmp(opcion, "editar") == 0) {
        printf("Se seleccionó la opción editar.\n");
    } else if (strcmp(opcion, "eliminar") == 0) {
        printf("Se seleccionó la opción eliminar.\n");
    } else {
        printf("Opción inválida: %s\n", opcion);
        return 1;
    }
    return 0; 
}