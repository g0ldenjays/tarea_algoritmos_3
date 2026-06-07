/**
 * @file searching.c
 * @brief Implementacion del algoritmo de busqueda binaria sobre deportistas.
 */

#include "searching.h"

/**
 * @brief Busca recursivamente un puntaje exacto en un arreglo ordenado por puntaje ascendente.
 *
 * @param deportistas Arreglo ordenado por puntaje ascendente.
 * @param left Indice izquierdo del rango.
 * @param right Indice derecho del rango.
 * @param tagetId ID a buscar.
 * @return Indice donde se encuentra un elemento con puntaje == targetScore, -1 si no existe.
 */
int binary_search_by_id(Deportista *deportistas, int left, int right, int targetId)
{
	if(deportistas == NULL || left > right) {
		return -1;
	}

	int mid = left + (right - left) / 2;

	if(deportistas[mid] == NULL) {
		return -1;
	}

	if(deportistas[mid]->id == targetId) {
		return mid;
	}

	if(deportistas[mid]->id < targetId) {
		return binary_search_by_id(deportistas, mid + 1, right, targetId);
	}

	return binary_search_by_id(deportistas, left, mid - 1, targetId);
}
