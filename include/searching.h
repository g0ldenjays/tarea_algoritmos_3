/**
 * @file searching.h
 * @brief Algoritmo de busqueda sobre arreglos de deportistas.
 */

#ifndef SEARCHING_H
#define SEARCHING_H

#include "deportista.h"

/**
 * @brief Busca recursivamente un deportista por ID en un arreglo ordenado por ID ascendente.
 *
 * @param deportistas Arreglo de deportistas ordenado por ID.
 * @param left Indice inicial del rango de busqueda.
 * @param right Indice final del rango de busqueda.
 * @param targetId ID del deportista que se desea buscar.
 * @return int Indice encontrado o -1 si no existe.
 */
int binary_search_by_id(Deportista *deportistas, int left, int right, int targetId);

#endif
