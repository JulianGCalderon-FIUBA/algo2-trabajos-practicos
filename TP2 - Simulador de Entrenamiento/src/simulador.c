#include "simulador.h"
#include "hospital.h"
#include "_interno_hospital.h"
#include "_interno_simulador.h"

typedef struct _simulador_t simulador_t;

/**
 * estadisticas : EstadisticasSimulacion*.
 * 
 * Almacena las estadisticas del simulador en estadisticas
 * 
 * Devuelve ExitoSimulacion o ErrorSimulacion.
 */
static ResultadoSimulacion f_ObtenerEstadisticas(simulador_t *simulador, void *_estadisticas)
{
    if (!simulador)
    {
        return ErrorSimulacion;
    }
    simulador->estadisticas.cantidad_eventos_simulados++;
    if (!_estadisticas)
    {
        return ErrorSimulacion;
    }

    EstadisticasSimulacion *estadisticas = _estadisticas;
    *estadisticas = simulador->estadisticas;

    return ExitoSimulacion;
}

/**
 * datos : NULL
 * 
 * Agrega todos los pokemones del proximo entrenador a la lista de espera
 * 
 * Devuelve ExitoSimulacion o ErrorSimulacion en caso de error.
 */
static ResultadoSimulacion f_AtenderProximoEntrenador(simulador_t *simulador, void *null)
{
    if (!simulador)
    {
        return ErrorSimulacion;
    }
    simulador->estadisticas.cantidad_eventos_simulados++;

    entrenador_t *entrenador = lista_elemento_en_posicion(simulador->hospital->lista_entrenadores,
                                                          simulador->estadisticas.entrenadores_atendidos);
    if (!entrenador)
    {
        return ErrorSimulacion;
    }

    lista_con_cada_elemento(entrenador->pokemones, agregar_pokemones_a_espera, simulador->pokemones_en_espera);

    if (!simulador->pokemon_atendido)
    {
        simulador->pokemon_atendido = heap_extraer(simulador->pokemones_en_espera);
    }
    simulador->estadisticas.pokemon_en_espera = (unsigned)heap_cantidad(simulador->pokemones_en_espera);

    simulador->estadisticas.entrenadores_atendidos++;
    return ExitoSimulacion;
}

/**
 * informacion : InformacionPokemon*
 * 
 * Almacena la informacion del pokemon siendo atendido y la almacena en informacion.
 * 
 * Devuele ExitoSimulacion o ErrorSimulacion en caso de error.
 */
static ResultadoSimulacion f_ObtenerInformacionPokemonEnTratamiento(simulador_t *simulador, void *_informacion)
{
    if (!simulador)
    {
        return ErrorSimulacion;
    }
    simulador->estadisticas.cantidad_eventos_simulados++;
    if (!_informacion)
    {
        return ErrorSimulacion;
    }

    InformacionPokemon *informacion = _informacion;
    if (!simulador->pokemon_atendido)
    {
        informacion->nombre_entrenador = NULL;
        informacion->nombre_pokemon = NULL;
        return ErrorSimulacion;
    }

    pokemon_t *pokemon = simulador->pokemon_atendido;
    informacion->nombre_pokemon = pokemon->nombre;
    informacion->nombre_entrenador = pokemon->entrenador->nombre;

    return ExitoSimulacion;
}

/**
 * intento : Intento* (inicializado con nivel_adivinado en la propuesta de la enfermera)
 * 
 * Hace los calculos necesarios dependiendo de la dificultad, almacena la respuesta del simulador en _intento.
 * 
 * Devuelve ExitoSimulacion o ErrorSimulacion en caso de error.
 */
static ResultadoSimulacion f_AdivinarNivelPokemon(simulador_t *simulador, void *_intento)
{
    if (!simulador)
    {
        return ErrorSimulacion;
    }
    simulador->estadisticas.cantidad_eventos_simulados++;
    if (!_intento)
    {
        return ErrorSimulacion;
    }

    Intento *intento = _intento;
    if (!simulador->pokemon_atendido)
    {
        return ErrorSimulacion;
    }

    simulador->intentos++;

    DatosDificultad *dificultad_actual = lista_elemento_en_posicion(simulador->dificultades, simulador->dificultad_actual);

    int resultado_verificacion = dificultad_actual->verificar_nivel(intento->nivel_adivinado, (unsigned)simulador->pokemon_atendido->nivel);
    const char *resultado_string = dificultad_actual->verificacion_a_string(resultado_verificacion);

    intento->resultado_string = resultado_string;
    intento->es_correcto = intento->nivel_adivinado == simulador->pokemon_atendido->nivel;

    if (intento->es_correcto)
    {
        simulador->estadisticas.puntos += dificultad_actual->calcular_puntaje(simulador->intentos);
        simulador->intentos = 0;
        simulador->estadisticas.pokemon_atendidos++;

        simulador->pokemon_atendido = heap_extraer(simulador->pokemones_en_espera);
        if (simulador->pokemon_atendido)
        {
            simulador->estadisticas.pokemon_en_espera--;
        }
    }

    return ExitoSimulacion;
}

/**
 * dificultad : DatosDificultad* (inicializado con la informacion de la dificultad a agregar)
 * 
 * Agrega la dificultad pasada por parametro al simulador.
 * Duplica los datos pasados, por lo que se pueden modificar luego de la funcion.
 * 
 * Devuelve ExitoSimulacion o ErrorSimulacion en caso de error.
 */
static ResultadoSimulacion f_AgregarDificultad(simulador_t *simulador, void *_dificultad)
{
    if (!simulador)
    {
        return ErrorSimulacion;
    }
    simulador->estadisticas.cantidad_eventos_simulados++;
    if (!_dificultad)
    {
        return ErrorSimulacion;
    }

    DatosDificultad *dificultad = _dificultad;

    bool existe = existe_dificultad(simulador->dificultades, dificultad->nombre);
    if (existe)
    {
        return ErrorSimulacion;
    }

    DatosDificultad *duplicado = duplicar_dificultad(dificultad);
    if (!duplicado)
    {
        return ErrorSimulacion;
    }

    lista_t *aux = lista_insertar(simulador->dificultades, duplicado);
    if (!aux)
    {
        free(duplicado);
        return ErrorSimulacion;
    }

    return ExitoSimulacion;
}

/**
 * id : int* (inicializado en id a seleccionar)
 * 
 * Selecciona la dificultad especificada (si existe)
 * 
 * Devuelve ExitoSimulacion o ErrorSimulacion en caso de error.
 */
static ResultadoSimulacion f_SeleccionarDificultad(simulador_t *simulador, void *_id)
{
    if (!simulador)
    {
        return ErrorSimulacion;
    }
    simulador->estadisticas.cantidad_eventos_simulados++;
    if (!_id)
    {
        return ErrorSimulacion;
    }

    int *id = _id;
    if (*id < 0 || *id >= lista_tamanio(simulador->dificultades))
    {
        return ErrorSimulacion;
    }

    simulador->dificultad_actual = (unsigned)*id;
    return ExitoSimulacion;
}

/**
 * informacion : InformacionDificultad* (.id inicializado en id a buscar)
 * 
 * Almacena la informacion de la dificultad actual en 'informacion'
 * 
 * Devuelve ExitoSimulacion o ErrorSimulacion en caso de error.
 */
static ResultadoSimulacion f_ObtenerInformacionDificultad(simulador_t *simulador, void *_informacion)
{
    if (!simulador)
    {
        return ErrorSimulacion;
    }
    simulador->estadisticas.cantidad_eventos_simulados++;
    if (!_informacion)
    {
        return ErrorSimulacion;
    }

    InformacionDificultad *informacion = _informacion;
    DatosDificultad *datos = lista_elemento_en_posicion(simulador->dificultades, (size_t)informacion->id);
    if (!datos)
    {
        informacion->id = ERROR;
        informacion->nombre_dificultad = NULL;
        return ErrorSimulacion;
    }

    informacion->en_uso = simulador->dificultad_actual == informacion->id;
    informacion->nombre_dificultad = datos->nombre;

    return ExitoSimulacion;
}

/**
 * datos : NULL
 * 
 * Marca el simulador como finalizado, no se podran efectuar mas simulaciones.
 * 
 * Devuelve ExitoSimulacion o ErrorSimulacion en caso de error.
 */
static ResultadoSimulacion f_FinalizarSimulacion(simulador_t *simulador, void *datos)
{
    if (!simulador)
    {
        return ErrorSimulacion;
    }

    simulador->estado = FINALIZADO;

    return ExitoSimulacion;
}

simulador_t *simulador_crear(hospital_t *hospital)
{
    if (!hospital)
    {
        return NULL;
    }

    simulador_t *simulador = malloc(sizeof(simulador_t));
    if (!simulador)
    {
        return NULL;
    }

    simulador->hospital = hospital;
    simulador->estado = EN_EJECUCION;
    simulador->dificultad_actual = 1;
    simulador->pokemon_atendido = NULL;
    simulador->intentos = 0;

    simulador->estadisticas.cantidad_eventos_simulados = 0;
    simulador->estadisticas.entrenadores_atendidos = 0;
    simulador->estadisticas.entrenadores_totales = (unsigned)hospital_cantidad_entrenadores(hospital);
    simulador->estadisticas.pokemon_atendidos = 0;
    simulador->estadisticas.pokemon_en_espera = 0;
    simulador->estadisticas.pokemon_totales = (unsigned)hospital_cantidad_pokemon(hospital);
    simulador->estadisticas.puntos = 0;

    simulador->dificultades = crear_dificultades();
    simulador->pokemones_en_espera = heap_crear(comparador_pokemones_nivel);
    if (!simulador->dificultades || !simulador->pokemones_en_espera)
    {
        heap_destruir(simulador->pokemones_en_espera, NULL);
        lista_destruir_todo(simulador->dificultades, free);
        free(simulador);
    }

    return simulador;
}

ResultadoSimulacion simulador_simular_evento(simulador_t *simulador, EventoSimulacion evento, void *datos)
{
    if (!simulador)
    {
        return ErrorSimulacion;
    }
    if (simulador->estado == FINALIZADO)
    {
        return ErrorSimulacion;
    }

    ResultadoSimulacion resultado = ErrorSimulacion;
    switch (evento)
    {
    case ObtenerEstadisticas:
        resultado = f_ObtenerEstadisticas(simulador, datos);
        break;
    case AtenderProximoEntrenador:
        resultado = f_AtenderProximoEntrenador(simulador, datos);
        break;
    case ObtenerInformacionPokemonEnTratamiento:
        resultado = f_ObtenerInformacionPokemonEnTratamiento(simulador, datos);
        break;
    case AdivinarNivelPokemon:
        resultado = f_AdivinarNivelPokemon(simulador, datos);
        break;
    case AgregarDificultad:
        resultado = f_AgregarDificultad(simulador, datos);
        break;
    case SeleccionarDificultad:
        resultado = f_SeleccionarDificultad(simulador, datos);
        break;
    case ObtenerInformacionDificultad:
        resultado = f_ObtenerInformacionDificultad(simulador, datos);
        break;
    case FinalizarSimulacion:
        resultado = f_FinalizarSimulacion(simulador, datos);
        break;
    }

    return resultado;
}

void simulador_destruir(simulador_t *simulador)
{
    if (simulador)
    {
        lista_destruir_todo(simulador->dificultades, destructor_dificultad);
        heap_destruir(simulador->pokemones_en_espera, NULL);
        hospital_destruir(simulador->hospital);
        free(simulador);
    }
}
