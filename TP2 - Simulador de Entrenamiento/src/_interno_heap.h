#ifndef _interno_heap_H_
#define _interno_heap_H_

#include <stdlib.h>

#define MAXIMO_INICIAL 3
#define FACTOR_REALLOC 2

struct _heap_t
{
    void **vector;
    size_t tope;
    size_t maximo;
    int (*comparador)(void *, void *);
};

/**
 * vector debe ser valido
 * pos_actual debe estar dentro del vector.
 * comparador debe ser una funcion valida.
 * 
 * Sube el elemento en la posicion actual hasta que el heap sea valido.
 */
void sift_up(void **vector, size_t pos_actual, int (*comparador)(void *, void *));

/**
 * vector debe ser valido
 * tope debe ser el tope del vector
 * pos_actual debe estar dentro del vector
 * comparador debe ser una funcion valida.
 * 
 * Baja el elemento de la posicion actual hasta que el heap sea valido.
 */
void sift_down(void **vector, size_t tope, size_t pos_actual, int (*comparador)(void *, void *));

#endif // _interno_heap_H_
