// boolean type
// #include <stdbool.h>

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

#include "state.h"
#include "clauses.h"
#include "random.h"

void print_array_int(int *data, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", ((int *)data)[i]);
    }
    printf("\n");
}

int execute_solver(const char *file_name)
{
    // Based on: https://stackoverflow.com/questions/2667095/how-to-get-the-return-value-of-a-program-ran-via-calling-a-member-of-the-exec-fa

    int child_pid = fork();
    char *solver = "./solver_data/parser";
    // char *remove_outputs = "> /dev/null 2>&1";
    //  if process is child
    if (child_pid == 0)
    {
        // run solver
        int fd = open("/dev/null", O_WRONLY | O_CREAT, 0666);
        dup2(fd, 1); // make stdout go to file
        dup2(fd, 2); // make stderr go to file - you may choose to not do this
        execl(solver, solver, file_name, NULL);
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
    set_seed(time_seed);

    // const char *file_name = tmpnam(NULL); // Get temp name
    const char *file_name = "test.txt"; // Get temp name
    FILE *fp = fopen(file_name, "w");
    printf("file name: %s\n", file_name);
    printf("Seed: %d\n", time_seed);

    int *found_statuses = calloc(100, sizeof(int));
    int exit_status;

    // 0.0 - 0.5   + v2  : 33, 38
    // 0.5 - 0.5   + v2  : 31, 33, 34

    float hard_clause_prob;
    float positive_variable_prob;

    int N_ITERS;
    for (int type = 1; type <= 3; type++)
    {
        for (int growth = 0; growth < 3; growth++)
        {
            N_ITERS = 1000 + 4000 * (growth);
            reset_state();
            hard_clause_prob = get_random_uniform();
            positive_variable_prob = get_random_uniform();
            printf("\n-------------- Type: %d - Growth: %d - N_ITERS: %d --------------\n", type, growth, N_ITERS);

            for (int n_iter = 0; n_iter < N_ITERS; n_iter++)
            {
                if (n_iter % 100 == 0)
                {
                    hard_clause_prob = get_random_uniform();
                    positive_variable_prob = get_random_uniform();
                    //printf("\n----- hard prob: %f - positive prob: %f-----\n", hard_clause_prob, positive_variable_prob);
                };
                Clause *lines = generate_lines(hard_clause_prob, positive_variable_prob);
                write_lines(fp, lines);
                exit_status = execute_solver(file_name);
                if (found_statuses[exit_status] == 0)
                {
                    found_statuses[exit_status] = 1;
                    printf("CODE: %d\n", exit_status);
                };

                free_lines(lines);
                fseek(fp, 0, SEEK_SET);
                truncate(file_name, 0);
                advance_state(type);
            };
        };
    }

    free(found_statuses);
    fclose(fp);
}

int main(int argc, char *argv[])
{

    fuzz();
    return 0;
};
