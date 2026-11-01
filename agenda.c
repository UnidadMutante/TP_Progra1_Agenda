#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

#include "agenda.h"
#define NOMBRE_ARCHIVO "agenda.txt"

void mostrarInstrucciones(void)
{
    printf("Instrucciones:\n");
    printf("./agenda mostrar  -> ver la lista de contactos\n");
    printf("./agenda agregar  -> agregar un contacto\n");
    printf("./agenda buscar   -> buscar y visualizar un contacto\n");
    printf("./agenda eliminar -> eliminar un contacto\n");
}

// REVISAR BIEN CARGAR CONTACTO Y LA PARTE DE POR QUE SI ESCRIBO 2 PALABRAS . EL ESPACIO HACE QUE SE CONSIDERE COMPLETADO EL TELEFONO

// si aun no hay ningun contacto, hago malloc, 
// si ya hay algun contacto, hago realloc, para expandir la memoria del arreglo
// y sumo la cantidad
Contacto *reservarMemoria(Contacto *contactos, size_t cantidad) {
    if (cantidad == 0) {
        return malloc(sizeof(Contacto));
    }
    return realloc(contactos, (cantidad + 1) * sizeof(Contacto));
}

// Para calcular bien el tamaño del arreglo de contactos cada vez que inicializo
// revisamos cuantos contactos tenemos en el archivo
size_t contarContactos(void) {
    //abro el archivo para lectura binaria
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        return 0;
    }
    //contador
    size_t cantidad = 0;
    Contacto leido;
    fread(&leido, sizeof(Contacto), 1, archivo);
    // mientras no hayamos encontrado el end of file seguimos leyendo
    while (!feof(archivo)) {
        cantidad++;
    // le pasamos 1 para que fread lea de a 1 elemento ( 1 x sizeof(Contacto))
        fread(&leido, sizeof(Contacto), 1, archivo);
    }
    fclose(archivo);
    return cantidad;
}

// Recibe el arreglo ya reservado y cuántos contactos debe traer. Llena el arreglo "contactos" con los contactos del archivo.
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

// un arreglo decae a un puntero a su primer elemento. Ya es una dirección, así que no lleva &.
int ingresarDatos(Contacto *nuevo, int id) {
    nuevo->id = id;
    printf("Ingrese el nombre del contacto: ");
    scanf(" %29s", nuevo->nombre);
    printf("Ingrese el telefono: ");
    scanf(" %16s", nuevo->telefono_main);
    return 0;
}

//guardo con fwrite - esta funcion es llamada por agregarContacto
int guardarContacto(FILE *archivo, const Contacto *nuevo) {
    if (fwrite(nuevo, sizeof(Contacto), 1, archivo) != 1) {
        return 1;
    }
    return 0;
}

void mostrarContacto(const Contacto *contacto) {
    printf("%d - %s - %s\n", contacto->id, contacto->nombre, contacto->telefono_main);
}


// lo separe aparte porque se usa varias veces
FILE *lecturaArchivo(void) {
    return fopen(NOMBRE_ARCHIVO, "rb");
}

int mostrarAgenda(void) {
    FILE *archivo = lecturaArchivo();
    if (archivo == NULL) {
        printf("La agenda está vacía.\n");
        return 0;
    }

    Contacto leido;
    printf("Contactos:\n");
    // para leer por contacto entero
    fread(&leido, sizeof(Contacto), 1, archivo);
    while (!feof(archivo)) {
        mostrarContacto(&leido);
        fread(&leido, sizeof(Contacto), 1, archivo);
    }
    fclose(archivo);

    return 0;
}
// uso un puntero al puntero. guardo lo que me devolcio reservar memoria en tmp
// y recien despues lo guardo en contactos
int agregarContacto(Contacto **contactos, size_t *cantidad) {
    Contacto *tmp = reservarMemoria(*contactos, *cantidad);
    if (tmp == NULL) {
        printf("No se pudo reservar memoria.\n");
        return 1;
    }
    // cambia el puntero contactos de main. Por eso usamos **contactos
    *contactos = tmp; 

// malloc y realloc no limpian la memoria: creo un contacto en cero y lo copio
// Declaro una variable local de tipo Contacto con todos sus campos en cero
// en el lugar nuevo (posición *cantidad), para no guardar basura en el archivo.
    Contacto vacio = {0};
// limpio todo el lugar nuevo del arreglo
    tmp[*cantidad] = vacio;

// el id es el del contacto anterior + 1 (o 1 si es el primero)
    int id = 1;
    if (*cantidad > 0) {
        // copio a id usando la posicion como referencia y le sumo 1 para que no empiece en 0 (los ids)
        id = tmp[*cantidad - 1].id + 1;
    }
    if (ingresarDatos(&tmp[*cantidad], id) != 0) {
        return 1;
    }

//append + perror para mostrar el motivo del fallo
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "ab");   
    if (archivo == NULL) {
        perror("Error al abrir el archivo para escritura");
        return 1;
    }

    // con & le pasamos la dirección de memoria del contacto que está en la posición *cantidad del arreglo.
    int contactoGuardado = guardarContacto(archivo, &tmp[*cantidad]);
    if (fclose(archivo) != 0) {
        contactoGuardado = 1;
    }
    if (contactoGuardado == 0) {
        printf("Contacto agregado.\n");

    //aumento el valor en contacto en el main solamente si el contacto se guardo bien
    (*cantidad)++;
       }
    return contactoGuardado;
}

// Para buscarContacto Muestra un mensaje y lee una línea del teclado con fgets.
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

// Para eliminar correctamente y dejar en orden el archivo
//Reescribe el archivo completo con lo que hay en el arreglo ("wb" lo pisa).
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
 