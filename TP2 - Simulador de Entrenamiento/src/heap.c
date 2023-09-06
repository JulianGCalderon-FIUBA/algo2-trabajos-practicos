#include "heap.h"
#include "_interno_heap.h"

heap_t *heap_crear(comparador_t comparador)
{
    if (!comparador)
    {
        return NULL;
    }

    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap)
    {
        return NULL;
    }

    heap->vector = malloc(MAXIMO_INICIAL * sizeof(void *));
    if (!heap->vector)
    {
        free(heap);
        return NULL;
    }

    heap->tope = 0;
    heap->maximo = MAXIMO_INICIAL;
    heap->comparador = comparador;

    return heap;
}

heap_t *heap_insertar(heap_t *heap, void *elemento)
{
    if (heap->tope == heap->maximo)
    {
        void **aux = realloc(heap->vector, (heap->maximo * FACTOR_REALLOC) * sizeof(void *));
        if (!aux)
        {
            return NULL;
        }
        heap->vector = aux;
        heap->maximo *= FACTOR_REALLOC;
    }

    heap->vector[heap->tope] = elemento;

    sift_up(heap->vector, heap->tope, heap->comparador);

    (heap->tope)++;

    return heap;
}

void *heap_extraer(heap_t *heap)
{
    if (heap->tope == 0)
    {
        return NULL;
    }

    if (heap->tope < heap->maximo / FACTOR_REALLOC)
    {
        void **aux = realloc(heap->vector, (heap->maximo / FACTOR_REALLOC) * sizeof(void *));
        if (!aux)
        {
            return NULL;
        }
        heap->vector = aux;
        heap->maximo /= FACTOR_REALLOC;
    }

    void *raiz = heap->vector[0];
    heap->vector[0] = heap->vector[heap->tope - 1];
    (heap->tope)--;

    sift_down(heap->vector, heap->tope, 0, heap->comparador);

    return raiz;
}

size_t heap_cantidad(heap_t *heap)
{
    if (!heap)
    {
        return 0;
    }
    return heap->tope;
}

void heap_destruir(heap_t *heap, destructor_t destructor)
{
    if (heap)
    {
        if (destructor)
        {
            for (size_t i = 0; i < heap->tope; i++)
            {
                destructor(heap->vector[i]);
            }
        }

        free(heap->vector);
        free(heap);
    }
}
