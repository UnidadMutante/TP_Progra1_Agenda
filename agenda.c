#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agenda.h"

#define NOMBRE_ARCHIVO "agenda.txt"

/* ---------- auxiliares ---------- */

void mostrarInstrucciones(void) {
    printf("Instrucciones:\n");
    printf("./agenda mostrar  -> ver la lista de contactos\n");
    printf("./agenda agregar  -> agregar un contacto\n");
    printf("./agenda buscar   -> buscar y visualizar un contacto\n");
    printf("./agenda eliminar -> eliminar un contacto\n");
}

// Muestra un mensaje y lee una línea del teclado con fgets.
// Le saca el '\n' del final. Devuelve 0 si salió bien y 1 si falló.
int leerTexto(const char *mensaje, char *destino, size_t tamano) {
    printf("%s", mensaje);
    if (fgets(destino, tamano, stdin) == NULL) {
        return 1;
    }
    size_t largo = strlen(destino);
    if (largo > 0 && destino[largo - 1] == '\n') {
        destino[largo - 1] = '\0';
    } else {
        // el texto era más largo que el arreglo: descartamos lo que sobró
        int caracter = fgetc(stdin);
        while (caracter != '\n' && caracter != EOF) {
            caracter = fgetc(stdin);
        }
    }
    return 0;
}

void mostrarContacto(const Contacto *contacto) {
    printf("%d - %s - %s\n", contacto->id, contacto->nombre, contacto->telefono_main);
}

/* ---------- archivo (binario: se guarda el struct completo) ---------- */

// Cuenta cuántos contactos hay en el archivo (0 si no existe).
size_t contarContactos(void) {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        return 0;
    }
    size_t cantidad = 0;
    Contacto leido;
    fread(&leido, sizeof(Contacto), 1, archivo);
    while (!feof(archivo)) {
        cantidad++;
        fread(&leido, sizeof(Contacto), 1, archivo);
    }
    fclose(archivo);
    return cantidad;
}

// Llena el arreglo "contactos" con los contactos del archivo.
int cargarContactos(Contacto *contactos, size_t cantidad) {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        perror("Error al abrir el archivo para lectura");
        return 1;
    }
    for (size_t i = 0; i < cantidad; i++) {
        fread(&contactos[i], sizeof(Contacto), 1, archivo);
    }
    fclose(archivo);
    return 0;
}

// Escribe un contacto completo en el archivo.
int guardarContacto(FILE *archivo, const Contacto *nuevo) {
    if (fwrite(nuevo, sizeof(Contacto), 1, archivo) != 1) {
        return 1;
    }
    return 0;
}

// Reescribe el archivo completo con lo que hay en el arreglo ("wb" lo pisa).
int guardarContactos(const Contacto *contactos, size_t cantidad) {
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

/* ---------- opciones ---------- */

int mostrarContactos(const Contacto *contactos, size_t cantidad) {
    if (cantidad == 0) {
        printf("La agenda está vacía.\n");
        return 0;
    }
    printf("Contactos:\n");
    for (size_t i = 0; i < cantidad; i++) {
        mostrarContacto(&contactos[i]);
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

// Agranda el arreglo en un lugar, completa el contacto nuevo con un id mayor
// al del anterior y lo agrega al final del archivo.
// Recibe &contactos (puntero al puntero) porque realloc puede mover el bloque
// a otra dirección y main tiene que enterarse.
int agregarContacto(Contacto **contactos, size_t *cantidad) {
    Contacto *tmp;
    if (*cantidad == 0) {
        tmp = malloc(sizeof(Contacto));
    } else {
        tmp = realloc(*contactos, (*cantidad + 1) * sizeof(Contacto));
    }
    if (tmp == NULL) {
        printf("No se pudo reservar memoria.\n");
        return 1;
    }
    *contactos = tmp;

    Contacto vacio = {0};
    tmp[*cantidad] = vacio;                 // el lugar nuevo empieza en cero
    int id = 1;
    if (*cantidad > 0) {
        id = tmp[*cantidad - 1].id + 1;
    }
    if (ingresarDatos(&tmp[*cantidad], id) != 0) {
        return 1;                           // no se cargó: la cantidad no cambia
    }

    FILE *archivo = fopen(NOMBRE_ARCHIVO, "ab");   // "ab": agrega al final
    if (archivo == NULL) {
        perror("Error al abrir el archivo para escritura");
        return 1;
    }
    int resultado = guardarContacto(archivo, &tmp[*cantidad]);
    if (fclose(archivo) != 0) {
        resultado = 1;
    }
    if (resultado == 0) {
        printf("Contacto agregado con id %d.\n", id);
        (*cantidad)++;
    }
    return resultado;
}

int buscarContacto(const Contacto *contactos, size_t cantidad) {
    char buscado[MAX_CHARS];
    if (leerTexto("Ingrese el nombre (o parte) a buscar: ", buscado, MAX_CHARS) != 0) {
        return 1;
    }
    int encontrados = 0;
    for (size_t i = 0; i < cantidad; i++) {
        if (strstr(contactos[i].nombre, buscado) != NULL) {
            mostrarContacto(&contactos[i]);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        printf("No se encontraron contactos.\n");
    }
    return 0;
}

int eliminarContacto(Contacto *contactos, size_t *cantidad) {
    char entrada[12];
    if (leerTexto("Ingrese el id del contacto a eliminar: ", entrada, 12) != 0) {
        return 1;
    }
    int id = atoi(entrada);   // si no era un número devuelve 0, y los ids empiezan en 1

    size_t posicion = *cantidad;   // "no encontrado"
    for (size_t i = 0; i < *cantidad; i++) {
        if (contactos[i].id == id) {
            posicion = i;
        }
    }
    if (posicion == *cantidad) {
        printf("No existe un contacto con id %d.\n", id);
        return 1;
    }

    // corremos un lugar hacia atrás todo lo que estaba después del eliminado
    for (size_t i = posicion; i + 1 < *cantidad; i++) {
        contactos[i] = contactos[i + 1];
    }
    (*cantidad)--;

    if (guardarContactos(contactos, *cantidad) != 0) {
        return 1;
    }
    printf("Contacto eliminado.\n");
    return 0;
}