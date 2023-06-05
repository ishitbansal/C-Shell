#include "headers.h"
#include "sig.h"

extern bg_proc bg_processes[];

void sig(char input[])
{
    input = strtok(NULL, "\t ");
    if (input == NULL)
    {
        printf("\033[1;31msig: job number expected\n\033[0m");
        return;
    }
    int job = atoi(input);
    input = strtok(NULL, "\t ");
    if (input == NULL)
    {
        printf("\033[1;31msig: signal number expected\n\033[0m");
        return;
    }
    int signal = atoi(input);
    input = strtok(NULL, "\t ");
    if (input != NULL)
    {
        printf("\033[1;31msig: too many arguments\n\033[0m");
        return;
    }

    if (job <= 0 || bg_processes[job - 1].pid == -__INT_MAX__)
    {
        printf("\033[1;31msig: job number does not exist \n\033[0m");
        return;
    }

    if (kill(bg_processes[job - 1].pid, signal) < 0)
    {
        printf("\033[1;31msig: invalid signal number\n\033[0m");
    }
}