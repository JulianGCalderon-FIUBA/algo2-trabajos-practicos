#include "split.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * PRE: El ultimo elemento de 'split' debe ser 'NULL'
 * POST: Libera la memoria reservada para los elementos del split.
 */
void liberar_vector(char **split)
{
    while (*split)
    {
        free(*split);
        split++;
    }
}

/**
 * PRE: 'indice' no puede ser mayor que la posicion del '\0'
 * POST: Devuelve la posicion del separador, buscando a partir del indice.
 *       Devuelve la posicion del '\0' si no encuentra al separador.
 */
int buscar_proximo_separador(const char *string, char separador, int indice)
{
    while (string[indice] != '\0' && string[indice] != separador)
    {
        indice++;
    }
    return indice;
}

/**
 * PRE: El ultimo elemento de 'string' debe ser '\0'
 * POST: Devuelve la cantidad de caracteres 'separador' que hay en el string.
 */
size_t contar_substrings(const char *string, char separador)
{
    int i = 0;
    size_t contador = 0;

    while (string[i] != '\0')
    {
        if (string[i] == separador)
        {
            contador++;
        }
        i++;
    }
    return contador + 1;
}

char **split(const char *string, char separador)
{
    if (!string)
        return NULL;

    size_t cantidad_substrings = contar_substrings(string, separador);
    char **split = calloc(cantidad_substrings + 1, sizeof(char *));
    if (!split)
        return NULL;

    int posicion_actual = 0;
    for (int tope_split = 0; tope_split < cantidad_substrings; tope_split++)
    {
        int posicion_proximo = buscar_proximo_separador(string, separador, posicion_actual);
        size_t longitud_substring = (size_t)(posicion_proximo - posicion_actual);

        char *aux_string = (char *)calloc(longitud_substring + 1, sizeof(char));
        if (!aux_string)
        {
            liberar_vector(split);
            free(split);
            return NULL;
        }
        split[tope_split] = aux_string;

        strncpy(split[tope_split], string + posicion_actual, longitud_substring);

        posicion_actual = posicion_proximo + 1;
    }

    return split;
}
