#include "headers.h"
#include "foreground.h"

extern int printTime;
extern int original_output;
extern bg_proc foreground_process;
extern int fgProc;

void foreground(char initial[], char input[])
{
    char *arguments[10];
    arguments[0] = initial;

    int i = 1;

    input = strtok(NULL, "\t ");

    while (input != NULL)
    {
        arguments[i++] = input;
        input = strtok(NULL, "\t ");
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
    int start = time(NULL);
    int end;

    if (forkReturn == 0)
    {
        int val = execvp(initial, arguments);
        if (val < 0)
        {
            printTime = 0;

            char temp[100];
            sprintf(temp, "\033[1;31m%s: command not found\033[0m\n", arguments[0]);
            write(original_output, temp, strlen(temp));

            exit(0);
        }
    }
    else
    {
        int status;

        foreground_process.n = 1;
        foreground_process.pid = forkReturn;
        strcpy(foreground_process.command, command);

        waitpid(forkReturn, &status, WUNTRACED | WCONTINUED);

        foreground_process.pid = -__INT_MAX__;
        fgProc = 0;

        end = time(NULL);
        if ((end - start) >= 1)
        {
            printTime += end - start;
        }
    }
}