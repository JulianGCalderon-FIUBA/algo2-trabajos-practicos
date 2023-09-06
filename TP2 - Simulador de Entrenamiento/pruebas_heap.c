#include "pa2mm.h"
#include "heap.h"
#include "_interno_heap.h"

// AUXILIARES

int comparador_entero(void *_e1, void *_e2)
{
    int *e1 = _e1;
    int *e2 = _e2;

    return *e1 - *e2;
}

// PRUEBAS

void puedoCrearUnHeap()
{
    heap_t *heap = heap_crear(comparador_entero);
    if (!heap)
    {
        return;
    }

    pa2m_afirmar(heap->comparador == comparador_entero, "Puedo crear un heap con comparador especificado");
    pa2m_afirmar(heap->tope == 0, "Puedo crear un heap con tope inicial cero");
    pa2m_afirmar(heap->maximo == MAXIMO_INICIAL, "Puedo crear un heap con tope inicial cero");

    heap_destruir(heap, NULL);
}

void dadoUnHeap_cantidadDevuelveCorrectamente()
{
    heap_t *heap = heap_crear(comparador_entero);
    if (!heap)
    {
        return;
    }

    heap->tope = 3;
    pa2m_afirmar(heap_cantidad(heap) == 3, "Al tener un heap con varios elementos, cantidad devuelve correctamente");
    heap->tope = 0;

    heap_destruir(heap, NULL);
}

void dadoUnHeap_puedoInsertarYQuitarElementos()
{
    heap_t *heap = heap_crear(comparador_entero);
    if (!heap)
    {
        return;
    }

    int n1 = 1;
    int n2 = 2;
    int n3 = 3;
    int n4 = 4;
    int n5 = 5;
    int n6 = 6;
    int n7 = 7;

    pa2m_afirmar(heap_insertar(heap, &n2), "Puedo insertar un elemento");
    pa2m_afirmar(heap_insertar(heap, &n3), "Puedo insertar otro elemento");
    pa2m_afirmar(heap_insertar(heap, &n5), "Puedo insertar otro elemento");
    pa2m_afirmar(heap_insertar(heap, &n1), "Puedo insertar otro elemento");
    pa2m_afirmar(heap_insertar(heap, &n4), "Puedo insertar un ultimo elemento");

    pa2m_afirmar(heap_extraer(heap) == &n1, "Puedo quitar un elemento");
    pa2m_afirmar(heap_extraer(heap) == &n2, "Puedo quitar otro elemento");

    pa2m_afirmar(heap_insertar(heap, &n7), "Puedo insertar un elemento nuevamente");
    pa2m_afirmar(heap_insertar(heap, &n6), "Puedo insertar otro elemento");

    pa2m_afirmar(heap_extraer(heap) == &n3, "Puedo quitar otro elemento");
    pa2m_afirmar(heap_extraer(heap) == &n4, "Puedo quitar otro elemento");
    pa2m_afirmar(heap_extraer(heap) == &n5, "Puedo quitar otro elemento");
    pa2m_afirmar(heap_extraer(heap) == &n6, "Puedo quitar otro elemento");
    pa2m_afirmar(heap_extraer(heap) == &n7, "Puedo quitar el ultimo elemento");

    heap_destruir(heap, NULL);
}

int main()
{
    pa2m_nuevo_grupo("Heap - Creacion");
    puedoCrearUnHeap();

    pa2m_nuevo_grupo("Heap - Cantidad");
    dadoUnHeap_cantidadDevuelveCorrectamente();

    pa2m_nuevo_grupo("Heap - Insertar y Quitar");
    dadoUnHeap_puedoInsertarYQuitarElementos();

    return pa2m_mostrar_reporte();
}
