#include "_interno_simulador.h"

// FUNCIONES DIFICULTADES

/**
 * Devuelve la diferencia a la que se encuentra el nivel del pokemon, del nivel adivinado.
 */
static int verificar_nivel(unsigned nivel_adivinado, unsigned nivel_pokemon)
{
    return (int)(nivel_pokemon - nivel_adivinado);
}

/**
 * Devuelve el puntaje obtenido en funcion de la cantidad de intentos.
 * El puntaje es inversamente proporsional a la cantidad de intentos.
 */
static unsigned calcular_puntaje_0(unsigned cantidad_intentos)
{
    return PUNTAJE_MAXIMO_DIFICULTAD_0 / cantidad_intentos;
}

/**
 * Devuelve el puntaje obtenido en funcion de la cantidad de intentos.
 * El puntaje es inversamente proporsional a la cantidad de intentos.
 */
static unsigned calcular_puntaje_1(unsigned cantidad_intentos)
{
    return PUNTAJE_MAXIMO_DIFICULTAD_1 / cantidad_intentos;
}

/**
 * Devuelve el puntaje obtenido en funcion de la cantidad de intentos.
 * El puntaje es inversamente proporsional a la cantidad de intentos.
 */
static unsigned calcular_puntaje_2(unsigned cantidad_intentos)
{
    return PUNTAJE_MAXIMO_DIFICULTAD_2 / cantidad_intentos;
}

/**
 * Devuelve un string que contiene un mensaje en funcion del resultado de verificacion.
 */
static const char *verificacion_a_string_0(int resultado_verificacion)
{
    if (resultado_verificacion > 0)
    {
        return MENSAJE_ARRIBA;
    }
    else if (resultado_verificacion < 0)
    {
        return MENSAJE_ABAJO;
    }

    return MENSAJE_CORRECTO;
}

/**
 * Devuelve un string que contiene un mensaje en funcion del resultado de verificacion.
 */
static const char *verificacion_a_string_1(int resultado_verificacion)
{

    if (resultado_verificacion == 0)
    {
        return MENSAJE_CORRECTO;
    }
    else if (abs(resultado_verificacion) <= LIMITE_CERCA)
    {
        return MENSAJE_CERCA;
    }

    return MENSAJE_LEJOS;
}

/**
 * Devuelve un string que contiene un mensaje en funcion del resultado de verificacion.
 */
static const char *verificacion_a_string_2(int resultado_verificacion)
{
    if (resultado_verificacion == 0)
    {
        return MENSAJE_CORRECTO;
    }

    return MENSAJE_INCORRECTO;
}

// FUNCIONES AUXILIARES

bool existe_dificultad(lista_t *dificultades, const char *nombre)
{
    if (!dificultades || !nombre)
    {
        return false;
    }

    bool existe = false;
    lista_iterador_t *it = lista_iterador_crear(dificultades);
    while (lista_iterador_tiene_siguiente(it) && !existe)
    {
        DatosDificultad *actual = lista_iterador_elemento_actual(it);

        existe = strcmp(actual->nombre, nombre) == 0;

        lista_iterador_avanzar(it);
    }
    lista_iterador_destruir(it);

    return existe;
}

DatosDificultad *duplicar_dificultad(DatosDificultad *dificultad)
{
    DatosDificultad *duplicado = malloc(sizeof(DatosDificultad));
    if (!duplicado)
    {
        return ErrorSimulacion;
    }

    duplicado->calcular_puntaje = dificultad->calcular_puntaje;
    duplicado->verificacion_a_string = dificultad->verificacion_a_string;
    duplicado->verificar_nivel = dificultad->verificar_nivel;

    duplicado->nombre = malloc(sizeof(char) * (strlen(dificultad->nombre) + 1));
    if (!duplicado->nombre)
    {
        free(duplicado);
        return NULL;
    }
    strcpy((char *)duplicado->nombre, dificultad->nombre);

    return duplicado;
}

void destructor_dificultad(void *_dificultad)
{
    if (_dificultad)
    {
        DatosDificultad *dificultad = _dificultad;

        free((char *)dificultad->nombre);
        free(dificultad);
    }
}

lista_t *crear_dificultades()
{
    lista_t *dificultades = lista_crear();
    if (!dificultades)
    {
        return NULL;
    }

    DatosDificultad *facil = malloc(sizeof(DatosDificultad));
    DatosDificultad *normal = malloc(sizeof(DatosDificultad));
    DatosDificultad *dificil = malloc(sizeof(DatosDificultad));
    if (!facil || !normal || !dificil)
    {
        free(facil);
        free(normal);
        free(dificil);
        lista_destruir(dificultades);

        return NULL;
    }

    facil->nombre = malloc(sizeof(char) * (strlen(NOMBRE_DIFICULTAD_0) + 1));
    normal->nombre = malloc(sizeof(char) * (strlen(NOMBRE_DIFICULTAD_1) + 1));
    dificil->nombre = malloc(sizeof(char) * (strlen(NOMBRE_DIFICULTAD_2) + 1));
    if (!facil->nombre || !normal->nombre || !dificil->nombre)
    {
        destructor_dificultad(facil);
        destructor_dificultad(normal);
        destructor_dificultad(dificil);
        lista_destruir(dificultades);
        return NULL;
    }

    strcpy((char *)facil->nombre, NOMBRE_DIFICULTAD_0);
    facil->calcular_puntaje = calcular_puntaje_0;
    facil->verificar_nivel = verificar_nivel;
    facil->verificacion_a_string = verificacion_a_string_0;

    strcpy((char *)normal->nombre, NOMBRE_DIFICULTAD_1);
    normal->calcular_puntaje = calcular_puntaje_1;
    normal->verificar_nivel = verificar_nivel;
    normal->verificacion_a_string = verificacion_a_string_1;

    strcpy((char *)dificil->nombre, NOMBRE_DIFICULTAD_2);
    dificil->calcular_puntaje = calcular_puntaje_2;
    dificil->verificar_nivel = verificar_nivel;
    dificil->verificacion_a_string = verificacion_a_string_2;

    lista_t *aux1 = lista_insertar(dificultades, facil);
    lista_t *aux2 = lista_insertar(dificultades, normal);
    lista_t *aux3 = lista_insertar(dificultades, dificil);
    if (!aux1 || !aux2 || !aux3)
    {
        free(facil);
        free(normal);
        free(dificil);
        lista_destruir(dificultades);

        return NULL;
    }

    return dificultades;
}

bool agregar_pokemones_a_espera(void *_pokemon, void *_heap)
{
    if (!_pokemon || !_heap)
    {
        return false;
    }

    pokemon_t *pokemon = _pokemon;
    heap_t *heap = _heap;

    heap_insertar(heap, pokemon);

    return true;
}

int comparador_pokemones_nivel(void *_p1, void *_p2)
{
    pokemon_t *p1 = _p1;
    pokemon_t *p2 = _p2;

    return (int)(p1->nivel - p2->nivel);
}
