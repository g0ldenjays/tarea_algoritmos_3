/**
 * @file main.c
 * @brief Punto de entrada de la version minima del proyecto.
 */

#include "base.h"
#include "pd.h"

static void print_help(const char *programName)
{
    printf("Uso:\n");
    printf("  %s -g <cantidad>      Genera deportistas aleatorios en db/deportistas.csv\n", programName);
    printf("  %s -i <id>            Busca un deportista por ID usando busqueda binaria\n", programName);
    printf("  %s -r <cantidad>      Muestra ranking ordenado por puntaje usando Quick Sort\n", programName);
    printf("  %s -t <presupuesto>   Ejecuta seleccion por programacion dinamica con tabulacion\n", programName);
    printf("  %s -m <presupuesto>   Ejecuta seleccion por programacion dinamica con memoizacion\n", programName);
    printf("  %s -h                 Muestra esta ayuda\n", programName);
}

static void search_by_id(int targetId)
{
    int count = 0;
    Deportista *deportistas = load_deportistas_array(&count);

    if(deportistas == NULL || count == 0) {
        print_error(ERROR_NO_DATA_LOADED, CSV_ROUTE);
        return;
    }

    quick_sort_deportistas(deportistas, count, SORT_BY_ID, ASCENDING, PIVOT_LAST);

    int index = binary_search_by_id(deportistas, 0, count - 1, targetId);

    if(index == -1) {
        print_error(ERROR_DEPORTISTA_NOT_FOUND, NULL);
    } else {
        print_deportista(deportistas[index]);
    }

    free_deportistas_array(deportistas, count);
}

static void show_ranking(int rankingAmount)
{
    int count = 0;
    Deportista *deportistas = load_deportistas_array(&count);

    if(deportistas == NULL || count == 0) {
        print_error(ERROR_NO_DATA_LOADED, CSV_ROUTE);
        return;
    }

    if(rankingAmount > count) {
        rankingAmount = count;
    }

    quick_sort_deportistas(deportistas, count, SORT_BY_PUNTAJE, DESCENDING, PIVOT_LAST);
    print_deportistas_array(deportistas, rankingAmount);

    free_deportistas_array(deportistas, count);
}

static void select_team_pd_tabulation(int presupuesto)
{
    int count = 0;
    Deportista *deportistas = load_deportistas_array(&count);

    if (deportistas == NULL || count == 0) {
        print_error(ERROR_NO_DATA_LOADED, CSV_ROUTE);
        return;
    }

    ResultadoPD resultado = seleccionar_equipo_pd_tabulation(deportistas, count, presupuesto);

    print_resultado_pd(resultado);

    free_resultado_pd(resultado);
    free_deportistas_array(deportistas, count);
}

static void select_team_pd_memoization(int presupuesto)
{
    int count = 0;
    Deportista *deportistas = load_deportistas_array(&count);

    if (deportistas == NULL || count == 0) {
        print_error(ERROR_NO_DATA_LOADED, CSV_ROUTE);
        return;
    }

    ResultadoPD resultado = seleccionar_equipo_pd_memoization(deportistas, count, presupuesto);

    print_resultado_pd(resultado);

    free_resultado_pd(resultado);
    free_deportistas_array(deportistas, count);
}

int main(int argc, char **argv)
{
    int opt;

    srand((unsigned int)time(NULL));

    if(argc == 1) {
        print_help(argv[0]);
        return EXIT_SUCCESS;
    }

    while((opt = getopt(argc, argv, "hg:i:r:t:m:")) != -1) {
        switch(opt) {
            case 'h':
                print_help(argv[0]);
                return EXIT_SUCCESS;

            case 'g':
            {
                int dataAmount = atoi(optarg);

                if(MIN_DATA > dataAmount || dataAmount > MAX_DATA) {
                    char detail[128];

                    snprintf(detail, sizeof(detail), "Debe estar entre %d y %d", MIN_DATA, MAX_DATA);
                    print_error(ERROR_INVALID_DATA_AMOUNT, detail);
                    return EXIT_FAILURE;
                }

                printf("\nGenerando" BOLD ORANGE " %d " RESET "datos...\n\n", dataAmount);
                create_deportistas_csv(dataAmount);
                break;
            }

            case 'i':
                search_by_id(atoi(optarg));
                break;

            case 'r':
            {
                int rankingAmount = atoi(optarg);

                if(rankingAmount <= 0) {
                    print_error(ERROR_INVALID_RANKING_AMOUNT, NULL);
                    return EXIT_FAILURE;
                }

                show_ranking(rankingAmount);
                break;
            }

            case 't':
            {
                int presupuesto = atoi(optarg);

                if (presupuesto <= 0) {
                    print_error(ERROR_INVALID_DATA_AMOUNT, "El presupuesto debe ser mayor a 0");
                    return EXIT_FAILURE;
                }

                select_team_pd_tabulation(presupuesto);
                break;
            }

            case 'm':
            {
                int presupuesto = atoi(optarg);

                if (presupuesto <= 0) {
                    print_error(ERROR_INVALID_DATA_AMOUNT, "El presupuesto debe ser mayor a 0");
                    return EXIT_FAILURE;
                }

                select_team_pd_memoization(presupuesto);
                break;
            }

            case '?':
            default:
                print_error(ERROR_UNKNOWN_OPTION, NULL);
                return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
