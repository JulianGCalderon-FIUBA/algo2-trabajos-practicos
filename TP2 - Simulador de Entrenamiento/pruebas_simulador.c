#include "pa2mm.h"
#include "simulador.h"
#include "_interno_simulador.h"

// AUXILIARES
unsigned _calcular_puntaje(unsigned cantidad_intentos)
{
    return 0;
}
int _verificar_nivel(unsigned nivel_adivinado, unsigned nivel_pokemon)
{
    return 0;
}
const char *_verificacion_a_string(int resultado_verificacion)
{
    return NULL;
}

// PRUEBAS

void dadoUnSimuladorNULL_noPuedoSimular()
{
    pa2m_afirmar(simulador_simular_evento(NULL, ObtenerEstadisticas, NULL) == ErrorSimulacion, "Dado un simulador nulo, no puedo simular ObtenerEstadisticas.");
    pa2m_afirmar(simulador_simular_evento(NULL, AtenderProximoEntrenador, NULL) == ErrorSimulacion, "Dado un simulador nulo, no puedo simular AtenderProximoEntrenador.");
    pa2m_afirmar(simulador_simular_evento(NULL, ObtenerInformacionPokemonEnTratamiento, NULL) == ErrorSimulacion, "Dado un simulador nulo, no puedo simular ObtenerInformacionPokemonEnTratamiento.");
    pa2m_afirmar(simulador_simular_evento(NULL, AdivinarNivelPokemon, NULL) == ErrorSimulacion, "Dado un simulador nulo, no puedo simular AdivinarNivelPokemon.");
    pa2m_afirmar(simulador_simular_evento(NULL, AgregarDificultad, NULL) == ErrorSimulacion, "Dado un simulador nulo, no puedo simular AgregarDificultad.");
    pa2m_afirmar(simulador_simular_evento(NULL, SeleccionarDificultad, NULL) == ErrorSimulacion, "Dado un simulador nulo, no puedo simular SeleccionarDificultad.");
    pa2m_afirmar(simulador_simular_evento(NULL, ObtenerInformacionDificultad, NULL) == ErrorSimulacion, "Dado un simulador nulo, no puedo simular ObtenerInformacionDificultad.");
    pa2m_afirmar(simulador_simular_evento(NULL, FinalizarSimulacion, NULL) == ErrorSimulacion, "Dado un simulador nulo, no puedo simular FinalizarSimulacion.");
}

void puedoCrearUnSimulador()
{
    hospital_t *hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t *simulador = simulador_crear(hospital);
    pa2m_afirmar(!simulador->pokemon_atendido, "Se crea con pokemon atendido inicial nulo");
    pa2m_afirmar(heap_cantidad(simulador->pokemones_en_espera) == 0, "Se crea con heap inicialmente vacio");
    pa2m_afirmar(simulador->intentos == 0, "Se crea con cantidad de intentos inicial CERO");

    pa2m_afirmar(simulador->hospital == hospital, "El hospital es el especificado");
    pa2m_afirmar(simulador->estado == EN_EJECUCION, "El estado inicial es correcto");

    pa2m_afirmar(simulador->estadisticas.cantidad_eventos_simulados == 0, "La cantidad de elementos simulados es cero");
    pa2m_afirmar(simulador->estadisticas.entrenadores_atendidos == 0, "La cantidad de entrenadores atendidos es cero");
    pa2m_afirmar(simulador->estadisticas.entrenadores_totales == hospital_cantidad_entrenadores(hospital),
                 "La cantidad de elementos totales es correcta");
    pa2m_afirmar(simulador->estadisticas.pokemon_atendidos == 0, "La cantidad de pokemones atendidos es cero");
    pa2m_afirmar(simulador->estadisticas.pokemon_en_espera == 0, "La cantidad de pokemones en espera es cero");
    pa2m_afirmar(simulador->estadisticas.pokemon_totales == hospital_cantidad_pokemon(hospital),
                 "La cantidad de pokemones totales es correcta");
    pa2m_afirmar(simulador->estadisticas.puntos == 0, "La cantidad de puntos es cero");

    pa2m_afirmar(lista_tamanio(simulador->dificultades) == 3, "La lista de dificultades se crea con 3 dificultades iniciales");

    DatosDificultad *facil = lista_elemento_en_posicion(simulador->dificultades, 0);
    bool facil_es_correcta = strcmp(facil->nombre, NOMBRE_DIFICULTAD_0) == 0;

    DatosDificultad *normal = lista_elemento_en_posicion(simulador->dificultades, 1);
    bool normal_es_correcta = strcmp(normal->nombre, NOMBRE_DIFICULTAD_1) == 0;

    DatosDificultad *dificil = lista_elemento_en_posicion(simulador->dificultades, 2);
    bool dificil_es_correcta = strcmp(dificil->nombre, NOMBRE_DIFICULTAD_2) == 0;

    pa2m_afirmar(facil_es_correcta, "La dificultad facil se crea correctamente");
    pa2m_afirmar(normal_es_correcta, "La dificultad normal se crea correctamente");
    pa2m_afirmar(dificil_es_correcta, "La dificultad dificil se crea correctamente");

    simulador_destruir(simulador);
}

void dadoUnSimulador_puedoObtenerEstadisticas()
{
    hospital_t *hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t *simulador = simulador_crear(hospital);

    simulador->estadisticas.cantidad_eventos_simulados = 15;
    simulador->estadisticas.entrenadores_atendidos = 2;
    simulador->estadisticas.entrenadores_totales = 5;
    simulador->estadisticas.pokemon_atendidos = 2;
    simulador->estadisticas.pokemon_en_espera = 5;
    simulador->estadisticas.pokemon_totales = 10;
    simulador->estadisticas.puntos = 100;

    EstadisticasSimulacion e;
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e) == ExitoSimulacion, "puedo obtener estadisticas de un simulador valido");
    pa2m_afirmar(e.cantidad_eventos_simulados == 16, "La cantidad de elementos simulados es correcta");
    pa2m_afirmar(e.entrenadores_atendidos == 2, "La cantidad de entrenadores atendidos es correcta");
    pa2m_afirmar(e.entrenadores_totales == 5, "La cantidad de elementos totales es correcta");
    pa2m_afirmar(e.pokemon_atendidos == 2, "La cantidad de pokemones atendidos es correcta");
    pa2m_afirmar(e.pokemon_en_espera == 5, "La cantidad de pokemones en espera es correcta");
    pa2m_afirmar(e.pokemon_totales == 10, "La cantidad de pokemones totales es correcta");
    pa2m_afirmar(e.puntos == 100, "La cantidad de puntos es correcta");

    pa2m_afirmar(!simulador_simular_evento(simulador, ObtenerEstadisticas, NULL), "Dado un simulador, al pasarle datos invalidos, no puedo obtener estadisticas");

    simulador_destruir(simulador);
}

void dadoUnSimulador_alFinalizar_noPuedoSimularMas()
{
    hospital_t *hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t *simulador = simulador_crear(hospital);

    EstadisticasSimulacion e;
    pa2m_afirmar(simulador_simular_evento(simulador, FinalizarSimulacion, NULL) == ExitoSimulacion, "Puedo finalizar la simulacion");
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, &e) == ErrorSimulacion, "Luego de finalizar, no puedo seguir simulando");

    simulador_destruir(simulador);
}

void dadoUnSimulador_puedoSeleccionarUnaDificultad()
{
    hospital_t *hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t *simulador = simulador_crear(hospital);

    int id_inexistente = 10;
    int id_correcto = 2;

    pa2m_afirmar(simulador_simular_evento(simulador, SeleccionarDificultad, NULL) == ErrorSimulacion,
                 "No puedo seleccionar una dificulad si el id es NULL");
    pa2m_afirmar(simulador_simular_evento(simulador, SeleccionarDificultad, &id_inexistente) == ErrorSimulacion,
                 "No puedo seleccionar una dificulad si el id no existe");
    pa2m_afirmar(simulador_simular_evento(simulador, SeleccionarDificultad, &id_correcto) == ExitoSimulacion,
                 "Puedo seleccionar una dificulad si el id es correcto");

    pa2m_afirmar(simulador->dificultad_actual == 2, "La dificultad se selecciona correctamente");

    simulador_destruir(simulador);
}

void dadoUnSimulador_puedoObtenerLaDificultadActual()
{
    hospital_t *hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t *simulador = simulador_crear(hospital);

    InformacionDificultad info;
    info.id = 2;

    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info),
                 "Puedo obtener informacion de la dificultad correctamente");

    bool info_es_correcta = info.en_uso == false &&
                            info.id == 2 &&
                            strcmp(info.nombre_dificultad, NOMBRE_DIFICULTAD_2) == 0;

    pa2m_afirmar(info_es_correcta, "La informacion obtenida es correcta");

    info.id = 5;
    pa2m_afirmar(!simulador_simular_evento(simulador, ObtenerInformacionDificultad, &info),
                 "No puedo obtener informacion de una dificultad inexistente");

    pa2m_afirmar(info.id == -1, "La id del dato se cambia a -1 correctamente");

    simulador_destruir(simulador);
}

void dadoUnSimulador_puedoAgregarUnaDificultadNueva()
{
    hospital_t *hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t *simulador = simulador_crear(hospital);

    DatosDificultad datos = {.nombre = "Nombre",
                             .calcular_puntaje = _calcular_puntaje,
                             .verificacion_a_string = _verificacion_a_string,
                             .verificar_nivel = _verificar_nivel};

    pa2m_afirmar(simulador_simular_evento(simulador, AgregarDificultad, &datos),
                 "Puedo agregar una dificultad nueva");

    DatosDificultad *dificultad = lista_ultimo(simulador->dificultades);
    bool dificultad_valida = dificultad->calcular_puntaje == datos.calcular_puntaje &&
                             dificultad->verificacion_a_string == datos.verificacion_a_string &&
                             dificultad->verificar_nivel == datos.verificar_nivel &&
                             strcmp(dificultad->nombre, datos.nombre) == 0;

    pa2m_afirmar(dificultad_valida, "La dificultad se agrega correctamente");

    datos.nombre = "Nombre2";

    pa2m_afirmar(simulador_simular_evento(simulador, AgregarDificultad, &datos),
                 "Puedo agregar otra dificultad nueva");

    datos.nombre = "Nombre";

    pa2m_afirmar(!simulador_simular_evento(simulador, AgregarDificultad, &datos),
                 "No puedo agregar una dificultad nueva con un nombre ya existente");

    simulador_destruir(simulador);
}

void dadoUnSimulador_alSimularUnEvento_AumentaLaCantidadDeEventosSimulados()
{
    hospital_t *hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t *simulador = simulador_crear(hospital);

    EstadisticasSimulacion e;
    simulador_simular_evento(simulador, ObtenerEstadisticas, &e);
    pa2m_afirmar(simulador->estadisticas.cantidad_eventos_simulados == 1,
                 "Al simular un evento, aumenta la cantidad de eventos simulados correctamente");

    simulador_simular_evento(simulador, ObtenerEstadisticas, &e);
    simulador_simular_evento(simulador, ObtenerEstadisticas, &e);
    pa2m_afirmar(simulador->estadisticas.cantidad_eventos_simulados == 3,
                 "Al simular dos evento mas, aumenta la cantidad de eventos simulados correctamente");

    simulador_simular_evento(simulador, ObtenerEstadisticas, NULL);
    pa2m_afirmar(simulador->estadisticas.cantidad_eventos_simulados == 4,
                 "Al simular un evento con datos invalidos, la cantidad de eventos simulados aumenta igualmente");

    simulador_destruir(simulador);
}

void dadoUnSimulador_alSimularUnEventoInexistente_NoAumentaLaCantidadDeEventosSimulados()
{
    hospital_t *hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t *simulador = simulador_crear(hospital);

    EstadisticasSimulacion e;
    simulador_simular_evento(simulador, 100, &e);
    pa2m_afirmar(simulador->estadisticas.cantidad_eventos_simulados == 0,
                 "Al simular un evento inexistente, no aumenta la cantidad de eventos simulados correctamente");

    simulador_destruir(simulador);
}

void dadoUnSimulador_puedoAtenderAlProximoEntrenador()
{
    hospital_t *hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t *simulador = simulador_crear(hospital);

    pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL), "Puedo atender al proximo entrenador");
    pa2m_afirmar(heap_cantidad(simulador->pokemones_en_espera) == 3, "Sus pokemones se agregan a la sala de espera correctamente");

    pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL), "Puedo atender al proximo entrenador");
    pa2m_afirmar(heap_cantidad(simulador->pokemones_en_espera) == 7, "Sus pokemones se agregan a la sala de espera correctamente");

    pa2m_afirmar(simulador->estadisticas.entrenadores_atendidos == 2, "La cantidad de entrenadores atendidos aumenta correctamente");

    simulador_destruir(simulador);
}

void dadoUnSimuladorSinMasEntrenadoresPorAtender_noPuedoAtenderAlProximoEntrenador()
{
    hospital_t *hospital = hospital_crear();

    simulador_t *simulador = simulador_crear(hospital);

    pa2m_afirmar(!simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL),
                 "Dado un simulador sin entrenadores restantes, no puedo atender al proximo entrenador");

    simulador_destruir(simulador);
}

void dadoUnSimulador_puedoObtenerInformacionDelPokemonEnTratamiento()
{
    hospital_t *hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t *simulador = simulador_crear(hospital);

    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);
    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

    InformacionPokemon i;
    bool informacion_correcta;

    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &i),
                 "Puedo obtener informacion del pokemon en tratamiento");
    informacion_correcta = strcmp(i.nombre_pokemon, "rampardos") == 0 &&
                           strcmp(i.nombre_entrenador, "lucas") == 0;
    pa2m_afirmar(informacion_correcta, "La informacion obtenida es correcta");

    simulador->pokemon_atendido = heap_extraer(simulador->pokemones_en_espera); //Atiendo el proximo pokemon de la sala de espera

    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &i),
                 "Puedo obtener informacion de proximo pokemon en tratamiento");
    informacion_correcta = strcmp(i.nombre_pokemon, "venonat") == 0 &&
                           strcmp(i.nombre_entrenador, "pucci") == 0;
    pa2m_afirmar(informacion_correcta, "La informacion obtenida es correcta");

    simulador_destruir(simulador);
}

void dadoUnSimuladorSinPokemonesRestantes_noPuedoObtenerInformacionDelPokemonEnTratamiento()
{
    hospital_t *hospital = hospital_crear();

    simulador_t *simulador = simulador_crear(hospital);

    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

    InformacionPokemon i;
    pa2m_afirmar(!simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, &i),
                 "Dado un simulador sin pokemones restantes, no puedo obtener informacion del pokemon en tratamiento");

    simulador_destruir(simulador);
}

void dadoUnSimulador_puedoAdivinarElNivelDelPokemonAtendido()
{
    hospital_t *hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    simulador_t *simulador = simulador_crear(hospital);

    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

    Intento i;
    bool intento_correcto;

    i.nivel_adivinado = 5;
    pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, &i), "Puedo adivinar el nivel del pokemon en tratamiento");
    intento_correcto = i.nivel_adivinado == 5 &&
                       i.es_correcto == false &&
                       strcmp(i.resultado_string, MENSAJE_CERCA) == 0;
    pa2m_afirmar(intento_correcto, "La respueta del simulador es la esperada");

    i.nivel_adivinado = 10;
    pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, &i), "Puedo adivinar nuevamente el nivel del pokemon en tratamiento");
    intento_correcto = i.nivel_adivinado == 10 &&
                       i.es_correcto == true &&
                       strcmp(i.resultado_string, MENSAJE_CORRECTO) == 0;
    pa2m_afirmar(intento_correcto, "La respueta del simulador es la esperada");

    i.nivel_adivinado = 50;
    pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, &i), "Puedo adivinar el nivel del proximo pokemon");
    intento_correcto = i.nivel_adivinado == 50 &&
                       i.es_correcto == false &&
                       strcmp(i.resultado_string, MENSAJE_LEJOS) == 0;
    pa2m_afirmar(intento_correcto, "La respueta del simulador es la esperada");

    int nueva_dificultad = 0;
    simulador_simular_evento(simulador, SeleccionarDificultad, &nueva_dificultad);

    i.nivel_adivinado = 19;
    pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, &i), "Puedo adivinar nuevamente el nivel del pokemon en tratamiento");
    intento_correcto = i.nivel_adivinado == 19 &&
                       i.es_correcto == false &&
                       strcmp(i.resultado_string, MENSAJE_ARRIBA) == 0;
    pa2m_afirmar(intento_correcto, "La respueta del simulador es la esperada (cambiando la dificultad)");

    simulador_destruir(simulador);
}

void dadoUnSimuladorSinPokemonesRestantes_noPuedoAdivinarElNivelDelPokemonAtendido()
{
    hospital_t *hospital = hospital_crear();

    simulador_t *simulador = simulador_crear(hospital);

    Intento i = {.nivel_adivinado = 10};
    pa2m_afirmar(!simulador_simular_evento(simulador, AdivinarNivelPokemon, &i),
                 "Dado un simulador sin pokemones restantes, no puedo adivinar el nivel del proximo pokemon");

    simulador_destruir(simulador);
}

int main()
{
    pa2m_nuevo_grupo("Simulador - Pruebas con NULL");
    dadoUnSimuladorNULL_noPuedoSimular();

    pa2m_nuevo_grupo("Simulador - Crear");
    puedoCrearUnSimulador();

    pa2m_nuevo_grupo("Simulador - ObtenerEstadisticas");
    dadoUnSimulador_puedoObtenerEstadisticas();

    pa2m_nuevo_grupo("Simulador - Finalizar");
    dadoUnSimulador_alFinalizar_noPuedoSimularMas();

    pa2m_nuevo_grupo("Simulador - Seleccionar Dificultad");
    dadoUnSimulador_puedoSeleccionarUnaDificultad();

    pa2m_nuevo_grupo("Simulador - Obtener Dificultad");
    dadoUnSimulador_puedoObtenerLaDificultadActual();

    pa2m_nuevo_grupo("Simulador - Agregar Dificultad");
    dadoUnSimulador_puedoAgregarUnaDificultadNueva();

    pa2m_nuevo_grupo("Simulador - Cantidad de Simulaciones");
    dadoUnSimulador_alSimularUnEvento_AumentaLaCantidadDeEventosSimulados();
    dadoUnSimulador_alSimularUnEventoInexistente_NoAumentaLaCantidadDeEventosSimulados();

    pa2m_nuevo_grupo("Simulador - Atender Entrenador");
    dadoUnSimulador_puedoAtenderAlProximoEntrenador();
    dadoUnSimuladorSinMasEntrenadoresPorAtender_noPuedoAtenderAlProximoEntrenador();

    pa2m_nuevo_grupo("Simulador - Obtener Informacion Pokemon");
    dadoUnSimulador_puedoObtenerInformacionDelPokemonEnTratamiento();
    dadoUnSimuladorSinPokemonesRestantes_noPuedoObtenerInformacionDelPokemonEnTratamiento();

    pa2m_nuevo_grupo("Simulador - Adivinar Nivel Pokemon");
    dadoUnSimulador_puedoAdivinarElNivelDelPokemonAtendido();
    dadoUnSimuladorSinPokemonesRestantes_noPuedoAdivinarElNivelDelPokemonAtendido();

    return pa2m_mostrar_reporte();
}
