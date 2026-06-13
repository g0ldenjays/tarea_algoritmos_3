/**
 * @file sorting.h
 * @brief Ordenamiento para deportistas.
 */

#ifndef SORTING_H
#define SORTING_H

#include "deportista.h"
#include <string.h>
#include <stdlib.h>

/**
 * @brief Criterios disponibles para ordenar deportistas.
 */
typedef enum {
    SORT_BY_ID = 1,
    SORT_BY_PUNTAJE = 2,
    SORT_BY_COMPETENCIAS = 3,
    SORT_BY_NOMBRE = 4,
    SORT_BY_EQUIPO = 5,
    SORT_BY_COSTO = 6,
    SORT_BY_RATIO = 7
} SortCriteria;

/**
 * @brief Sentidos de ordenamiento disponibles.
 */
typedef enum {
    DESCENDING = 1,
    ASCENDING = 0
} SortOrder;

/**
 * @brief Algoritmo de ordenamiento disponible en esta version minima.
 */
typedef enum {
    QUICK_SORT = 1
} SortAlgorithm;

/**
 * @brief Algoritmo de busqueda disponible en esta version minima.
 */
typedef enum {
    BINARY_SEARCH = 1
} SearchAlgorithm;

/**
 * @brief Criterios disponibles para la busqueda.
 */
typedef enum {
    SEARCH_BY_ID = 1
} SearchCriteria;

/**
 * @brief Tipos de pivote disponibles para Quick Sort.
 */
typedef enum {
    PIVOT_LAST = 1,
    PIVOT_FIRST = 2,
    PIVOT_RANDOM = 3,
    PIVOT_MEDIAN_OF_THREE = 4
} PivotType;

int compare_by_criteria(Deportista left, Deportista right, SortCriteria criteria);
void quick_sort_deportistas(Deportista *deportistas, int count, SortCriteria criteria, SortOrder order, PivotType pivotType);

#endif
