#ifndef _INTERNO_SIMULADOR_H_
#define _INTERNO_SIMULADOR_H_

#include <stdio.h>
#include <string.h>

#include "_interno_hospital.h"
#include "hospital.h"
#include "simulador.h"
#include "lista.h"
#include "heap.h"

#define MAXIMO_STRING_VERIFICACION 100

#define NOMBRE_DIFICULTAD_0 "Facil"
#define NOMBRE_DIFICULTAD_1 "Normal"
#define NOMBRE_DIFICULTAD_2 "Dificil"

#define PUNTAJE_MAXIMO_DIFICULTAD_0 5
#define PUNTAJE_MAXIMO_DIFICULTAD_1 10
#define PUNTAJE_MAXIMO_DIFICULTAD_2 25

#define MENSAJE_CORRECTO "Adivinaste! Muy bien."
#define MENSAJE_INCORRECTO "Te equivocaste, proba otra vez."
#define MENSAJE_CERCA "Te equivocaste, pero estas cerca."
#define MENSAJE_LEJOS "Te equivocaste, estas lejos."
#define MENSAJE_ARRIBA "Te equivocaste, proba mas alto."
#define MENSAJE_ABAJO "Te equivocaste, proba mas abajo."

#define LIMITE_CERCA 10
#define LIMITE_LEJOS 20

#define ERROR -1

enum EstadoSimulacion
{
    FINALIZADO,
    EN_EJECUCION
};

struct _simulador_t
{
    hospital_t *hospital;
    EstadisticasSimulacion estadisticas;

    heap_t *pokemones_en_espera;
    pokemon_t *pokemon_atendido;

    lista_t *dificultades;
    unsigned dificultad_actual;
    unsigned intentos;

    int estado;
};

/**
 * Devuelve true si existe una dificultad con el nombre especificado.
 * Devuelve false en caso contrario.
 */
bool existe_dificultad(lista_t *dificultades, const char *nombre);

/**
 * pokemon : pokemon_t*
 * heap : heap_t*
 * 
 * Almacena el pokemon en el heap
 * 
 * Devuelve TRUE o FALSE en caso de error.
 */
bool agregar_pokemones_a_espera(void *_pokemon, void *_heap);

/**
 * Crea una lista que contiene las dificultades por defecto del simulador.
 * 
 * Devuelve un puntero a la lista o NULL en caso de error.
 */
lista_t *crear_dificultades();

/**
 * Duplica el struct pasado por parametro, almacenando el duplicado en el heap.
 * Devuelve el puntero al duplicado o NULL en caso de error.
 */
DatosDificultad *duplicar_dificultad(DatosDificultad *dificultad);

/**
 * Destruye la memoria reservada para la dificultad y su nombre
 */
void destructor_dificultad(void *_dificultad);

/**
 * Devuelve un numero positivo si el primer pokemon es de mayor nivel que el segundo.
 * Devuelve un numero negativo en el caso contrario.
 * Devuelve cero si son iguales.
 */
int comparador_pokemones_nivel(void *_p1, void *_p2);

#endif //_INTERNO_SIMULADOR_H_
