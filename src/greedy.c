/**
 * @file greedy.c
 * @brief Implementacion de estrategias greedy para seleccion de equipo
 *        con restriccion de presupuesto.
 *
 * Las tres estrategias reutilizan quick_sort_deportistas (sorting.c) con
 * distintos criterios de ordenamiento (SORT_BY_PUNTAJE, SORT_BY_COSTO,
 * SORT_BY_RATIO) y luego recorren el arreglo ordenado seleccionando
 * deportistas mientras el presupuesto lo permita.
 *
 * Ninguna de estas estrategias garantiza la solucion optima; para eso
 * existe la programacion dinamica (pd.c).
 */

#include "greedy.h"
#include "sorting.h"
#include "print_format.h"


/**
 * @brief Funcion auxiliar comun a las tres estrategias.
 *
 * Crea una copia del arreglo de punteros, la ordena segun el criterio
 * y orden indicados, y selecciona codiciosamente mientras el costo
 * acumulado no supere el presupuesto.
 */
int greedy_generico(Deportista *deportistas, int cantidad, int presupuesto, SortCriteria criteria, SortOrder order, Deportista *seleccionados, int *costo_total, float *puntaje_total)
{
    *costo_total = 0;
    *puntaje_total = 0.0f;

    if (deportistas == NULL || cantidad <= 0 || presupuesto <= 0) {
        return 0;
    }

    Deportista *copia = malloc(sizeof(Deportista) * cantidad);

    if (copia == NULL) {
        return 0;
    }

    memcpy(copia, deportistas, sizeof(Deportista) * cantidad);

    quick_sort_deportistas(copia, cantidad, criteria, order, PIVOT_LAST);

    int n = 0;

    for (int i = 0; i < cantidad; i++) {
        Deportista actual = copia[i];

        if (*costo_total + actual->costo <= presupuesto) {
            seleccionados[n] = actual;
            n++;
            *costo_total += actual->costo;
            *puntaje_total += actual->puntaje;
        }
    }

    free(copia);
    return n;
}

int greedy_por_puntaje(Deportista *deportistas, int cantidad, int presupuesto, Deportista *seleccionados, int *costo_total, float *puntaje_total)
{
    return greedy_generico(deportistas, cantidad, presupuesto, SORT_BY_PUNTAJE, DESCENDING, seleccionados, costo_total, puntaje_total);
}

int greedy_por_costo(Deportista *deportistas, int cantidad, int presupuesto, Deportista *seleccionados, int *costo_total, float *puntaje_total)
{
    return greedy_generico(deportistas, cantidad, presupuesto, SORT_BY_COSTO, ASCENDING, seleccionados, costo_total, puntaje_total);
}

int greedy_por_razon(Deportista *deportistas, int cantidad, int presupuesto, Deportista *seleccionados, int *costo_total, float *puntaje_total)
{
    return greedy_generico(deportistas, cantidad, presupuesto, SORT_BY_RATIO, DESCENDING, seleccionados, costo_total, puntaje_total);
}

void print_resultado_greedy(Deportista *seleccionados, int cantidad, int costo_total, float puntaje_total, const char *nombre_estrategia)
{
    printf("Equipo seleccionado por estrategia greedy (%s):\n", nombre_estrategia);
    printf("Cantidad: %d\n", cantidad);
    printf("Costo total: %d\n", costo_total);
    printf("Puntaje total: %.2f\n\n", puntaje_total);

    for (int i = 0; i < cantidad; i++) {
        print_deportista(seleccionados[i]);
    }
}