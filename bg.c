#include "headers.h"
#include "bg.h"

extern bg_proc bg_processes[];

void bg(char input[])
{
    input = strtok(NULL, "\t ");
    if (input == NULL)
    {
        printf("\033[1;31mbg: job number expected\033[0m\n");
        return;
    }
    int job = atoi(input);
    input = strtok(NULL, "\t ");
    if (input != NULL)
    {
        printf("\033[1;31mbg: too many arguments\033[0m\n");
        return;
    }

    if (job <= 0 || bg_processes[job - 1].pid == -__INT_MAX__)
    {
        printf("\033[1;31mbg: invalid job number\033[0m\n");
        return;
    }

    kill(bg_processes[job - 1].pid, SIGCONT);
}