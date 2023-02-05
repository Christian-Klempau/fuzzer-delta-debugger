#pragma once
#include <stdio.h>

extern int MAX_INT;
extern int MAX_INT_STEP;

extern int MAX_LINES;
extern int N_CLAUSES;
extern int N_LINES_STEP;

extern int MAX_VARS;
extern int N_VARS;
extern int N_VARS_STEP;

extern int which;
extern int reducer;

void reset_state();
void advance_state(int version);
void step_state_v1();
void step_state_v2();
void print_state();