#ifndef HEAP_H_
#define HEAP_H_

#include <stddef.h>

typedef struct _heap_t heap_t;
typedef int (*comparador_t)(void *, void *);
typedef void (*destructor_t)(void *);

/**
 * La funcion comparador debe ser valida.
 * 
 * Reserva memoria para el heap, inicializa los valores iniciales.
 * 
 * el heap es un heap MINIMAL.
 */
heap_t *heap_crear(comparador_t comparador);

/**
 * heap debe ser un heap_t valido.
 * El elemento debe poder ser comparable con la funcion comparador.
 * 
 * Inserta un elemento en el heap.
 * 
 * Devuelve el heap_t* o NULL en caso de error.
 */
heap_t *heap_insertar(heap_t *heap, void *elemento);

/**
 * heap debe ser un heap_t valido.
 * 
 * Elimina y devuelve el elemento de la raiz del heap.
 */
void *heap_extraer(heap_t *heap);

/**
 * heap debe ser un heap_t valido.
 * 
 * Devuelve la cantidad de elementos que contiene el heap.
 */
size_t heap_cantidad(heap_t *heap);

/**
 * heap debe ser un heap_t valido.
 * 
 * Destruye la memoria reservada por el heap.
 * Invoca la funcion destructor en cada elemento si este no es nulo.
 */
void heap_destruir(heap_t *heap, destructor_t destructor);

#endif // HEAP_H_
