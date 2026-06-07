#include "pd.h"

#include <stdio.h>
#include <stdlib.h>

static float max_float(float a, float b)
{
    if (a > b){
        return a;
    }else{
        return b;
    }
}

static float solve_memo(Deportista *deportistas, int index, int presupuesto, float **memo)
{
    if (index == 0 || presupuesto == 0) {
        return 0.0f;
    }

    if (memo[index][presupuesto] >= 0.0f) {
        return memo[index][presupuesto];
    }

    Deportista actual = deportistas[index-1];

    float no_incluir = solve_memo(deportistas, index-1, presupuesto, memo);
    float incluir = 0.0f;

    if (actual->costo <= presupuesto) {
        incluir = solve_memo(deportistas, index-1, presupuesto-actual->costo, memo) + actual->puntaje;
    }

    memo[index][presupuesto] = max_float(no_incluir, incluir);

    return memo[index][presupuesto];
}

static float **create_tab_table(int rows, int cols)
{
    float **table = malloc(sizeof(float *) * rows);

    if (table == NULL) {
        return NULL;
    }

    for (int i=0; i<rows; i++) {
        table[i] = calloc(cols, sizeof(float));

        if (table[i] == NULL) {
            for (int j=0; j<i; j++) {
                free(table[j]);
            }

            free(table);
            return NULL;
        }
    }

    return table;
}

static float **create_memo_table(int rows, int cols)
{
    float **table = malloc(sizeof(float *) * rows);

    if (table == NULL) {
        return NULL;
    }

    for (int i=0; i<rows; i++) {
        table[i] = malloc(sizeof(float) * cols);

        if (table[i] == NULL) {
            for (int j=0; j<i; j++) {
                free(table[j]);
            }

            free(table);
            return NULL;
        }

        for (int j=0; j<cols; j++) {
            table[i][j] = -1.0f;
        }
    }

    return table;
}

static void free_dp_table(float **table, int rows)
{
    if (table == NULL) {
        return;
    }

    for (int i=0; i<rows; i++) {
        free(table[i]);
    }

    free(table);
}

ResultadoPD seleccionar_equipo_pd_tabulation(Deportista *deportistas, int cantidad, int presupuesto)
{
    ResultadoPD resultado;
    float **dp;

    resultado.seleccionados = NULL;
    resultado.cantidad = 0;
    resultado.costo_total = 0;
    resultado.puntaje_total = 0.0f;

    if (deportistas == NULL || cantidad <= 0 || presupuesto <= 0) {
        return resultado;
    }

    dp = create_tab_table(cantidad + 1, presupuesto + 1);

    if(dp == NULL) {
        return resultado;
    }
    
    for (int i=1; i<=cantidad; i++) {
        int costo = deportistas[i-1]->costo;
        float puntaje = deportistas[i-1]->puntaje;

        for (int p=0; p<=presupuesto; p++) {
            dp[i][p] = dp[i-1][p];

            if (costo <= p) {
                float incluir = dp[i-1][p-costo]+puntaje;
                dp[i][p] = max_float(dp[i][p], incluir);
            }
        }
    }
    resultado.puntaje_total = dp[cantidad][presupuesto];
    resultado.seleccionados = malloc(sizeof(Deportista) * cantidad);

    if (resultado.seleccionados == NULL) {
        free_dp_table(dp, cantidad + 1);
        resultado.puntaje_total = 0.0f;
        return resultado;
    }

    int p = presupuesto;

    for (int i=cantidad; i>0; i--) {
        if (dp[i][p] > dp[i-1][p]) {
            Deportista seleccionado = deportistas[i-1];

            resultado.seleccionados[resultado.cantidad] = seleccionado;
            resultado.cantidad++;
            resultado.costo_total += seleccionado->costo;

            p -= seleccionado->costo;
        }
    }

    free_dp_table(dp, cantidad+1);
    return resultado;
}

ResultadoPD seleccionar_equipo_pd_memoization(Deportista *deportistas, int cantidad, int presupuesto)
{
    ResultadoPD resultado;
    float **memo;

    resultado.seleccionados = NULL;
    resultado.cantidad = 0;
    resultado.costo_total = 0;
    resultado.puntaje_total = 0.0f;

    if (deportistas == NULL || cantidad <= 0 || presupuesto <= 0) {
        return resultado;
    }

    memo = create_memo_table(cantidad+1, presupuesto+1);

    if (memo == NULL) {
        return resultado;
    }

    resultado.puntaje_total = solve_memo(deportistas, cantidad, presupuesto, memo);

    resultado.seleccionados = malloc(sizeof(Deportista) * cantidad);

    if (resultado.seleccionados == NULL) {
        free_dp_table(memo, cantidad+1);
        resultado.puntaje_total = 0.0f;
        return resultado;
    }

    int p = presupuesto;

    for (int i=cantidad; i>0; i--) {
        Deportista actual = deportistas[i-1];

        float sin_actual = solve_memo(deportistas, i-1, p, memo);

        if (actual->costo <= p) {
            float con_actual = solve_memo(deportistas, i-1, p-actual->costo, memo) + actual->puntaje;
            if (con_actual > sin_actual) {
                resultado.seleccionados[resultado.cantidad] = actual;
                resultado.cantidad++;
                resultado.costo_total += actual->costo;
                p -= actual->costo;
            }
        }
    }

    free_dp_table(memo, cantidad+1);
    return resultado;
}

void free_resultado_pd(ResultadoPD resultado)
{
    free(resultado.seleccionados);
}

void print_resultado_pd(ResultadoPD resultado)
{ 
    printf("Equipo seleccionado por programacion dinamica:\n");
    printf("Cantidad: %d\n", resultado.cantidad);
    printf("Costo total: %d\n", resultado.costo_total);
    printf("Puntaje total: %.2f\n\n", resultado.puntaje_total);

    for (int i=0; i<resultado.cantidad; i++){
        print_deportista(resultado.seleccionados[i]);
    }
}