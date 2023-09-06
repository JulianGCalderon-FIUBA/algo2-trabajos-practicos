#ifndef INTERNO_HOSPITAL_H
#define INTERNO_HOSPITAL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hospital.h"
#include "lista.h"
#include "abb.h"

#define ERROR -1
#define EXITO 0

struct _entrenador_t
{
    char *nombre;
    lista_t *pokemones;
    size_t id;
};

struct _hospital_pkm_t
{
    abb_t *abb_pokemones;
    lista_t *lista_entrenadores;
};

struct _pkm_t
{
    char *nombre;
    size_t nivel;
    entrenador_t *entrenador;
};

/**
 * Recibe dos punteros a pokemon_t.
 * Devuelve un numero positivo si el primer pokemon es mayor al segundo.
 */
int comparador_pokemones(void *_p1, void *_p2);

/**
 * Recibe un puntero pokemon_t.
 * Libera la memoria reservada para el pokemon.
 */
void destructor_pokemon(void *_p1);

/**
 * Recibe un puntero entrenador_t.
 * Libera la memoria reservada para el entrenador.
 */
void destructor_entrenador(void *_e1);

/**
 * El ultimo elemento de 'split' debe ser el puntero NULL
 * El formato del split es:
 *  ID;ENTRENADOR;POKEMON;NIVEL;POKEMON;NIVEL;...;...;POKEMON;NIVEL
 * 
 * Devuelve la cantidad de pokemones que hay en el 'split'
 */
size_t contar_pokemones(char **split);

/**
 * Lee el archivo hasta que encuentra un '\n' y devuelve un string 
 *   almacenado en el heap con la informacion leida. (no almacena el '\n').
 * 
 * La capacidad maxima del string sera un multiplo de 128, puede tener 
 *  capacidad para almacenar mas bytes de los necesarios.
 * 
 * Devuelve NULL en caso de un error
 */
char *leer_linea(FILE *archivo);

/**
 * El formato de 'substrings' debe ser:
 *  {"POKEMON", "NIVEL", ..., ..., "POKEMON", "NIVEL", NULL}
 *  
 * Agrega los pokemones de 'substrings' con su respectivo nivel en abb de pokemones.
 */
int agregar_pokemones(abb_t *abb_pokemones, entrenador_t *entrenador, char **substrings);

/**
 * El formato de 'substrings' debe ser:
 *  {"ID", "ENTRENADOR", ..., ...}
 *  
 * Agrega los entrenador de 'substrings' (si no existe) con su respectivo id en la lista de entrenadores.
 * Solo utiliza los primeros dos elementos de 'substrings'
 * 
 * Devuelve el entrenador, o NULL en caso de error.
 */
entrenador_t *agregar_entrandor(lista_t *entrenadores, char **substrings);

#endif // INTERNO_HOSPITAL_H
