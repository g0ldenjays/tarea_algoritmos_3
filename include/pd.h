#ifndef PD_H
#define PD_H

#include "deportista.h"

typedef struct ResultadoPD {
    Deportista *seleccionados;
    int cantidad;
    int costo_total;
    float puntaje_total;
} ResultadoPD;

ResultadoPD seleccionar_equipo_pd_tabulation(Deportista *deportistas, int cantidad, int presupuesto);

ResultadoPD seleccionar_equipo_pd_memoization(Deportista *deportistas, int cantidad, int presupuesto);

void free_resultado_pd(ResultadoPD resultado);

void print_resultado_pd(ResultadoPD resultado);

#endif