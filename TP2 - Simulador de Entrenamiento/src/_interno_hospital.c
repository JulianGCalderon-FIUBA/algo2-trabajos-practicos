#include "_interno_hospital.h"

int comparador_pokemones(void *_p1, void *_p2)
{
    pokemon_t *p1 = _p1;
    pokemon_t *p2 = _p2;

    return strcmp(p1->nombre, p2->nombre);
}

void destructor_pokemon(void *_p1)
{
    pokemon_t *p1 = _p1;

    free(p1->nombre);
    free(p1);
}

void destructor_entrenador(void *_e1)
{
    entrenador_t *e1 = _e1;

    lista_destruir(e1->pokemones);
    free(e1->nombre);
    free(e1);
}

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

int agregar_pokemones(abb_t *abb_pokemones, entrenador_t *entrenador, char **substrings)
{
    if (!*substrings)
    {
        return EXITO;
    }

    pokemon_t *pokemon = malloc(sizeof(pokemon_t));
    if (!pokemon)
    {
        return ERROR;
    }

    pokemon->nombre = substrings[0];
    pokemon->nivel = (size_t)atoi(substrings[1]);
    pokemon->entrenador = entrenador;
    free(substrings[1]);

    abb_t *abb_aux = abb_insertar(abb_pokemones, pokemon);
    lista_t *lista_aux = lista_insertar(entrenador->pokemones, pokemon);
    if (!abb_aux || !lista_aux)
    {
        return ERROR;
    }

    return agregar_pokemones(abb_pokemones, entrenador, substrings + 2);
}

/**
 * Devuelve la posicion del entrenador con el id especificado.
 * En caso de error, devuelve -1
 */
static int posicion_entrenador_en_lista(lista_t *entrenadores, size_t id)
{
    int posicion = ERROR;
    int indice = 0;
    lista_iterador_t *it = lista_iterador_crear(entrenadores);
    while (lista_iterador_tiene_siguiente(it) && posicion == -1)
    {
        entrenador_t *entrenador = lista_iterador_elemento_actual(it);

        if (entrenador->id == id)
        {
            posicion = indice;
        }

        indice++;
        lista_iterador_avanzar(it);
    }
    lista_iterador_destruir(it);

    return posicion;
}

entrenador_t *agregar_entrandor(lista_t *entrenadores, char **substrings)
{
    size_t id_entrenador = (size_t)atoi(substrings[0]);
    free(substrings[0]);

    int posicion_entrenador = posicion_entrenador_en_lista(entrenadores, id_entrenador);

    entrenador_t *entrenador;
    if (posicion_entrenador == ERROR)
    {
        entrenador = malloc(sizeof(entrenador_t));
        if (!entrenador)
        {
            return NULL;
        }
        entrenador->id = id_entrenador;
        entrenador->nombre = substrings[1];
        entrenador->pokemones = lista_crear();
        if (!entrenador->pokemones)
        {
            free(entrenador);
            return NULL;
        }

        lista_t *lista_aux = lista_insertar(entrenadores, entrenador);
        if (!lista_aux)
        {
            destructor_entrenador(entrenador);
            return NULL;
        }
    }
    else
    {
        entrenador = lista_elemento_en_posicion(entrenadores, (size_t)posicion_entrenador);
        free(substrings[1]);
    }

    return entrenador;
}
