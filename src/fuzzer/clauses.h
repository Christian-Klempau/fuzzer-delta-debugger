#pragma once
#include "state.h"

typedef struct clause
{
    int weight;     // 0 is hard clause, >= 1 is soft clause
    int *variables; // list of ints, negatives are negated variables
    int n_variables;
} Clause;

Clause *generate_lines(float hard_clause_probability, float positive_variable_probability);

void *free_lines(Clause *lines);

void *write_lines(FILE *fp, Clause *lines);
