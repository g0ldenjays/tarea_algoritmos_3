/**
 * @file greedy.h
 * @brief Estrategias greedy para seleccion de equipo con restriccion de presupuesto.
 */

#ifndef GREEDY_H
#define GREEDY_H

#include "deportista.h"

/**
 * @brief Estrategia greedy: prioriza deportistas con mayor puntaje.
 *
 * Ordena por puntaje descendente y selecciona
 * mientras el presupuesto lo permita.
 *
 * @param deportistas Arreglo de deportistas disponibles.
 * @param cantidad Cantidad de deportistas en el arreglo.
 * @param presupuesto Presupuesto maximo disponible.
 * @param seleccionados Arreglo de salida (tamanio = cantidad), ya alocado por el llamador.
 * @param costo_total Variable de salida con el costo total usado.
 * @param puntaje_total Variable de salida con el puntaje total obtenido.
 * @return int Cantidad de deportistas seleccionados.
 */
int greedy_por_puntaje(Deportista *deportistas, int cantidad, int presupuesto, Deportista *seleccionados, int *costo_total, float *puntaje_total);

/**
 * @brief Estrategia greedy: prioriza deportistas con menor costo.
 *
 * Ordena (copia interna) por costo ascendente, maximizando la cantidad
 * de deportistas que entran en el presupuesto.
 */
int greedy_por_costo(Deportista *deportistas, int cantidad, int presupuesto, Deportista *seleccionados, int *costo_total, float *puntaje_total);

/**
 * @brief Estrategia greedy: prioriza la mejor razon puntaje/costo.
 *
 * Ordena (copia interna) por la razon puntaje/costo descendente.
 */
int greedy_por_razon(Deportista *deportistas, int cantidad, int presupuesto, Deportista *seleccionados, int *costo_total, float *puntaje_total);

/**
 * @brief Imprime el resultado de una estrategia greedy.
 *
 * @param seleccionados Arreglo de deportistas seleccionados.
 * @param cantidad Cantidad de deportistas seleccionados.
 * @param costo_total Costo total utilizado.
 * @param puntaje_total Puntaje total obtenido.
 * @param nombre_estrategia Nombre descriptivo de la estrategia (para el encabezado).
 */
void print_resultado_greedy(Deportista *seleccionados, int cantidad, int costo_total, float puntaje_total, const char *nombre_estrategia);

#endif