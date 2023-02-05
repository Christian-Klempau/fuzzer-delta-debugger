// boolean type
#include <stdbool.h>

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// to wait for child
#include <sys/wait.h>

// to redirect to /dev/null
#include <sys/stat.h>
#include <fcntl.h>

// to get time(), for random seed
#include <time.h>

int MAX_INT = 1;
int MAX_INT_STEP = 1;

int MAX_LINES = 1000;
int N_CLAUSES = 1;
int N_LINES_STEP = 1;

int MAX_VARS = 100;
int N_VARS = 1;
int N_VARS_STEP = 1;

int which = 0;
int reducer = 0;

int step_state_v1()
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

    return 0;
};

int step_state_v2()
{
    if (reducer == 100)
    {
        if (which == 0)
        {
            N_CLAUSES = N_CLAUSES + N_LINES_STEP;
        }
        else if (which == 1)
        {
            N_VARS = N_VARS + N_VARS_STEP;
        }
        else if (which == 2)
        {
            MAX_INT = MAX_INT + MAX_INT_STEP;
        };

        reducer = 0;
    };

    reducer++;
    which = (which + 1) % 3;

    return 0;
};

void print_state()
{
    printf("N_LINES: %d N_VARS: %d MAX_INT: %d\n", N_CLAUSES, N_VARS, MAX_INT);
    printf("Which: %d Reducer %d\n", which, reducer);
}

int random2(int min, int max)
{
    return rand() % (max - min + 1) + min; // 0 to max, inclusive
}

int random3(int max)
{
    // return a random int between 0 and max
    // Original source: https://gist.github.com/lucis/96dc883af28c037104ef42ecedb8c3bf

    if (max <= 0)
    {
        return 1;
    }

    max++;

    static int z1 = 12345; // 12345 for rest of zx
    static int z2 = 12345; // 12345 for rest of zx
    static int z3 = 12345; // 12345 for rest of zx
    static int z4 = 12345; // 12345 for rest of zx

    int b;

    b = (((z1 << 6) ^ z1) >> 13);
    z1 = (((z1 & 4294967294) << 18) ^ b);
    b = (((z2 << 2) ^ z2) >> 27);
    z2 = (((z2 & 4294967288) << 2) ^ b);
    b = (((z3 << 13) ^ z3) >> 21);
    z3 = (((z3 & 4294967280) << 7) ^ b);
    b = (((z4 << 3) ^ z4) >> 12);
    z4 = (((z4 & 4294967168) << 13) ^ b);

    // if we have an argument, then we can use it
    int rand = ((z1 ^ z2 ^ z3 ^ z4)) % max;

    if (rand < 0)
    {
        rand = rand * -1;
    }

    return rand;
}

void print_array_int(int *data, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", ((int *)data)[i]);
    }
    printf("\n");
}

typedef struct clause
{
    int weight;     // 0 is hard clause, >= 1 is soft clause
    int *variables; // list of ints, negatives are negated variables
    int n_variables;
} Clause;

Clause *generate_lines()
{

    Clause *clauses = malloc(N_CLAUSES * sizeof(Clause));

    for (int i = 0; i < N_CLAUSES; i++)
    {
        clauses[i].weight = random2(0, 1) ? 0 : random2(1, MAX_INT); // 0 is hard clause, >= 1 weight is soft clause
        clauses[i].n_variables = random2(0, N_VARS);
        clauses[i].variables = malloc(clauses[i].n_variables * sizeof(int));

        for (int var_idx = 0; var_idx < clauses[i].n_variables; var_idx++)
        {
            clauses[i].variables[var_idx] = random2(1, MAX_INT) * (random2(0, 1) ? 1 : -1);
        };
    };

    return clauses;
};

void *free_lines(Clause *lines)
{
    for (int i = 0; i < N_CLAUSES; i++)
    {
        free(lines[i].variables);
    }
    free(lines);
    return NULL;
};

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

int execute_solver(const char *file_name)
{
    // Based on: https://stackoverflow.com/questions/2667095/how-to-get-the-return-value-of-a-program-ran-via-calling-a-member-of-the-exec-fa

    int child_pid = fork();
    char *solver = "./solver_data/parser";
    char *file = "./test.txt";
    // char *remove_outputs = "> /dev/null 2>&1";
    //  if process is child
    if (child_pid == 0)
    {
        // run solver
        int fd = open("/dev/null", O_WRONLY | O_CREAT, 0666);
        dup2(fd, 1); // make stdout go to file
        dup2(fd, 2); // make stderr go to file - you may choose to not do this
        execl(solver, solver, file, NULL);
        return EXIT_FAILURE;
    };

    int status;
    if (waitpid(child_pid, &status, 0) == -1)
    {
        perror("waitpid failed");
        return EXIT_FAILURE;
    };
    if (WIFEXITED(status))
    {
        const int exit_status = WEXITSTATUS(status);
        return exit_status;
    };

    return EXIT_FAILURE;
};

void fuzz()
{
    int time_seed = time(NULL);
    srand(time_seed);

    const char *file_name2 = tmpnam(NULL); // Get temp name
    const char *file_name = "test.txt";    // Get temp name
    FILE *fp = fopen(file_name, "w");
    printf("file name: %s\n", file_name);
    printf("Seed: %d\n", time_seed);

    int *found_statuses = calloc(100, sizeof(int));
    int exit_status;

    int END = 1000;
    for (int i = 0; i < END; i++)
    {
        Clause *lines = generate_lines();
        write_lines(fp, lines);
        exit_status = execute_solver(file_name);
        if (found_statuses[exit_status] == 0)
        {
            found_statuses[exit_status] = 1;
            printf("CODE: %d - #iter: %d\n", exit_status, i);
        };

        free_lines(lines);
        fseek(fp, 0, SEEK_SET);
        truncate(file_name, 0);
        step_state_v1();
        // step_state_v2();
    };

    free(found_statuses);
    fclose(fp);
}

int main(int argc, char *argv[])
{

    fuzz();
    return 0;
};
