#include "headers.h"
#include "jobs.h"

extern bg_proc bg_processes[];

int findStatus(int pid)
{
    char pid_stat[100] = "/proc/";

    char pid_[10];
    sprintf(pid_, "%d", pid);

    strcat(pid_stat, pid_);
    strcat(pid_stat, "/stat");

    char temp[100];

    FILE *fp = fopen(pid_stat, "r");
    if (fp == NULL)
    {
        return -1;
    }

    fscanf(fp, "%s", temp);
    fscanf(fp, "%s", temp);
    fscanf(fp, "%s", temp);

    if (strcmp(temp, "T") == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

    fclose(fp);
}

void jobs(char input[])
{
    input = strtok(NULL, "\t ");

    int flag = -1; // 0 for r,1 for s

    if (input != NULL)
    {
        if (strcmp(input, "-r") == 0)
        {
            flag = 0;
        }
        else if (strcmp(input, "-s") == 0)
        {
            flag = 1;
        }
        else
        {
            printf("\033[1;31mjobs: invalid flag\033[0m\n");
            return;
        }

        input = strtok(NULL, "\t ");
        if (input != NULL)
        {
            printf("\033[1;31mjobs: too many arguments\033[0m\n");
            return;
        }
    }

    char commands[100][100];
    int m = 0;

    for (int i = 0; i < 100; i++)
    {
        if (bg_processes[i].pid != -__INT_MAX__)
        {
            strcpy(commands[m++], bg_processes[i].command);
        }
    }

    bubbleSort(commands, m);

    int sorted[m];
    for (int i = 0; i < m; i++)
    {
        for (int j = i; j < 100; j++)
        {
            if (strcmp(bg_processes[j].command, commands[i]) == 0)
            {
                sorted[i] = j;
                break;
            }
        }
    }

    for (int i = 0; i < m; i++)
    {
        int k = sorted[i];
        int status = findStatus(bg_processes[k].pid);

        if (status == -1)
        {
            continue;
        }

        if (flag == 0 && status == 0)
        {
            printf("[%d] Running %s [%d]\n", bg_processes[k].n, bg_processes[k].command, bg_processes[k].pid);
        }
        else if (flag == 1 && status == 1)
        {
            printf("[%d] Stopped %s [%d]\n", bg_processes[k].n, bg_processes[k].command, bg_processes[k].pid);
        }
        else if (flag == -1)
        {
            if (status == 0)
            {
                printf("[%d] Running %s [%d]\n", bg_processes[k].n, bg_processes[k].command, bg_processes[k].pid);
            }
            if (status == 1)
            {
                printf("[%d] Stopped %s [%d]\n", bg_processes[k].n, bg_processes[k].command, bg_processes[k].pid);
            }
        }
    }
}