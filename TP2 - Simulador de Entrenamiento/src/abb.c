#include "abb.h"
#include "_interno_abb.h"

#define ERROR 0

abb_t *abb_crear(abb_comparador comparador)
{
    if (!comparador)
    {
        return NULL;
    }

    abb_t *arbol = calloc(1, sizeof(abb_t));
    if (!arbol)
    {
        return NULL;
    }

    arbol->comparador = comparador;

    return arbol;
}

/**
 * PRE: 
 *      'comparador' debe ser una funcion valida.
 * POST: 
 *      Inserta el elemento en la posicion correcta del arbol.
 *      Si pudo insertar correctamente, almacena TRUE en 'pudo_insertar'
 *      Si no pudo, almacena FALSE.
 */
static nodo_abb_t *abb_insertar_aux(nodo_abb_t *nodo, void *elemento, abb_comparador comparador, bool *pudo_insertar)
{
    if (!comparador)
    {
        return NULL;
    }

    if (!nodo)
    {
        nodo_abb_t *nuevo = calloc(1, sizeof(nodo_abb_t));
        if (!nuevo)
        {
            *pudo_insertar = false;
            return NULL;
        }
        nuevo->elemento = elemento;

        *pudo_insertar = true;
        return nuevo;
    }

    int comparacion = comparador(elemento, nodo->elemento);

    if (comparacion >= 0)
    {
        nodo->derecha = abb_insertar_aux(nodo->derecha, elemento, comparador, pudo_insertar);
    }
    else
    {
        nodo->izquierda = abb_insertar_aux(nodo->izquierda, elemento, comparador, pudo_insertar);
    }

    return nodo;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
    if (!arbol)
    {
        return NULL;
    }

    bool pudo_insertar;
    arbol->nodo_raiz = abb_insertar_aux(arbol->nodo_raiz, elemento, arbol->comparador, &pudo_insertar);

    if (!pudo_insertar)
    {
        return NULL;
    }

    arbol->tamanio++;
    return arbol;
}

/**
 * PRE: 
 *      'comparador' debe ser una funcion valida.
 * POST: 
 *      Elimina el nodo del subarbol que contiene el elemento indicado.
 *      Devuelve el nodo ingresado (actualizado).
 *      Almacena el elemento eliminado en 'elemento_quitado'.
 */
static nodo_abb_t *abb_quitar_aux(nodo_abb_t *nodo, void *elemento, abb_comparador comparador, void **elemento_quitado)
{
    if (!nodo)
    {
        return NULL;
    }

    int comparacion = comparador(elemento, nodo->elemento);

    if (comparacion == 0)
    {
        *elemento_quitado = nodo->elemento;

        if (nodo->izquierda && nodo->derecha)
        {
            nodo_abb_t *remplazo;
            nodo->izquierda = extraer_maximo(nodo->izquierda, &remplazo);

            remplazo->derecha = nodo->derecha;
            remplazo->izquierda = nodo->izquierda;

            free(nodo);

            return remplazo;
        }
        else
        {
            nodo_abb_t *remplazo = nodo->derecha ? nodo->derecha : nodo->izquierda;

            free(nodo);

            return remplazo;
        }
    }
    else if (comparacion > 0)
    {
        nodo->derecha = abb_quitar_aux(nodo->derecha, elemento, comparador, elemento_quitado);
    }
    else
    {
        nodo->izquierda = abb_quitar_aux(nodo->izquierda, elemento, comparador, elemento_quitado);
    }

    return nodo;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
    if (!arbol)
    {
        return NULL;
    }

    void *elemento_quitado = NULL;
    arbol->nodo_raiz = abb_quitar_aux(arbol->nodo_raiz, elemento, arbol->comparador, &elemento_quitado);

    if (elemento_quitado)
    {
        (arbol->tamanio)--;
    }

    return elemento_quitado;
}

/**
 * PRE:
 *      'comparador' debe ser una funcion valida.
 * POST: 
 *      Busca el elemento en el arbol.
 *      Si lo encuentra, devuelve el elemento.
 *      Si no lo encuentra, devuelve NULL.
 */
static void *abb_buscar_aux(nodo_abb_t *nodo, void *elemento, abb_comparador comparador)
{
    if (!nodo)
    {
        return NULL;
    }

    int comparacion = comparador(elemento, nodo->elemento);

    if (comparacion == 0)
    {
        return nodo->elemento;
    }
    else if (comparacion > 0)
    {
        return abb_buscar_aux(nodo->derecha, elemento, comparador);
    }
    return abb_buscar_aux(nodo->izquierda, elemento, comparador);
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
    if (!arbol || !elemento)
    {
        return NULL;
    }

    return abb_buscar_aux(arbol->nodo_raiz, elemento, arbol->comparador);
}

bool abb_vacio(abb_t *arbol)
{
    if (!arbol)
    {
        return true;
    }

    return arbol->tamanio == 0;
}

size_t abb_tamanio(abb_t *arbol)
{
    if (!arbol)
    {
        return ERROR;
    }

    return arbol->tamanio;
}

/**
 * POST: 
 *      Libera la memoria almacenada para el nodo ingresado y todos sus sucesores.
 */
static void abb_destruir_aux(nodo_abb_t *nodo)
{
    if (!nodo)
    {
        return;
    }

    abb_destruir_aux(nodo->izquierda);
    abb_destruir_aux(nodo->derecha);

    free(nodo);
}

void abb_destruir(abb_t *arbol)
{
    if (!arbol)
    {
        return;
    }

    abb_destruir_aux(arbol->nodo_raiz);

    free(arbol);
}

/**
 * PRE: 
 *      'destructor' debe ser una funcion valida.
 * POST: 
 *      Liberar la memoria almacenada para el nodo ingresado y todos sus sucesores.
 *      Aplica la funcion destructor a todos los elementos.
 */
static void abb_destruir_todo_aux(nodo_abb_t *nodo, void (*destructor)(void *))
{
    if (!nodo || !destructor)
    {
        return;
    }

    abb_destruir_todo_aux(nodo->izquierda, destructor);
    abb_destruir_todo_aux(nodo->derecha, destructor);

    if (nodo->elemento)
    {
        destructor(nodo->elemento);
    }
    free(nodo);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
    if (!arbol || !destructor)
    {
        return;
    }

    abb_destruir_todo_aux(arbol->nodo_raiz, destructor);

    free(arbol);
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido, bool (*funcion)(void *, void *), void *aux)
{
    if (!arbol || !funcion)
    {
        return ERROR;
    }

    bool terminar_ejecucion = false;

    switch (recorrido)
    {
    case INORDEN:
        return abb_con_cada_elemento_inorden(arbol->nodo_raiz, funcion, aux, &terminar_ejecucion);
    case PREORDEN:
        return abb_con_cada_elemento_preorden(arbol->nodo_raiz, funcion, aux, &terminar_ejecucion);
    case POSTORDEN:
        return abb_con_cada_elemento_postorden(arbol->nodo_raiz, funcion, aux, &terminar_ejecucion);
    }

    return ERROR;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array, size_t tamanio_array)
{
    if (!arbol || !array || tamanio_array == 0)
    {
        return ERROR;
    }

    int tope = 0;
    void *parametros[3] = {array, &tamanio_array, &tope};
    return abb_con_cada_elemento(arbol, recorrido, agregar_elemento, parametros);
}
