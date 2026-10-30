#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#include "agenda.h"
#define NOMBRE_ARCHIVO "agenda.txt"


Contacto *reservarMemoria() {
    Contacto * nuevo = (Contacto*)malloc(sizeof(Contacto));
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

void agregarContacto(Contacto * nuevo){
// un arreglo decae a un puntero a su primer elemento. Ya es una dirección, así que no lleva &.
        printf("Ingrese el nombre del contacto: ");
        scanf(" %29s", nuevo->nombre);
        nuevo->id = nuevo->id + 1;
    // p[i].precio equivale a (p + i)->precio    

    }

void guardarContacto(FILE *archivo, Contacto *nuevo) {
    fprintf(archivo, "\nNombre: %s", nuevo->nombre);
    fprintf(archivo, "\nID: %d", nuevo->id);
    }

int ejecutarOpcion(char* opcion){
    if (strcmp(opcion, "mostrar") == 0) {
        mostrarAgenda();
    } else if (strcmp(opcion, "agregar") == 0) {
       FILE *archivo = escrituraArchivo();
       Contacto *nuevo = reservarMemoria();
    if (nuevo == NULL) {
        printf("No se pudo reservar memoria.\n");
         fclose(archivo);
        return 1;
    }
       agregarContacto(nuevo);
       guardarContacto(archivo, nuevo);
    } else if (strcmp(opcion, "buscar") == 0) {
        printf("Se seleccionó la opción buscar.\n");
    } else if (strcmp(opcion, "editar") == 0) {
        printf("Se seleccionó la opción editar.\n");
    } else if (strcmp(opcion, "eliminar") == 0) {
        printf("Se seleccionó la opción eliminar.\n");
    } else {
        printf("Opción inválida: %s\n", opcion);
        mostrarInstrucciones();
        return 1;
    }
    return 0; 
}