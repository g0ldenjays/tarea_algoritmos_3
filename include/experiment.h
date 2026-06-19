#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EXPERIMENT_TRIALS 10

typedef struct ExperimentMetrics {
    double avg_time_sec;
    float score;
    int cost;
    int team_size;
} ExperimentMetrics;

/**
 * @brief Ejecuta la experimentación del proyecto.
 *
 * Genera un CSV con tiempos, puntajes, costos, cantidad de 
 * seleccionados, calidad relativa y memoria estimada.
 */
void run_experiments(void);

#endif