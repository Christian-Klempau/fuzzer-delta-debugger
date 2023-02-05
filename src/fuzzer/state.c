#include <assert.h>

#include "state.h"

int MAX_INT;
int MAX_INT_STEP;

int MAX_LINES;
int N_CLAUSES;
int N_LINES_STEP;

int MAX_VARS;
int N_VARS;
int N_VARS_STEP;

int which;
int reducer;

/*
 * Resets the fuzzer state to the initial values
 */
void reset_state()
{
    MAX_INT = 1;
    MAX_INT_STEP = 1;

    MAX_LINES = 1000;
    N_CLAUSES = 1;
    N_LINES_STEP = 1;

    MAX_VARS = 100;
    N_VARS = 1;
    N_VARS_STEP = 1;

    which = 0;
    reducer = 0;

    return;
};

/*
 */
void advance_state(int version)
{
    const int MAX_VERSION = 2;

    // transform this if else into a switch statement
    switch (version)
    {
    case 1:
        step_state_v1();
        break;
    case 2:
        step_state_v2();
        break;
    default:
        assert(1 <= version && version <= MAX_VERSION), "Invalid advance_state version";
        break;
    };

    // make sure we don't have 0's
    if (N_VARS == 0)
        N_VARS = 1;
    if (N_CLAUSES == 0)
        N_CLAUSES = 1;
    if (MAX_INT == 0)
        MAX_INT = 1;

    // advance counter
    reducer++;
    which = (which + 1) % 3;
    return;
};

void step_state_v1()
{
    // advance state
    if (which == 0)
    {
        N_CLAUSES += N_LINES_STEP;
    }
    else if (which == 1)
    {
        N_VARS += N_VARS_STEP;
    }
    else if (which == 2)
    {
        MAX_INT += MAX_INT_STEP;
    };

    if (N_CLAUSES > MAX_LINES)
    {
        N_CLAUSES = MAX_LINES;
        N_LINES_STEP = 0;
    };

    if (N_VARS > MAX_VARS)
    {
        N_VARS = MAX_VARS;
        N_VARS_STEP = 0;
    };

    // limit state growth
    if (reducer == 100)
    {
        if (which == 0)
        {
            N_CLAUSES = N_CLAUSES / 2;
        }
        else if (which == 1)
        {
            N_VARS = N_VARS / 2;
        }
        else if (which == 2)
        {
            MAX_INT = MAX_INT / 3;
        };

        reducer = 0;
    };

    return;
};

void step_state_v2()
{
    // advance state
    if (which == 0)
    {
        N_CLAUSES += N_LINES_STEP;
    }
    else if (which == 1)
    {
        N_VARS += N_VARS_STEP;
    }
    else if (which == 2)
    {
        MAX_INT += MAX_INT_STEP;
    };

    if (N_CLAUSES > MAX_LINES)
    {
        N_CLAUSES = MAX_LINES;
        N_LINES_STEP = 0;
    };

    if (N_VARS > MAX_VARS)
    {
        N_VARS = MAX_VARS;
        N_VARS_STEP = 0;
    };

    return;
};

/*
 * Print the current state of the fuzzer
 */
void print_state()
{
    printf("N_LINES: %d N_VARS: %d MAX_INT: %d\n", N_CLAUSES, N_VARS, MAX_INT);
    printf("Which: %d Reducer %d\n", which, reducer);
    return;
};
