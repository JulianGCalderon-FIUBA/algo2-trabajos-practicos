#include "src/hospital.h"
#include "src/simulador.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define COMANDO "\e[1;31m"
#define INPUT "\e[1;36m"
#define INTRODUCCION "\e[0;97m"
#define RESET "\e[0m"

#define NOMBRE_DIFICULTAD_1 "Codigo Morse"
#define NOMBRE_DIFICULTAD_2 "Charly Garcia"
#define PUNTAJE_MAXIMO_DIFICULTAD_1 17
#define PUNTAJE_MAXIMO_DIFICULTAD_2 23
#define MENSAJE_0_CORRECTO "-.-. --- .-. .-. . -.-. - ---"
#define MENSAJE_0_ABAJO "-- . -. --- .-."
#define MENSAJE_0_ARRIBA "-- .- -.-- --- .-."
#define MENSAJE_1_CORRECTO "Quizas porque no soy un buen soldado dejo que ataques de frente y costado."
#define MENSAJE_1_ABAJO "Quizas porque no soy de la nobleza puedo nombrarte mi reina y princesa."
#define MENSAJE_1_ARRIBA "Quizas porque no soy un gran artista puedo decir \"tu pintura esta lista\"."

#define SIMBOLO_CONSOLA ">> "
#define TECLA_ESTADISTICAS 'e'
#define TECLA_FINALIZAR 'q'
#define TECLA_ATENDER 'p'
#define TECLA_ADIVINAR 'a'
#define TECLA_INFO_POKEMON 'i'
#define TECLA_SELECCIONAR_DIFICULTAD 'd'
#define TECLA_LIMPIAR 'c'

typedef struct
{
    simulador_t *simulador;
    bool jugando;
} juego_t;

// DIFICULTADES

/**
 * Devuelve la diferencia a la que se encuentra el nivel del pokemon, del nivel adivinado.
 */
int verificar_nivel(unsigned nivel_adivinado, unsigned nivel_pokemon)
{
    return (int)(nivel_pokemon - nivel_adivinado);
}

/**
 * Devuelve el puntaje obtenido en funcion de la cantidad de intentos.
 * El puntaje es inversamente proporsional a la cantidad de intentos.
 * PUNTAJE_MAXIMO_DIFICULTAD_1 es el puntaje maximo que se puede obtener.
 */
unsigned calcular_puntaje_1(unsigned cantidad_intentos)
{
    return PUNTAJE_MAXIMO_DIFICULTAD_1 / cantidad_intentos;
}

/**
 * Devuelve el puntaje obtenido en funcion de la cantidad de intentos.
 * El puntaje es inversamente proporsional a la cantidad de intentos.
 * PUNTAJE_MAXIMO_DIFICULTAD_2 es el puntaje maximo que se puede obtener.
 */
unsigned calcular_puntaje_2(unsigned cantidad_intentos)
{
    return PUNTAJE_MAXIMO_DIFICULTAD_2 / cantidad_intentos;
}

/**
 * Devuelve un string que contiene frases arbitrarias en funcion
 *  del resultado de la verificacion
 * 
 * Devuelve NULL en caso de error
 */
const char *verificacion_a_string_1(int resultado_verificacion)
{
    if (resultado_verificacion == 0)
    {
        return MENSAJE_0_CORRECTO;
    }
    else if (resultado_verificacion < 0)
    {
        return MENSAJE_0_ABAJO;
    }
    return MENSAJE_0_ARRIBA;
}

/**
 * Devuelve un string que contiene frases arbitrarias en funcion
 *  del resultado de la verificacion
 * 
 * Devuelve NULL en caso de error
 */
const char *verificacion_a_string_2(int resultado_verificacion)
{
    if (resultado_verificacion == 0)
    {
        return MENSAJE_1_CORRECTO;
    }
    else if (resultado_verificacion < 0)
    {
        return MENSAJE_1_ABAJO;
    }

    return MENSAJE_1_ARRIBA;
}

/**
 * Agrega las dificultades personalizadas al juego
 */
void agregar_dificultades(juego_t *juego)
{
    DatosDificultad dificultad1 = {
        .nombre = NOMBRE_DIFICULTAD_1,
        .calcular_puntaje = calcular_puntaje_1,
        .verificacion_a_string = verificacion_a_string_1,
        .verificar_nivel = verificar_nivel};

    simulador_simular_evento(juego->simulador, AgregarDificultad, &dificultad1);

    DatosDificultad dificultad2 = {
        .nombre = NOMBRE_DIFICULTAD_2,
        .calcular_puntaje = calcular_puntaje_2,
        .verificacion_a_string = verificacion_a_string_2,
        .verificar_nivel = verificar_nivel};
    simulador_simular_evento(juego->simulador, AgregarDificultad, &dificultad2);
}

// JUEGO

/**
 * Imprime una introduccion y los controles del juego.
 */
void imprimir_bienvenida()
{
    printf(INTRODUCCION "Bienvenido al simulador del hospital:\n");
    printf("(%c) Obtener Estadisticas\n", toupper(TECLA_ESTADISTICAS));
    printf("(%c) Atender Proximo Entrenador\n", toupper(TECLA_ATENDER));
    printf("(%c) Obtener Informacion Pokemon\n", toupper(TECLA_INFO_POKEMON));
    printf("(%c) Adivinar Nivel Pokemon\n", toupper(TECLA_ADIVINAR));
    printf("(%c) Seleccionar Dificultad\n", toupper(TECLA_SELECCIONAR_DIFICULTAD));
    printf("(%c) Finalizar Simulacion\n", toupper(TECLA_FINALIZAR));
    printf("(%c) Limpiar Pantalla\n", toupper(TECLA_LIMPIAR));
    printf(RESET);
}

/**
 * Inicializa el juego con el hospital pasado por parametro.
 */
void inicializar_juego_t(juego_t *juego, hospital_t *hospital)
{
    juego->simulador = simulador_crear(hospital);
    juego->jugando = true;
}

/**
 * Destruye la memoria reservada para el juego.
 */
void destruir_juego_t(juego_t *juego)
{
    simulador_destruir(juego->simulador);
}

/**
 * Lee un caracter de la entrada estandar y lo devuelve.
 */
char leer_comando()
{

    char linea[100];
    char *leido;
    printf(INPUT);
    leido = fgets(linea, 100, stdin);
    printf(RESET);
    if (!leido)
        return 0;
    while (*leido == ' ')
        leido++;
    return (char)tolower(*leido);
}

/**
 * Lee un numero de la entrada estandar y lo devuelve.
 * En caso de error al leer, devuelve 0;
 */
int leer_numero()
{
    int numero = 0;
    printf(INPUT);
    scanf("%i", &numero);
    printf(RESET);

    while (getchar() != '\n')
        ;

    return numero;
}

/**
 * Imprime las estadisticas del juego o un mensaje de error en caso de error.
 */
void mostrar_estadisticas(juego_t *juego)
{
    EstadisticasSimulacion e;
    simulador_simular_evento(juego->simulador, ObtenerEstadisticas, &e);

    printf("Entrenadores Totales: %i \n", e.entrenadores_totales);
    printf("Pokemones Totales: %i \n", e.pokemon_totales);
    printf("Entrenadores Atendidos: %i \n", e.entrenadores_atendidos);
    printf("Pokemones Atendidos: %i \n", e.pokemon_atendidos);
    printf("Pokmones en Espera: %i \n", e.pokemon_en_espera);
    printf("Puntos: %i \n", e.puntos);
}

/**
 * Atiende al proximo entrenador en la lista.
 * Imprime un mensaje de confirmacion o uno de error si es el caso.
 */
void atender_entrenador(juego_t *juego)
{
    ResultadoSimulacion resultado = simulador_simular_evento(juego->simulador, AtenderProximoEntrenador, NULL);
    if (resultado == ExitoSimulacion)
    {
        printf("Atendiendo al proximo entrenador, sus pokemones se sumaran a la lista de espera.\n");
    }
    else
    {
        printf("No hay mas entrenadores disponibles.\n");
    }
}

/**
 * Imprime la informacion del pokemon siendo atendido.
 * Imprime un mensaje de error si es el caso.
 */
void mostrar_informacion_pokemon(juego_t *juego)
{
    InformacionPokemon p;
    ResultadoSimulacion resultado = simulador_simular_evento(juego->simulador, ObtenerInformacionPokemonEnTratamiento, &p);
    if (resultado == ExitoSimulacion)
    {
        printf("Entrenador: %s\n", p.nombre_entrenador);
        printf("Pokemon: %s\n", p.nombre_pokemon);
    }
    else
    {
        printf("No hay ningun pokemon en tratamiento.\n");
    }
}

/**
 * Le pide entrada al usuario hasta que este adivine el nivel del pokemon.
 * El jugador recibe pistas para mejorar, dependiendo de la dificultad.
 * 
 * Imprime un mensaje de error si es el caso.
 */
void adivinar_nivel(juego_t *juego)
{
    bool hay_pokemones;
    EstadisticasSimulacion e;
    simulador_simular_evento(juego->simulador, ObtenerEstadisticas, &e);
    hay_pokemones = e.pokemon_en_espera > 0;

    if (hay_pokemones)
    {
        Intento i = {.es_correcto = false};
        bool error = false;
        while (!i.es_correcto && !error)
        {
            printf("Adivina el nivel del pokemon atendido: ");
            i.nivel_adivinado = (unsigned)leer_numero();

            simulador_simular_evento(juego->simulador, AdivinarNivelPokemon, &i);
            printf("%s\n", i.resultado_string);
        }
    }
    else
    {
        printf("No hay pokemones disponibles.\n");
    }
}

/**
 * Le pide al jugador una numero y cambia la dificultad del juego.
 */
void seleccionar_dificultad(juego_t *juego)
{
    printf("Porfavor, seleccione una dificultad: ");

    int id = leer_numero();
    ResultadoSimulacion resultado = simulador_simular_evento(juego->simulador, SeleccionarDificultad, &id);
    if (resultado == ExitoSimulacion)
    {
        printf("La dificultad se selecciono correctamente.\n");
    }
    else
    {
        printf("La dificultad ingresda no existe.\n");
    }
}

/**
 * Le pide al usuario un numero.
 * Imprime el nombre la dificultad que se encuentra en ese numero.
 */
void mostrar_informacion_dificultad(juego_t *juego)
{
    printf("Dificultades disponibles:\n");

    InformacionDificultad info = {.id = 0};
    ResultadoSimulacion resultado;

    resultado = simulador_simular_evento(juego->simulador, ObtenerInformacionDificultad, &info);
    while (resultado == ExitoSimulacion)
    {
        printf("%i : %s", info.id, info.nombre_dificultad);
        if (info.en_uso)
        {
            printf(" (en uso)\n");
        }
        else
        {
            printf("\n");
        }

        info.id++;

        resultado = simulador_simular_evento(juego->simulador, ObtenerInformacionDificultad, &info);
    }
}

/**
 * Ejecuta una comando particular, dependiendo del caracter ingresado.
 */
void ejecutar_comando(juego_t *juego, char comando)
{
    switch (comando)
    {
    case TECLA_FINALIZAR:
        juego->jugando = false;
        break;
    case TECLA_LIMPIAR:
        system("clear");
        imprimir_bienvenida();
        break;
    case TECLA_ESTADISTICAS:
        mostrar_estadisticas(juego);
        break;
    case TECLA_ATENDER:
        atender_entrenador(juego);
        break;
    case TECLA_INFO_POKEMON:
        mostrar_informacion_pokemon(juego);
        break;
    case TECLA_ADIVINAR:
        adivinar_nivel(juego);
        break;
    case TECLA_SELECCIONAR_DIFICULTAD:
        mostrar_informacion_dificultad(juego);
        seleccionar_dificultad(juego);
        break;
    default:
        printf("Introduciste una entrada invalida.\n");
        break;
    }
}

int main()
{
    hospital_t *hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    juego_t juego;
    inicializar_juego_t(&juego, hospital);
    agregar_dificultades(&juego);

    imprimir_bienvenida();

    do
    {
        printf(COMANDO SIMBOLO_CONSOLA RESET);
        char c = leer_comando();
        ejecutar_comando(&juego, c);
    } while (juego.jugando);

    destruir_juego_t(&juego);

    return 0;
}
