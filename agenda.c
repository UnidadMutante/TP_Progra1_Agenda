#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agenda.h"

#define NOMBRE_ARCHIVO "agenda.txt"

Contacto *reservarMemoria(void) {
    return malloc(sizeof(Contacto));
}

void mostrarInstrucciones(void) {
    printf("Uso:\n");
    printf("./agenda mostrar  -> ver los contactos\n");
    printf("./agenda agregar  -> agregar un contacto\n");
    printf("./agenda buscar   -> buscar por nombre o teléfono\n");
    printf("./agenda eliminar -> eliminar por nombre o ID\n");
}

/* El archivo tiene registros binarios, por eso se usa "rb". */
static FILE *lecturaArchivo(void) {
    return fopen(NOMBRE_ARCHIVO, "rb");
}

/* "ab" agrega cada nuevo registro al final del archivo. */
static FILE *escrituraArchivo(void) {
    return fopen(NOMBRE_ARCHIVO, "ab");
}

/* Lee una línea y quita el salto de línea. Rechaza entradas demasiado largas. */
static int leerLinea(char *texto, size_t tamano) {
    if (fgets(texto, tamano, stdin) == NULL) {
        return 0;
    }

    for (size_t i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == '\n') {
            texto[i] = '\0';
            return 1;
        }
    }

    if (!feof(stdin)) {
        int caracter = getchar();

        if (caracter != '\n' && caracter != EOF) {
            while (caracter != '\n' && caracter != EOF) {
                caracter = getchar();
            }
            return 0;
        }
    }

    return 1;
}

/* Devuelve el primer ID disponible después del ID máximo guardado. */
static int siguienteId(void) {
    FILE *archivo = lecturaArchivo();

    if (archivo == NULL) {
        if (errno == ENOENT) {
            return 1; /* La agenda todavía no existe. */
        }

        perror("Error al abrir la agenda para leer los IDs");
        return -1;
    }

    Contacto contacto;
    int mayorId = 0;

    while (fread(&contacto, sizeof contacto, 1, archivo) == 1) {
        if (contacto.id > mayorId) {
            mayorId = contacto.id;
        }
    }

    if (ferror(archivo)) {
        perror("Error al leer los IDs");
        fclose(archivo);
        return -1;
    }

    fclose(archivo);
    return mayorId + 1;
}

static int mostrarAgenda(void) {
    FILE *archivo = lecturaArchivo();

    if (archivo == NULL) {
        if (errno == ENOENT) {
            printf("La agenda todavía no tiene contactos.\n");
            return 0;
        }

        perror("Error al abrir la agenda para lectura");
        return 1;
    }

    Contacto contacto;
    int encontrados = 0;

    while (fread(&contacto, sizeof contacto, 1, archivo) == 1) {
        printf("ID: %d\n", contacto.id);
        printf("Nombre: %s\n", contacto.nombre);
        printf("Teléfono: %s\n\n", contacto.telefono_main);
        encontrados = 1;
    }

    if (ferror(archivo)) {
        perror("Error al leer la agenda");
        fclose(archivo);
        return 1;
    }

    if (!encontrados) {
        printf("La agenda está vacía.\n");
    }

    fclose(archivo);
    return 0;
}

int ingresarDatos(Contacto *nuevo, int id) {
    /* Inicializa todos los campos de la estructura. */
    *nuevo = (Contacto){0};
    nuevo->id = id;

    printf("Ingrese el nombre del contacto: ");
    if (!leerLinea(nuevo->nombre, sizeof nuevo->nombre)) {
        printf("No se pudo leer el nombre o es demasiado largo.\n");
        return 1;
    }

    printf("Ingrese el teléfono: ");
    if (!leerLinea(nuevo->telefono_main, sizeof nuevo->telefono_main)) {
        printf("No se pudo leer el teléfono o es demasiado largo.\n");
        return 1;
    }

    return 0;
}

int guardarContacto(FILE *archivo, const Contacto *nuevo) {
    if (fwrite(nuevo, sizeof *nuevo, 1, archivo) != 1) {
        perror("Error al guardar el contacto");
        return 1;
    }

    return 0;
}

int agregarContacto(void) {
    int id = siguienteId();

    if (id < 0) {
        return 1;
    }

    Contacto *nuevo = reservarMemoria();

    if (nuevo == NULL) {
        printf("No se pudo reservar memoria.\n");
        return 1;
    }

    if (ingresarDatos(nuevo, id) != 0) {
        free(nuevo);
        return 1;
    }

    FILE *archivo = escrituraArchivo();

    if (archivo == NULL) {
        perror("Error al abrir la agenda para escritura");
        free(nuevo);
        return 1;
    }

    int resultado = guardarContacto(archivo, nuevo);

    if (fclose(archivo) != 0) {
        perror("Error al cerrar la agenda");
        resultado = 1;
    }

    if (resultado == 0) {
        printf("Contacto guardado con ID %d.\n", nuevo->id);
    }

    free(nuevo);
    return resultado;
}

/* Carga la agenda en memoria dinámica. */
static int cargarContactos(Contacto **contactos, size_t *cantidad) {
    *contactos = NULL;
    *cantidad = 0;

    FILE *archivo = lecturaArchivo();

    if (archivo == NULL) {
        if (errno == ENOENT) {
            return 0; /* Archivo inexistente: agenda vacía. */
        }

        perror("Error al abrir la agenda para lectura");
        return 1;
    }

    Contacto actual;

    while (fread(&actual, sizeof actual, 1, archivo) == 1) {
        Contacto *nuevaLista =
            realloc(*contactos, (*cantidad + 1) * sizeof **contactos);

        if (nuevaLista == NULL) {
            printf("No se pudo reservar memoria.\n");
            free(*contactos);
            *contactos = NULL;
            *cantidad = 0;
            fclose(archivo);
            return 1;
        }

        *contactos = nuevaLista;
        (*contactos)[*cantidad] = actual;
        (*cantidad)++;
    }

    if (ferror(archivo)) {
        perror("Error al leer la agenda");
        free(*contactos);
        *contactos = NULL;
        *cantidad = 0;
        fclose(archivo);
        return 1;
    }

    fclose(archivo);
    return 0;
}

/* Reescribe el archivo completo; se usa al eliminar un contacto. */
static int guardarLista(const Contacto *contactos, size_t cantidad) {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "wb");

    if (archivo == NULL) {
        perror("Error al abrir la agenda para escribir");
        return 1;
    }

    if (cantidad > 0 &&
        fwrite(contactos, sizeof *contactos, cantidad, archivo) != cantidad) {
        perror("Error al guardar la agenda");
        fclose(archivo);
        return 1;
    }

    if (fclose(archivo) != 0) {
        perror("Error al cerrar la agenda");
        return 1;
    }

    return 0;
}

int buscarContacto(void) {
    char opcion[10];
    char busqueda[MAX_CHARS];

    printf("Buscar por: 1) nombre  2) teléfono\n");
    printf("Opción: ");

    if (!leerLinea(opcion, sizeof opcion)) {
        printf("No se pudo leer la opción.\n");
        return 1;
    }

    if (strcmp(opcion, "1") != 0 && strcmp(opcion, "2") != 0) {
        printf("Opción inválida.\n");
        return 1;
    }

    printf("Texto a buscar: ");

    if (!leerLinea(busqueda, sizeof busqueda)) {
        printf("El texto de búsqueda es demasiado largo.\n");
        return 1;
    }

    if (busqueda[0] == '\0') {
        printf("La búsqueda no puede estar vacía.\n");
        return 1;
    }

    FILE *archivo = lecturaArchivo();

    if (archivo == NULL) {
        if (errno == ENOENT) {
            printf("La agenda todavía no tiene contactos.\n");
            return 0;
        }

        perror("Error al abrir la agenda para lectura");
        return 1;
    }

    Contacto contacto;
    int encontrados = 0;

    while (fread(&contacto, sizeof contacto, 1, archivo) == 1) {
        int coincide;

        if (strcmp(opcion, "1") == 0) {
            coincide = strstr(contacto.nombre, busqueda) != NULL;
        } else {
            coincide = strstr(contacto.telefono_main, busqueda) != NULL;
        }

        if (coincide) {
            printf("ID: %d\n", contacto.id);
            printf("Nombre: %s\n", contacto.nombre);
            printf("Teléfono: %s\n\n", contacto.telefono_main);
            encontrados = 1;
        }
    }

    if (ferror(archivo)) {
        perror("Error al leer la agenda");
        fclose(archivo);
        return 1;
    }

    if (!encontrados) {
        printf("No se encontraron contactos.\n");
    }

    fclose(archivo);
    return 0;
}

int eliminarContacto(void) {
    char opcion[10];
    char nombre[MAX_CHARS];
    char entradaId[20];
    int id = 0;

    printf("Eliminar por: 1) nombre  2) ID\n");
    printf("Opción: ");

    if (!leerLinea(opcion, sizeof opcion)) {
        printf("No se pudo leer la opción.\n");
        return 1;
    }

    if (strcmp(opcion, "1") == 0) {
        printf("Ingrese el nombre exacto: ");

        if (!leerLinea(nombre, sizeof nombre)) {
            printf("El nombre es demasiado largo.\n");
            return 1;
        }
    } else if (strcmp(opcion, "2") == 0) {
        printf("Ingrese el ID: ");

        if (!leerLinea(entradaId, sizeof entradaId) ||
            sscanf(entradaId, "%d", &id) != 1) {
            printf("ID inválido.\n");
            return 1;
        }
    } else {
        printf("Opción inválida.\n");
        return 1;
    }

    Contacto *contactos = NULL;
    size_t cantidad = 0;

    if (cargarContactos(&contactos, &cantidad) != 0) {
        return 1;
    }

    size_t posicion;

    for (posicion = 0; posicion < cantidad; posicion++) {
        int coincide;

        if (strcmp(opcion, "1") == 0) {
            coincide = strcmp(contactos[posicion].nombre, nombre) == 0;
        } else {
            coincide = contactos[posicion].id == id;
        }

        if (coincide) {
            break;
        }
    }

    if (posicion == cantidad) {
        printf("No se encontró el contacto.\n");
        free(contactos);
        return 0;
    }

    /* Desplaza los contactos siguientes una posición. */
    for (size_t i = posicion; i + 1 < cantidad; i++) {
        contactos[i] = contactos[i + 1];
    }

    cantidad--;

    int resultado = guardarLista(contactos, cantidad);
    free(contactos);

    if (resultado == 0) {
        printf("Contacto eliminado.\n");
    }

    return resultado;
}

int ejecutarOpcion(const char *opcion) {
    if (strcmp(opcion, "mostrar") == 0) {
        return mostrarAgenda();
    } else if (strcmp(opcion, "agregar") == 0) {
        return agregarContacto();
    } else if (strcmp(opcion, "buscar") == 0) {
        return buscarContacto();
    } else if (strcmp(opcion, "eliminar") == 0) {
        return eliminarContacto();
    }

    printf("Opción inválida: %s\n", opcion);
    mostrarInstrucciones();
    return 1;
}