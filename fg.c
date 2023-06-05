#include "headers.h"
#include "bg.h"

extern bg_proc bg_processes[];

void fg(char input[])
{
    input = strtok(NULL, "\t ");
    if (input == NULL)
    {
        printf("\033[1;31mfg: job number expected\033[0m\n");

        return;
    }

    int job = atoi(input);
    input = strtok(NULL, "\t ");
    if (input != NULL)
    {
        printf("\033[1;31mfg: too many arguments\033[0m\n");
        return;
    }

    if (job <= 0 || bg_processes[job - 1].pid == -__INT_MAX__)
    {
        printf("\033[1;31mfg: invalid job number\033[0m\n");
        return;
    }

    setpgid(bg_processes[job - 1].pid, getpgid(0));

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    tcsetpgrp(0, bg_processes[job - 1].pid);

    kill(bg_processes[job - 1].pid, SIGCONT);

    int status;
    waitpid(bg_processes[job - 1].pid, &status, WUNTRACED);

    tcsetpgrp(0, getpgid(0));

    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    bg_processes[job - 1].pid = -__INT_MAX__;
}