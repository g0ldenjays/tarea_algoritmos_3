#include "experiment.h"

#include "csv.h"
#include "pd.h"
#include "greedy.h"
#include "errors.h"

/**
 * @brief Calcula la suma total de los costos de un subconjunto de deportistas.
 *
 * @param deportistas Arreglo de deportistas cargados desde el CSV.
 * @param n Cantidad de deportistas que serán considerados en la suma.
 * @return int Suma total de los costos de los primeros n deportistas.
 */
static int sum_costs(Deportista *deportistas, int n)
{
    int total = 0;

    for (int i = 0; i < n; i++) {
        total += deportistas[i]->costo;
    }

    return total;
}

/**
 * @brief Estima la cantidad de celdas utilizadas por la tabla de programación dinámica.
 *
 * @param n Cantidad de deportistas considerados.
 * @param budget Presupuesto máximo usado como restricción.
 * @return long long Cantidad estimada de celdas de la tabla DP.
 */
static long long estimate_dp_cells(int n, int budget)
{
    return (long long)(n + 1) * (long long)(budget + 1);
}

/**
 * @brief Estima la memoria utilizada por la tabla de programación dinámica en bytes.
 *
 * @param n Cantidad de deportistas considerados.
 * @param budget Presupuesto máximo usado como restricción.
 * @return long long Memoria estimada en bytes.
 */
static long long estimate_dp_bytes(int n, int budget)
{
    long long cells = estimate_dp_cells(n, budget);
    return cells * (long long)sizeof(float);
}

/**
 * @brief Calcula el porcentaje de calidad de una solución respecto al óptimo.
 *
 * @param score Puntaje obtenido por el algoritmo evaluado.
 * @param optimal_score Puntaje óptimo usado como referencia.
 * @return double Porcentaje de calidad de la solución. Retorna 0.0 si el óptimo no es válido.
 */
static double quality_percentage(float score, float optimal_score)
{
    if (optimal_score <= 0.0f) {
        return 0.0;
    }

    return ((double)score / (double)optimal_score) * 100.0;
}

/**
 * @brief Calcula la eficiencia de una solución en términos de puntaje por costo.
 *
 * @param score Puntaje total de la solución.
 * @param cost Costo total de la solución.
 * @return double Razón puntaje/costo. Retorna 0.0 si el costo es menor o igual a cero.
 */
static double score_per_cost(float score, int cost)
{
    if (cost <= 0) {
        return 0.0;
    }

    return (double)score / (double)cost;
}

/**
 * @brief Mide experimentalmente el rendimiento de la programación dinámica por tabulación.
 *
 * @param deportistas Arreglo de deportistas disponibles.
 * @param n Cantidad de deportistas que serán considerados.
 * @param budget Presupuesto máximo permitido.
 * @return ExperimentMetrics Estructura con tiempo promedio, puntaje, costo y tamaño del equipo.
 */
static ExperimentMetrics measure_pd_tabulation(Deportista *deportistas, int n, int budget)
{
    ExperimentMetrics metrics;
    metrics.avg_time_sec = 0.0;
    metrics.score = 0.0f;
    metrics.cost = 0;
    metrics.team_size = 0;

    for (int trial = 0; trial < EXPERIMENT_TRIALS; trial++) {
        clock_t start = clock();

        ResultadoPD result = seleccionar_equipo_pd_tabulation(deportistas, n, budget);

        clock_t end = clock();

        metrics.avg_time_sec += (double)(end - start) / CLOCKS_PER_SEC;

        metrics.score = result.puntaje_total;
        metrics.cost = result.costo_total;
        metrics.team_size = result.cantidad;

        free_resultado_pd(result);
    }

    metrics.avg_time_sec /= EXPERIMENT_TRIALS;

    return metrics;
}

/**
 * @brief Mide experimentalmente el rendimiento de la programación dinámica por memoización.
 * 
 * @param deportistas Arreglo de deportistas disponibles.
 * @param n Cantidad de deportistas que serán considerados.
 * @param budget Presupuesto máximo permitido.
 * @return ExperimentMetrics Estructura con tiempo promedio, puntaje, costo y tamaño del equipo.
 */
static ExperimentMetrics measure_pd_memoization(Deportista *deportistas, int n, int budget)
{
    ExperimentMetrics metrics;
    metrics.avg_time_sec = 0.0;
    metrics.score = 0.0f;
    metrics.cost = 0;
    metrics.team_size = 0;

    for (int trial = 0; trial < EXPERIMENT_TRIALS; trial++) {
        clock_t start = clock();

        ResultadoPD result = seleccionar_equipo_pd_memoization(deportistas, n, budget);

        clock_t end = clock();

        metrics.avg_time_sec += (double)(end - start) / CLOCKS_PER_SEC;

        metrics.score = result.puntaje_total;
        metrics.cost = result.costo_total;
        metrics.team_size = result.cantidad;

        free_resultado_pd(result);
    }

    metrics.avg_time_sec /= EXPERIMENT_TRIALS;

    return metrics;
}

/**
 * @brief Mide experimentalmente una estrategia greedy con restricción de presupuesto.
 *
 * @param deportistas Arreglo de deportistas disponibles.
 * @param n Cantidad de deportistas que serán considerados.
 * @param budget Presupuesto máximo permitido.
 * @param strategy Código numérico de la estrategia greedy a evaluar.
 * @return ExperimentMetrics Estructura con tiempo promedio, puntaje, costo y tamaño del equipo.
 */
static ExperimentMetrics measure_greedy_budget(Deportista *deportistas, int n, int budget, int strategy)
{
    ExperimentMetrics metrics;
    metrics.avg_time_sec = 0.0;
    metrics.score = 0.0f;
    metrics.cost = 0;
    metrics.team_size = 0;

    for (int trial = 0; trial < EXPERIMENT_TRIALS; trial++) {
        Deportista *selected = malloc(sizeof(Deportista) * n);

        if (selected == NULL) {
            return metrics;
        }

        int total_cost = 0;
        float total_score = 0.0f;

        clock_t start = clock();

        int selected_count = 0;

        if (strategy == 1) {
            selected_count = greedy_por_puntaje(
                deportistas,
                n,
                budget,
                selected,
                &total_cost,
                &total_score
            );
        } else if (strategy == 2) {
            selected_count = greedy_por_costo(
                deportistas,
                n,
                budget,
                selected,
                &total_cost,
                &total_score
            );
        } else {
            selected_count = greedy_por_razon(
                deportistas,
                n,
                budget,
                selected,
                &total_cost,
                &total_score
            );
        }

        clock_t end = clock();

        metrics.avg_time_sec += (double)(end - start) / CLOCKS_PER_SEC;

        metrics.score = total_score;
        metrics.cost = total_cost;
        metrics.team_size = selected_count;

        free(selected);
    }

    metrics.avg_time_sec /= EXPERIMENT_TRIALS;

    return metrics;
}

/**
 * @brief Mide experimentalmente la estrategia greedy sin restricción de presupuesto.
 *
 * @param deportistas Arreglo de deportistas disponibles.
 * @param n Cantidad de deportistas que serán considerados.
 * @param k Cantidad exacta de deportistas que se desea seleccionar.
 * @return ExperimentMetrics Estructura con tiempo promedio, puntaje, costo y tamaño del equipo.
 */
static ExperimentMetrics measure_greedy_without_budget(Deportista *deportistas, int n, int k)
{
    ExperimentMetrics metrics;
    metrics.avg_time_sec = 0.0;
    metrics.score = 0.0f;
    metrics.cost = 0;
    metrics.team_size = 0;

    for (int trial = 0; trial < EXPERIMENT_TRIALS; trial++) {
        Deportista *selected = malloc(sizeof(Deportista) * k);

        if (selected == NULL) {
            return metrics;
        }

        int total_cost = 0;
        float total_score = 0.0f;

        clock_t start = clock();

        int selected_count = greedy_sin_presupuesto(deportistas, n, k, selected, &total_cost, &total_score);

        clock_t end = clock();

        metrics.avg_time_sec += (double)(end - start) / CLOCKS_PER_SEC;

        metrics.score = total_score;
        metrics.cost = total_cost;
        metrics.team_size = selected_count;

        free(selected);
    }

    metrics.avg_time_sec /= EXPERIMENT_TRIALS;

    return metrics;
}

/**
 * @brief Escribe una fila de resultados experimentales en el archivo CSV.
 *
 * @param file Archivo CSV abierto en modo escritura.
 * @param scenario Escenario evaluado: con presupuesto o sin presupuesto.
 * @param n Cantidad de deportistas considerados.
 * @param k Cantidad exacta de deportistas solicitada, usada en el escenario sin presupuesto.
 * @param budget_ratio Proporción del costo total usada para definir el presupuesto.
 * @param budget Presupuesto máximo utilizado en el experimento.
 * @param algorithm Nombre del algoritmo evaluado.
 * @param metrics Métricas obtenidas durante la ejecución del algoritmo.
 * @param optimal_score Puntaje óptimo de referencia para calcular calidad y brecha.
 * @param estimated_cells Cantidad estimada de celdas usadas por la tabla DP.
 * @param estimated_bytes Memoria estimada en bytes usada por la tabla DP.
 */
static void write_result_row(FILE *file, char *scenario, int n, int k, double budget_ratio, int budget, char *algorithm, ExperimentMetrics metrics, float optimal_score, long long estimated_cells, long long estimated_bytes)
{
    double gap = (double)optimal_score - (double)metrics.score;
    double quality = quality_percentage(metrics.score, optimal_score);
    double efficiency = score_per_cost(metrics.score, metrics.cost);

    fprintf(
        file,
        "%s,%d,%d,%.2f,%d,%s,%d,%.8f,%.2f,%d,%d,%.2f,%.2f,%.2f,%.8f,%lld,%lld\n",
        scenario,
        n,
        k,
        budget_ratio,
        budget,
        algorithm,
        EXPERIMENT_TRIALS,
        metrics.avg_time_sec,
        metrics.score,
        metrics.cost,
        metrics.team_size,
        optimal_score,
        gap,
        quality,
        efficiency,
        estimated_cells,
        estimated_bytes
    );
}

/**
 * @brief Ejecuta la batería completa de experimentos del proyecto.
 *
 * Carga los deportistas desde el archivo CSV principal, evalúa distintos tamaños
 * de entrada y distintos niveles de presupuesto, ejecutando para cada caso los
 * algoritmos de programación dinámica y greedy implementados.
 */
void run_experiments()
{
    int total_count = 0;
    Deportista *deportistas = load_deportistas_array(&total_count);

    if (deportistas == NULL || total_count == 0) {
        print_error(ERROR_NO_DATA_LOADED, "db/deportistas.csv");
        return;
    }

    FILE *file = fopen("./db/experiment_results.csv", "w");

    if (file == NULL) {
        print_error(ERROR_FILE_CREATE_FAILED, "./db/experiment_results.csv");
        free_deportistas_array(deportistas, total_count);
        return;
    }

    fprintf(
        file,
        "scenario,input_n,k,budget_ratio,budget,algorithm,trials,"
        "avg_time_sec,score,cost,team_size,optimal_score,gap_score,"
        "quality_pct,score_per_cost,estimated_space_cells,estimated_space_bytes\n"
    );

    double size_ratios[] = {0.01, 0.05, 0.10, 0.25, 0.50, 0.75, 1.00};
    double budget_ratios[] = {0.25, 0.50, 0.75};

    int size_count = (int)(sizeof(size_ratios) / sizeof(size_ratios[0]));
    int ratio_count = (int)(sizeof(budget_ratios) / sizeof(budget_ratios[0]));

    printf("\nEjecutando experimentos...\n");
    printf("Datos disponibles: %d deportistas\n", total_count);
    printf("Resultados: db/experiment_results.csv\n\n");

    for (int i = 0; i < size_count; i++) {
        int n = (int)(total_count * size_ratios[i]);

        if (n < 10) {
            n = 10;
        }

        if (n > total_count) {
            n = total_count;
        }

        int total_cost = sum_costs(deportistas, n);
        int k_without_budget = n / 2;

        for (int r = 0; r < ratio_count; r++) {
            double ratio = budget_ratios[r];
            int budget = (int)(total_cost * ratio);

            long long dp_cells = estimate_dp_cells(n, budget);
            long long dp_bytes = estimate_dp_bytes(n, budget);

            printf("n=%d | presupuesto=%d | ratio=%.2f\n", n, budget, ratio);

            ExperimentMetrics pd_tab = measure_pd_tabulation(deportistas, n, budget);
            ExperimentMetrics pd_memo = measure_pd_memoization(deportistas, n, budget);

            float optimal_score = pd_tab.score;

            ExperimentMetrics greedy_score = measure_greedy_budget(deportistas, n, budget, 1);
            ExperimentMetrics greedy_cost = measure_greedy_budget(deportistas, n, budget, 2);
            ExperimentMetrics greedy_ratio = measure_greedy_budget(deportistas, n, budget, 3);

            write_result_row(file, "with_budget", n, 0, ratio, budget, "PD_TABULATION", pd_tab, optimal_score, dp_cells, dp_bytes);
            write_result_row(file, "with_budget", n, 0, ratio, budget, "PD_MEMOIZATION", pd_memo, optimal_score, dp_cells, dp_bytes);
            write_result_row(file, "with_budget", n, 0, ratio, budget, "GREEDY_SCORE", greedy_score, optimal_score, 0, 0);
            write_result_row(file, "with_budget", n, 0, ratio, budget, "GREEDY_COST", greedy_cost, optimal_score, 0, 0);
            write_result_row(file, "with_budget", n, 0, ratio, budget, "GREEDY_RATIO", greedy_ratio, optimal_score, 0, 0);
        }

        ExperimentMetrics greedy_no_budget = measure_greedy_without_budget(deportistas, n, k_without_budget);

        write_result_row(file, "without_budget", n, k_without_budget, 0.0, 0, "GREEDY_NO_BUDGET", greedy_no_budget, greedy_no_budget.score, 0, 0);
    }

    fclose(file);
    free_deportistas_array(deportistas, total_count);

    printf("\nExperimentos finalizados.\n");
    printf("CSV generado en: db/experiment_results.csv\n");
}