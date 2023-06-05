#include "headers.h"
#include "background.h"

extern int exit_commands[];
extern bg_proc bg_processes[];
extern int original_output;

void background(char bg_process[])
{
    char *a = strtok(bg_process, "\t ");
    if (a == NULL)
    {
        return;
    }

    char *arguments[10];
    int i = 0;

    while (a != NULL)
    {
        arguments[i++] = a;
        a = strtok(NULL, "\t ");
    }
    arguments[i] = NULL;

    char command[100] = "\0";
    for (int j = 0; j < i; j++)
    {
        strcat(command, arguments[j]);
        if (j != i - 1)
        {
            strcat(command, " ");
        }
    }

    int forkReturn = fork();

    if (forkReturn == 0)
    {
        setpgrp();
        int val = execvp(arguments[0], arguments);
        if (val < 0)
        {
            char temp[100];
            sprintf(temp, "\033[1;31m%s: command not found\033[0m\n", arguments[0]);
            write(original_output, temp, strlen(temp));

            exit(0);
        }
    }
    else
    {
        int index = -1;
        for (int j = 0; j < 100; j++)
        {
            if (bg_processes[j].pid != -__INT32_MAX__)
            {
                index = j;
            }
        }

        index++;
        bg_processes[index].pid = forkReturn;
        bg_processes[index].n = index + 1;

        char temp[100];
        sprintf(temp, "[%d]  %d\n", bg_processes[index].n, forkReturn);
        write(original_output, temp, strlen(temp));

        strcpy(bg_processes[index].command, command);
    }
}

void initializeBg()
{
    for (int i = 0; i < 100; i++)
    {
        bg_processes[i].n = i + 1;
        bg_processes[i].pid = -__INT32_MAX__;
    }
}

void initializeExitCommands()
{
    for (int i = 0; i < 100; i++)
    {
        exit_commands[i] = -1;
    }
}

void checkExitStatus()
{
    int status;
    int pid = waitpid(-1, &status, WNOHANG);

    int flag = 0;
    if (WIFEXITED(status) != 1)
    {
        flag = 1;
    }

    for (int i = 0; i < 100; i++)
    {
        if (bg_processes[i].pid == pid)
        {
            for (int j = 0; j < 100; j++)
            {
                if (exit_commands[j] == -1)
                {
                    exit_commands[j] = i;
                    bg_processes[i].check = flag;
                    break;
                }
            }
            break;
        }
    }
}

void printExitCommands()
{
    for (int i = 0; i < 100; i++)
    {
        if (exit_commands[i] != -1)
        {
            printf("%s with pid = %d exited ", bg_processes[exit_commands[i]].command, bg_processes[exit_commands[i]].pid);
            if (bg_processes[exit_commands[i]].check == 1)
            {
                printf("abnormally\n");
            }
            else
            {
                printf("normally\n");
            }
            bg_processes[exit_commands[i]].pid = -__INT32_MAX__;
            bg_processes[exit_commands[i]].check = 0;
            exit_commands[i] = -1;
        }
    }
}
