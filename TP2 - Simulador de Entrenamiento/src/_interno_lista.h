#ifndef _interno_H_
#define _interno_H_

#include "lista.h"

struct _nodo_t
{
    void *elemento;
    struct _nodo_t *siguiente;
};

struct _lista_t
{
    nodo_t *nodo_inicio;
    nodo_t *nodo_fin;
    size_t cantidad;
};

struct _lista_iterador_t
{
    nodo_t *corriente;
    lista_t *lista;
};

struct _pila_t
{
    nodo_t *nodo_inicio;
    nodo_t *nodo_fin;
    size_t cantidad;
};

struct _cola_t
{
    nodo_t *nodo_inicio;
    nodo_t *nodo_fin;
    size_t cantidad;
};

/**
 * Devuelve el nodo en la posicion especificada.
 * Toma el nodo inicial como posicion 0
 */
nodo_t *buscar_nodo_posicion(nodo_t *inicial, size_t posicion);

#endif // _interno_H_
