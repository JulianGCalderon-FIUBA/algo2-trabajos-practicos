#include "_interno_heap.h"

void sift_up(void **vector, size_t pos_actual, int (*comparador)(void *, void *))
{
    if (!vector || pos_actual == 0)
    {
        return;
    }

    size_t pos_padre = (pos_actual % 2 == 0) ? ((pos_actual - 2) / 2) : ((pos_actual - 1) / 2);

    if (comparador(vector[pos_actual], vector[pos_padre]) < 0)
    {
        void *aux = vector[pos_actual];
        vector[pos_actual] = vector[pos_padre];
        vector[pos_padre] = aux;

        sift_up(vector, pos_padre, comparador);
    }
}

void sift_down(void **vector, size_t tope, size_t pos_actual, int (*comparador)(void *, void *))
{
    if (!vector)
    {
        return;
    }

    size_t pos_hijo_izquierdo = 2 * pos_actual + 1;
    size_t pos_hijo_derecho = 2 * pos_actual + 2;

    size_t pos_hijo_menor;
    if (pos_hijo_izquierdo > tope)
    {
        return;
    }
    else if (pos_hijo_derecho > tope)
    {
        pos_hijo_menor = pos_hijo_izquierdo;
    }
    else
    {
        pos_hijo_menor = (comparador(vector[pos_hijo_derecho], vector[pos_hijo_izquierdo]) < 0)
                             ? (pos_hijo_derecho)
                             : (pos_hijo_izquierdo);
    }

    if (comparador(vector[pos_hijo_menor], vector[pos_actual]) < 0)
    {
        void *aux = vector[pos_actual];
        vector[pos_actual] = vector[pos_hijo_menor];
        vector[pos_hijo_menor] = aux;

        sift_down(vector, tope, pos_hijo_menor, comparador);
    }
}
