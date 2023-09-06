#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hospital.h"
#include "_interno_hospital.h"
#include "abb.h"
#include "lista.h"
#include "split.h"

hospital_t *hospital_crear()
{
    hospital_t *hospital = (hospital_t *)malloc(sizeof(hospital_t));
    if (!hospital)
    {
        return NULL;
    }

    hospital->abb_pokemones = abb_crear(comparador_pokemones);
    hospital->lista_entrenadores = lista_crear();
    if (!hospital->abb_pokemones || !hospital->lista_entrenadores)
    {
        abb_destruir(hospital->abb_pokemones);
        lista_destruir(hospital->lista_entrenadores);
        free(hospital);

        return NULL;
    }

    return hospital;
}

bool hospital_leer_archivo(hospital_t *hospital, const char *nombre_archivo)
{
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo)
    {
        return false;
    }

    char *linea = leer_linea(archivo);
    while (linea)
    {
        char **substrings = split(linea, ';');
        if (!substrings)
        {
            free(linea);
            return false;
        }

        entrenador_t *entrenador = agregar_entrandor(hospital->lista_entrenadores, substrings);
        if (!entrenador)
        {
            free(linea);
            free(substrings);
            return false;
        }

        int estado = agregar_pokemones(hospital->abb_pokemones, entrenador, substrings + 2);
        if (estado == ERROR)
        {
            free(linea);
            free(substrings);
            return false;
        }

        free(substrings);
        free(linea);
        linea = leer_linea(archivo);
    }

    fclose(archivo);

    return true;
}

void hospital_destruir(hospital_t *hospital)
{
    if (!hospital)
    {
        return;
    }

    abb_destruir_todo(hospital->abb_pokemones, destructor_pokemon);
    lista_destruir_todo(hospital->lista_entrenadores, destructor_entrenador);

    free(hospital);
}

size_t hospital_cantidad_pokemon(hospital_t *hospital)
{
    if (!hospital)
    {
        return 0;
    }
    return abb_tamanio(hospital->abb_pokemones);
}

size_t hospital_cantidad_entrenadores(hospital_t *hospital)
{
    if (!hospital)
    {
        return 0;
    }
    return lista_tamanio(hospital->lista_entrenadores);
}

/**
 * Aplica 'funcion' a 'pokemon'
 *  pokemon -> pokemon_t* pokemon
 *  funcion -> bool (*funcion)(pokemon_t* p)
 * 
 * Devuelve lo que devuelve 'funcion'
 */
bool hospital_a_cada_pokemon_aux(void *_pokemon, void *_funcion)
{
    pokemon_t *pokemon = _pokemon;
    bool (*funcion)(pokemon_t * p) = _funcion;

    return funcion(pokemon);
}

size_t hospital_a_cada_pokemon(hospital_t *hospital, bool (*funcion)(pokemon_t *p))
{
    if (!hospital || !funcion)
    {
        return 0;
    }

    return abb_con_cada_elemento(hospital->abb_pokemones, INORDEN, hospital_a_cada_pokemon_aux, funcion);
}

size_t pokemon_nivel(pokemon_t *pokemon)
{
    if (!pokemon)
    {
        return 0;
    }
    return pokemon->nivel;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
    if (!pokemon)
    {
        return NULL;
    }
    return pokemon->nombre;
}

entrenador_t *pokemon_entrenador(pokemon_t *pokemon)
{
    if (!pokemon)
    {
        return NULL;
    }

    return pokemon->entrenador;
}
