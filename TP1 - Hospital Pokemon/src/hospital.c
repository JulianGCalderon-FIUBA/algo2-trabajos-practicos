#include "hospital.h"
#include "split.h"
#include <stdio.h>
#include <string.h>

struct _entrenador_t
{
    char *nombre;
    size_t id;
};

struct _hospital_pkm_t
{
    size_t cantidad_pokemon;
    size_t cantidad_entrenador;
    pokemon_t *vector_pokemones;
    entrenador_t *vector_entrenadores;
};

struct _pkm_t
{
    char *nombre;
    size_t nivel;
    entrenador_t *entrenador;
};

/**
 * PRE: El ultimo elemento de 'split' debe ser el puntero NULL
 * POST: Devuelve la cantidad de pokemones que hay en el 'split'
 */
size_t contar_pokemones(char **split)
{
    size_t i = 0;
    while (*split)
    {
        split++;
        i++;
    }
    return (i - 2) / 2;
}

/**
 * PRE: 'archivo' debe ser un stream valido
 * POST: Lee el archivo hasta que encuentra un '\n' y devuelve un string 
 *         almacenado en el heap con la informacion leida. (no almacena el '\n').
 *       El tamaño del string sera un multiplo de 128, puede tener 
 *         capacidad para almacenar mas bytes de los necesarios.
 *       Devuelve NULL en caso de un error
 */
char *leer_linea(FILE *archivo)
{
    size_t tope = 0;
    size_t maximo = 128;

    char *string = (char *)malloc(maximo * sizeof(char));
    if (!string)
    {
        return NULL;
    }

    while (fgets(string + tope, (int)(maximo - tope), archivo))
    {
        size_t leido = strlen(string + tope);
        tope += leido;

        if (string[tope - 1] == '\n')
        {
            string[tope - 1] = '\0';
            return string;
        }

        maximo *= 2;
        char *aux = realloc(string, maximo * sizeof(char));
        if (!aux)
        {
            free(string);
            return NULL;
        }
        string = aux;
    }

    if (tope == 0)
    {
        free(string);
        return NULL;
    }

    return string;
}

/**
 * PRE: el maximo de 'orden_pokemones' debe ser igual al tope del vector
 * POST: almacena en 'orden_pokemones' los indices de cada pokemon de 'vector' 
 *         en orden alfabetico.
 */
void ordenar_indice_pokemones(size_t *orden_pokemones, size_t tope, pokemon_t *vector)
{
    for (size_t i = 0; i < tope; i++)
    {
        orden_pokemones[i] = i;
    }

    for (size_t i = 1; i < tope; i++)
    {
        for (size_t j = 0; j < tope - i; j++)
        {
            pokemon_t poke_actual = vector[orden_pokemones[j]];
            pokemon_t poke_siguiente = vector[orden_pokemones[j + 1]];
            int cmp = strcmp(poke_actual.nombre, poke_siguiente.nombre);
            if (cmp > 0)
            {
                size_t aux = orden_pokemones[j];
                orden_pokemones[j] = orden_pokemones[j + 1];
                orden_pokemones[j + 1] = aux;
            }
        }
    }
}

/**
 * PRE: El tope del vector debe ser menor que su maximo. (Debe teener espacio disponible)
 * POST: Agrega un entrenador_t con los datos especificados al final del vector.
 *       Aumenta el tope.
 */
void agregar_entrenador(entrenador_t *vector_entrenadores, size_t *cantidad_entrenador, size_t id, char *nombre)
{
    vector_entrenadores[*cantidad_entrenador].id = id;
    vector_entrenadores[*cantidad_entrenador].nombre = nombre;
    (*cantidad_entrenador)++;
}

/**
 * PRE: El vector debe tener suficiente espacio disponible como para almacenar los nuevos pokemones
 *      El vector 'substrings' contiene informacion de los nuevos pokemones.
 *      La informacion viene de a pares, siendo el primer elemento su nombre, y el segundo elemento su nivel.
 *      El ultimo elemento de 'substrings' debe ser el caracter nulo
 *  
 * POST: Agrega los pokemones nuevos de 'substrings' en el vector de pokemones
 */
void agregar_pokemones(pokemon_t *vector_pokemones, size_t *cantidad_pokemones, char **substrings, entrenador_t *entrenador)
{
    while (*substrings)
    {
        vector_pokemones[*cantidad_pokemones].nombre = substrings[0];
        vector_pokemones[*cantidad_pokemones].nivel = (size_t)atoi(substrings[1]);
        vector_pokemones[*cantidad_pokemones].entrenador = entrenador;
        free(substrings[1]);

        (*cantidad_pokemones)++;
        substrings += 2;
    }
}

hospital_t *hospital_crear()
{
    hospital_t *hospital = (hospital_t *)malloc(sizeof(hospital_t));
    if (!hospital)
    {
        return NULL;
    }

    hospital->cantidad_pokemon = 0;
    hospital->cantidad_entrenador = 0;
    hospital->vector_pokemones = NULL;
    hospital->vector_entrenadores = NULL;

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
        // char **substrings_original = substrings;

        size_t cantidad_nueva_pokemones = contar_pokemones(substrings);
        size_t cantidad_total_pokemones = cantidad_nueva_pokemones + hospital->cantidad_pokemon;
        size_t cantidad_total_entrenadores = hospital->cantidad_entrenador + 1;

        pokemon_t *aux_pokemon = (pokemon_t *)
            realloc(hospital->vector_pokemones, sizeof(pokemon_t) * cantidad_total_pokemones);
        if (!aux_pokemon)
        {
            free(linea);
            free(substrings);
            return false;
        }
        hospital->vector_pokemones = aux_pokemon;

        entrenador_t *aux_entrenador = (entrenador_t *)
            realloc(hospital->vector_entrenadores, sizeof(entrenador_t) * cantidad_total_entrenadores);
        if (!aux_entrenador)
        {
            free(linea);
            free(substrings);
            return false;
        }
        hospital->vector_entrenadores = aux_entrenador;

        size_t id_entrenador = (size_t)atoi(substrings[0]);
        char *nombre_entrenador = substrings[1];
        free(substrings[0]);

        agregar_entrenador(hospital->vector_entrenadores, &(hospital->cantidad_entrenador),
                           id_entrenador, nombre_entrenador);

        agregar_pokemones(hospital->vector_pokemones, &(hospital->cantidad_pokemon),
                          substrings + 2, &(hospital->vector_entrenadores[cantidad_total_entrenadores]));

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

    for (size_t i = 0; i < hospital->cantidad_pokemon; i++)
    {
        free(hospital->vector_pokemones[i].nombre);
    }
    free(hospital->vector_pokemones);

    for (size_t i = 0; i < hospital->cantidad_entrenador; i++)
    {
        free(hospital->vector_entrenadores[i].nombre);
    }
    free(hospital->vector_entrenadores);

    free(hospital);
}

size_t hospital_cantidad_pokemon(hospital_t *hospital)
{
    if (!hospital)
    {
        return 0;
    }
    return hospital->cantidad_pokemon;
}

size_t hospital_cantidad_entrenadores(hospital_t *hospital)
{
    if (!hospital)
    {
        return 0;
    }
    return hospital->cantidad_entrenador;
}

size_t hospital_a_cada_pokemon(hospital_t *hospital, bool (*funcion)(pokemon_t *p))
{
    if (!hospital || !funcion)
    {
        return 0;
    }

    bool finalizo = false;

    size_t tope = hospital->cantidad_pokemon;
    pokemon_t *vector = hospital->vector_pokemones;
    size_t orden_pokemones[tope];
    ordenar_indice_pokemones(orden_pokemones, tope, vector);

    size_t i = 0;
    while (i < tope && !finalizo)
    {
        size_t indice = orden_pokemones[i];
        pokemon_t *pokemon = &(vector[indice]);

        finalizo = !funcion(pokemon);
        i++;
    }

    return i;
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
