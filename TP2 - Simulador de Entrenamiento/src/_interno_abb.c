#include "_interno_abb.h"

nodo_abb_t *extraer_maximo(nodo_abb_t *nodo, nodo_abb_t **mayor)
{
    if (!nodo->derecha)
    {
        *mayor = nodo;

        return nodo->izquierda;
    }

    nodo->derecha = extraer_maximo(nodo->derecha, mayor);
    return nodo;
}

size_t abb_con_cada_elemento_inorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *), void *aux, bool *terminar_ejecucion)
{
    if (!nodo || *terminar_ejecucion)
    {
        return 0;
    }

    size_t contador = abb_con_cada_elemento_inorden(nodo->izquierda, funcion, aux, terminar_ejecucion);

    if (!*terminar_ejecucion)
    {
        contador++;
        *terminar_ejecucion = !funcion(nodo->elemento, aux);
    }

    contador += abb_con_cada_elemento_inorden(nodo->derecha, funcion, aux, terminar_ejecucion);

    return contador;
}

size_t abb_con_cada_elemento_preorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *), void *aux, bool *terminar_ejecucion)
{
    if (!nodo || *terminar_ejecucion)
    {
        return 0;
    }

    size_t contador = 1;
    *terminar_ejecucion = !funcion(nodo->elemento, aux);

    contador += abb_con_cada_elemento_preorden(nodo->izquierda, funcion, aux, terminar_ejecucion);

    contador += abb_con_cada_elemento_preorden(nodo->derecha, funcion, aux, terminar_ejecucion);

    return contador;
}

size_t abb_con_cada_elemento_postorden(nodo_abb_t *nodo, bool (*funcion)(void *, void *), void *aux, bool *terminar_ejecucion)
{
    if (!nodo || *terminar_ejecucion)
    {
        return 0;
    }

    size_t contador = abb_con_cada_elemento_postorden(nodo->izquierda, funcion, aux, terminar_ejecucion);

    contador += abb_con_cada_elemento_postorden(nodo->derecha, funcion, aux, terminar_ejecucion);

    if (!*terminar_ejecucion)
    {
        contador++;
        *terminar_ejecucion = !funcion(nodo->elemento, aux);
    }

    return contador;
}

bool agregar_elemento(void *elemento, void *_parametros)
{
    if (!_parametros)
    {
        return false;
    }

    void **parametros = _parametros;

    void **arreglo = parametros[0];
    int *maximo = parametros[1];
    int *tope = parametros[2];

    if (*tope == *maximo)
    {
        return false;
    }

    arreglo[*tope] = elemento;
    (*tope)++;

    return *tope != *maximo;
}
