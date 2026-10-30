#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <errno.h>

#include "agenda.h"

#define NOMBRE_ARCHIVO "agenda.txt"


Contacto *reservarMemoria(void) {
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

// rb y ab para trabajar en binario y no en texto plano. 
static FILE *lecturaArchivo(void) {
    return fopen(NOMBRE_ARCHIVO, "rb");
}

static FILE *escrituraArchivo(void) {
    return fopen(NOMBRE_ARCHIVO, "ab");
}

// El id nuevo es el mayor id que hay en el archivo + 1 (el primero es 1).
// Recorre el archivo contacto por contacto, sin cargarlo entero en memoria.
static int proximoId(void) {
    int mayor = 0;
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        return 1;
    }
    Contacto leido;
    fread(&leido, sizeof(Contacto), 1, archivo);
    while (!feof(archivo)) {
        if (leido.id > mayor) {
            mayor = leido.id;
        }
        fread(&leido, sizeof(Contacto), 1, archivo);
    }
    fclose(archivo);
    return mayor + 1;
}

static void mostrarContacto(const Contacto *contacto) {
    printf("%d - %s - %s\n", contacto->id, contacto->nombre, contacto->telefono_main);
}


//fread(&leido, sizeof(Contacto), 1, archivo) trae un registro completo a la variable leido, ya separado en id, nombre y telefono
static int mostrarAgenda(void) {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        printf("La agenda está vacía.\n");
        return 0;
    }
 
    int hubo = 0;
    Contacto leido;
    fread(&leido, sizeof(Contacto), 1, archivo);
    while (!feof(archivo)) {
        if (!hubo) {
            printf("Contactos:\n");
            hubo = 1;
        }
        mostrarContacto(&leido);
        fread(&leido, sizeof(Contacto), 1, archivo);
    }
    fclose(archivo);
 
    if (!hubo) {
        printf("La agenda está vacía.\n");
    }
    return 0;
}
 
// Lee una línea del teclado con fgets (cadena segura) y le saca el '\n'.
// Si la línea era más larga que el arreglo, descarta lo que sobró para
// que no quede en el teclado y arruine la próxima lectura.
static int leerTexto(const char *mensaje, char *destino, size_t tamano) {
    printf("%s", mensaje);
    if (fgets(destino, tamano, stdin) == NULL) {
        return 1;
    }
    size_t largo = strlen(destino);
    if (largo > 0 && destino[largo - 1] == '\n') {
        destino[largo - 1] = '\0';
    } else {
        int caracter = fgetc(stdin);
        while (caracter != '\n' && caracter != EOF) {
            caracter = fgetc(stdin);
        }
    }
    return 0;
}
 
int ingresarDatos(Contacto *nuevo, int id) {
    nuevo->id = id;
    if (leerTexto("Ingrese el nombre del contacto: ", nuevo->nombre, MAX_CHARS) != 0) {
        return 1;
    }
    if (nuevo->nombre[0] == '\0') {
        printf("El nombre no puede estar vacío.\n");
        return 1;
    }
    if (leerTexto("Ingrese el telefono: ", nuevo->telefono_main, PHONE_LENGTH) != 0) {
        return 1;
    }
    return 0;
}
 
int guardarContacto(FILE *archivo, const Contacto *nuevo) {
    if (fwrite(nuevo, sizeof(Contacto), 1, archivo) != 1) {
        return 1;
    }
    return 0;
}

int agregarContacto(void) {
    Contacto *nuevo = reservarMemoria();
    if (nuevo == NULL) {
        printf("No se pudo reservar memoria.\n");
        return 1;
    }
 
    int resultado = 1;
    int id = proximoId();
    if (ingresarDatos(nuevo, id) == 0) {
        FILE *archivo = fopen(NOMBRE_ARCHIVO, "ab");
        if (archivo == NULL) {
            perror("Error al abrir el archivo para escritura");
        } else {
            resultado = guardarContacto(archivo, nuevo);
            if (fclose(archivo) != 0) {
                resultado = 1;
            }
            if (resultado == 0) {
                printf("Contacto agregado con id %d.\n", id);
            }
        }
    }
    free(nuevo);
    return resultado;
}
 

int buscarContacto(void) {
    char buscado[MAX_CHARS];
    if (leerTexto("Ingrese el nombre (o parte) a buscar: ", buscado, MAX_CHARS) != 0) {
        return 1;
    }
 
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        printf("No se encontraron contactos.\n");
        return 0;
    }
 
    int encontrados = 0;
    Contacto leido;
    fread(&leido, sizeof(Contacto), 1, archivo);
    while (!feof(archivo)) {
        if (strstr(leido.nombre, buscado) != NULL) {
            mostrarContacto(&leido);
            encontrados++;
        }
        fread(&leido, sizeof(Contacto), 1, archivo);
    }
    fclose(archivo);
 
    if (encontrados == 0) {
        printf("No se encontraron contactos.\n");
    }
    return 0;
}

/*
Devuelve un puntero al primer contacto del arreglo. Pero hace falta devolver también cuántos son
y una función solo puede devolver un valor con return. Por eso recibe size_t *cantidad
la dirección de una variable de quien la llama, donde escribe el resultado. Es el mismo mecanismo que el swap
*/
// Carga todos los contactos del archivo en un arreglo dinámico.
// Devuelve el arreglo y deja la cantidad en *cantidad.
// Devuelve NULL (con *cantidad = 0) si la agenda está vacía, no existe o hubo un error.
static Contacto *cargarContactos(size_t *cantidad) {
    *cantidad = 0;
    Contacto *contactos = NULL;
 
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        return NULL;
    }
 
    Contacto leido;
    fread(&leido, sizeof(Contacto), 1, archivo);
    while (!feof(archivo)) {
        Contacto *tmp;
        if (*cantidad == 0) {
            tmp = malloc(sizeof(Contacto));
        } else {
            tmp = realloc(contactos, (*cantidad + 1) * sizeof(Contacto));
        }
        if (tmp == NULL) {
            printf("No se pudo reservar memoria.\n");
            free(contactos);
            *cantidad = 0;
            fclose(archivo);
            return NULL;
        }
        contactos = tmp;
        contactos[*cantidad] = leido;
        (*cantidad)++;
        fread(&leido, sizeof(Contacto), 1, archivo);
    }
    fclose(archivo);
    return contactos;
}

// guardar en el archivo todos los contactos que hay en memoria reemplazando lo que había antes.
static int guardarContactos(const Contacto *contactos, size_t cantidad) {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "wb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo para escritura");
        return 1;
    }
    int resultado = 0;
    for (size_t i = 0; i < cantidad; i++) {
        if (guardarContacto(archivo, &contactos[i]) != 0) {
            resultado = 1;
        }
    }
    if (fclose(archivo) != 0) {
        resultado = 1;
    }
    return resultado;
}

int eliminarContacto(void) {
    char entrada[12];
    if (leerTexto("Ingrese el id del contacto a eliminar: ", entrada, sizeof(entrada)) != 0) {
        return 1;
    }
    int id = atoi(entrada);   // atoi devuelve 0 si no era un número; los ids empiezan en 1
 
    size_t cantidad;
    Contacto *contactos = cargarContactos(&cantidad);
 
    size_t posicion = cantidad;   // "no encontrado"
    for (size_t i = 0; i < cantidad; i++) {
        if (contactos[i].id == id) {
            posicion = i;
        }
    }
    if (posicion == cantidad) {
        printf("No existe un contacto con id %d.\n", id);
        free(contactos);
        return 1;
    }
 
    // corremos un lugar hacia atrás todo lo que estaba después del eliminado
    for (size_t i = posicion; i + 1 < cantidad; i++) {
        contactos[i] = contactos[i + 1];
    }
    cantidad--;
 
    int resultado = guardarContactos(contactos, cantidad);
    if (resultado == 0) {
        printf("Contacto eliminado.\n");
    }
    free(contactos);
    return resultado;
}
 
int ejecutarOpcion(char* opcion){
    if (strcmp(opcion, "mostrar") == 0) {
        mostrarAgenda();
    } else if (strcmp(opcion, "agregar") == 0) {
        agregarContacto();
    } else if (strcmp(opcion, "buscar") == 0) {
        buscarContacto();
    } else if (strcmp(opcion, "eliminar") == 0) {
        eliminarContacto();
    } else {
        printf("Opción inválida: %s\n", opcion);
        mostrarInstrucciones();
        return 1;
    }
    return 0; 
}