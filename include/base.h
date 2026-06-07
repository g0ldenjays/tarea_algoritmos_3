/**
 * @file base.h
 * @brief Constantes globales usadas por la version minima del proyecto.
 */

#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "csv.h"
#include "errors.h"
#include "searching.h"
#include "sorting.h"
#include "print_format.h"

#define NAME_LENGTH_MIN 3
#define NAME_LENGTH_MAX 8
#define MAX_DATA 2147483647
#define MIN_DATA 10
#define MAX_SCORE 100.0
#define MIN_SCORE 0.0
#define MAX_COMPETITIONS 45
#define MIN_COMPETITIONS 1
#define MIN_COSTS 10
#define MAX_COSTS 100
#define CSV_ROUTE "./db/deportistas.csv"

#endif
