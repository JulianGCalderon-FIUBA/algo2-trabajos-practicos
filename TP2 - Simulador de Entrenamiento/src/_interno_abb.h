#ifndef ___interno_abb__H__
#define ___interno_abb__H__

#include "abb.h"
#include <stdio.h>

typedef struct nodo_abb
{
    void *elemento;
    struct nodo_abb *izquierda;
    struct nodo_abb *derecha;
} nodo_abb_t;

struct _abb_t
{
    nodo_abb_t *nodo_raiz;
    abb_comparador comparador;
    size_t tamanio;
};

/**
 * POST: 
 *      Busca al nodo mayor del subarbol 'nodo'.
 *      Lo elimina del arbol y lo guarda en 'mayor';
 */
nodo_abb_t *extraer_maximo(nodo_abb_t *nodo, nodo_abb_t **mayor);

/**
 * PRE: 
 *      'funcion' debe ser valida
 *      'terminar_ejecucion' debe ser inicialmente false
 * POST: 
 *      Aplica la funcion a cada elemento del arbol en el recorrido indicado (inorden).
 *      Corta la ejecucion si alguna de las funciones devuelve false.
 *      Devuelve la cantidad de veces que se aplico la funcion
 */
size_t abb_con_cada_elemento_inorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *), void *aux, bool *terminar_ejecucion);

/**
 * PRE: 
 *      'funcion' debe ser valida
 *      'terminar_ejecucion' debe ser inicialmente false
 * POST: 
 *      Aplica la funcion a cada elemento del arbol en el recorrido indicado (preorden).
 *      Corta la ejecucion si alguna de las funciones devuelve false.
 *      Devuelve la cantidad de veces que se aplico la funcion
 */
size_t abb_con_cada_elemento_preorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *), void *aux, bool *terminar_ejecucion);

/**
 * PRE: 
 *      'funcion' debe ser valida
 *      'terminar_ejecucion' debe ser inicialmente false
 * POST: 
 *      Aplica la funcion a cada elemento del arbol en el recorrido indicado (postorden).
 *      Corta la ejecucion si alguna de las funciones devuelve false.
 *      Devuelve la cantidad de veces que se aplico la funcion
 */
size_t abb_con_cada_elemento_postorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *), void *aux, bool *terminar_ejecucion);

/**
 * PRE: 
 *      '_parametros' debe tener la estructura valida.
 *      Es un vector de punteros void con los siguientes elementos:
 *          [0] Un vector de punteros void
 *          [1] El maximo del vector (puntero)
 *          [2] El tope del vector (puntero)
 * POST: 
 *      Devuelve TRUE si aun no se alcanzo el maximo del vector.
 *      Devuelve FALSE de lo contrario.
 */
bool agregar_elemento(void *elemento, void *_parametros);

#endif /* ___interno_abb__H__ */
