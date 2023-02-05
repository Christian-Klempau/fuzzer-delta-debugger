#include <stdlib.h>

#include "state.h"
#include "random.h"

/*
 * int  weight: 0 is hard clause, >= 1 is soft clause
 * int *variables: list of ints, negatives are negated variables
 * int  n_variables: number of variables in the clause
 */
typedef struct clause
{
    int weight;
    int *variables;
    int n_variables;
} Clause;

/*
 * hard_clause_probability: probability [0, 1] that a clause is hard instead of soft
 * positive_variable_probability: probability [0, 1] that a variable is present instead of negated
 *
 * returns: array of clauses
 */
Clause *generate_lines(float hard_clause_probability, float positive_variable_probability)
{

    Clause *clauses = malloc(N_CLAUSES * sizeof(Clause));

    for (int i = 0; i < N_CLAUSES; i++)
    {

        clauses[i].weight = random_binary_uniform(hard_clause_probability) ? 0 : random_range(1, MAX_INT); // 0 is hard clause, >= 1 weight is soft clause
        clauses[i].n_variables = random_range(0, N_VARS);

        clauses[i].variables = malloc(clauses[i].n_variables * sizeof(int));

        for (int var_idx = 0; var_idx < clauses[i].n_variables; var_idx++)
        {
            clauses[i].variables[var_idx] = random_range(1, MAX_INT) * (random_binary_uniform(positive_variable_probability) ? 1 : -1);
        };
    };

    return clauses;
};

/*
 * Frees all the memory allocated for the lines
 *
 * lines: array of clauses
 *
 * returns: NULL
 */
void *free_lines(Clause *lines)
{
    for (int i = 0; i < N_CLAUSES; i++)
    {
        free(lines[i].variables);
    }
    free(lines);
    return NULL;
};

/*
 * Writes the lines to a file
 *
 * fp: file pointer
 * lines: array of clauses
 *
 * returns: NULL
 */
void *write_lines(FILE *fp, Clause *lines)
{
    for (int i = 0; i < N_CLAUSES; i++)
    {
        if (lines[i].weight == 0)
        {
            fprintf(fp, "h ");
        }
        else
        {
            fprintf(fp, "%d ", lines[i].weight);
        };

        for (int j = 0; j < lines[i].n_variables; j++)
        {
            fprintf(fp, "%d ", lines[i].variables[j]);
        };
        fprintf(fp, "0\n");
    };
    return NULL;
};
