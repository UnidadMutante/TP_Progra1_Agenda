#ifndef __AGENDA_H__
#define __AGENDA_H__
#include <stdio.h>  // FILE
#include <stddef.h> // size_t
#define MAX_CHARS 30
#define PHONE_LENGTH 17

typedef struct {
    int id;
    char nombre[MAX_CHARS];
    char apellido[MAX_CHARS];
    char telefono[PHONE_LENGTH];
} Contacto;

//Funciones llamadas desde el main
// @post: muestra por pantalla las instrucciones de uso del programa.
void mostrarInstrucciones(void);

// @post: devuelve la cantidad de contactos del archivo
size_t contarContactos(void);

// @pre: contactos debe tener memoria reservada para cantidad contactos.
// @post: carga los contactos del archivo en el arreglo recibido.
// @params: Recibe el arreglo ya reservado y cuántos contactos debería tener.
int cargarContactos(Contacto *contactos, size_t cantidad);

//Opcion mostrar

// @param contacto que se quiere visualizar.
// @pre: contacto debe apuntar a una estructura Contacto válida.
// @post: muestra por pantalla los datos del contacto.
void mostrarContacto(Contacto *contacto);

// @post: muestra todos los contactos almacenados en la agenda.
// @return: 0 si la operación finalizó correctamente.
int mostrarAgenda(void);

//Opción Agregar

// @param contactos: arreglo actual de contactos.
// @param cantidad: cantidad actual de contactos.
// @pre: una estructura de contactos inicializada. Puede estar vacía. 
// @post: devuelve un puntero a la memoria reservada para la cantidad de contactos que le haya pasado por size_t cantidad + 1 (Asi expande la memoria para guardar un contacto mas)
Contacto *reservarMemoria(Contacto *contactos, size_t cantidad);

// @param nuevo: contacto nuevo que se ingresa
// @param id: identificador que se asignará al contacto.
// @pre: nuevo debe apuntar a una estructura Contacto válida.
// @post: completa los datos del contacto ingresados por el usuario.
int ingresarDatos(Contacto *nuevo, int id);

// @param archivo: archivo abierto para escritura.
// @param nuevo: contacto que se desea guardar.
// @pre: archivo debe estar abierto correctamente para write o append
// @post: escribe el contacto en el archivo.
int guardarContacto(FILE *archivo, Contacto *nuevo);

// @param contactos: dirección del arreglo dinámico de contactos.
// @param cantidad: cantidad actual de contactos.
// @pre: contactos y cantidad deben ser punteros válidos.
// @post: agrega un nuevo contacto a la agenda y actualiza la cantidad.
int agregarContacto(Contacto **contactos, size_t *cantidad);

//Opcion buscar

// @param contactos: arreglo de contactos.
// @param cantidad: cantidad de contactos almacenados.
// @pre: contactos debe apuntar a un arreglo válido.
// @post: muestra los contactos cuyo nombre coincide con el buscado
int buscarContacto(Contacto *contactos, size_t cantidad);

//Opcion eliminar

// @param contactos: arreglo de contactos.
// @param cantidad: cantidad de contactos almacenados.
// @pre: contactos debe contener cantidad contactos válidos.
// @post: reescribe el archivo con los contactos actuales (wb)
int reescribirArchivo(Contacto *contactos, size_t cantidad);

// @param contactos: arreglo de contactos.
// @param cantidad: cantidad actual de contactos.
// @pre: contactos y cantidad deben ser punteros válidos.
// @post: elimina un contacto de la agenda y actualiza el archivo.
int eliminarContacto(Contacto *contactos, size_t *cantidad);

#endif