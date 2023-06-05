#include "headers.h"
#include "pinfo.h"

extern char home[];

void pinfo(char input[])
{
    input = strtok(NULL, "\t ");

    char pid_path[100] = "/proc/";

    if (input == NULL)
    {
        int pid;
        pid = getpid();
        printf("pid : %d\n", pid);

        strcat(pid_path, "self/");
    }
    else
    {
        char pid_[100];
        strcpy(pid_, input);

        input = strtok(NULL, "\t ");
        if (input != NULL)
        {
            printf("\033[1;31mpinfo: too many arguments\033[0m\n");
            return;
        }

        strcat(pid_path, pid_);
        strcat(pid_path, "/");

        struct stat filestat;
        if (stat(pid_path, &filestat) == -1)
        {
            printf("\033[1;31mpinfo: pid does not exist\033[0m\n");
            return;
        }
        else
        {
            printf("pid : %s\n", pid_);
        }
    }

    char pid_status[100];
    strcpy(pid_status, pid_path);
    strcat(pid_status, "status");

    FILE *fp = fopen(pid_status, "r");

    char temp[100];
    for (int i = 0; i < 2; i++)
    {
        fgets(temp, 100, fp);
    }

    // currently pointer on status line
    fscanf(fp, "%s", temp);
    fscanf(fp, "%s", temp);

    char state[10];
    strcpy(state, temp); // state store process status

    fgets(temp, 100, fp); // pointer moves to next line (tgid)

    for (int i = 0; i < 14; i++)
    {
        fgets(temp, 100, fp);
    }
    // pointer on VMSize line
    fscanf(fp, "%s", temp);
    fscanf(fp, "%s", temp); // pointer on VMSize val (in kb)

    char vm[20];
    strcpy(vm, temp);

    fclose(fp);

    // comparing pgrp and tpgid for checking if the process is foreground or background from /proc/pid/stat
    char pid_stat[100];
    strcpy(pid_stat, pid_path);
    strcat(pid_stat, "stat");

    fp = fopen(pid_stat, "r");

    for (int i = 0; i < 4; i++)
    {
        fscanf(fp, "%s", temp);
    }
    fscanf(fp, "%s", temp); // 5th argument
    char pgrp[100];
    strcpy(pgrp, temp);

    for (int i = 0; i < 2; i++)
    {
        fscanf(fp, "%s", temp);
    }
    fscanf(fp, "%s", temp); // 8th argument
    char tpgid[100];
    strcpy(tpgid, temp);

    if (strcmp(pgrp, tpgid) == 0)
    {
        state[1] = '+';
        state[2] = '\0';
    }

    fclose(fp);

    // executable path from proc/pid/exe
    char pid_exe[100];
    strcpy(pid_exe, pid_path);
    strcat(pid_exe, "exe");

    char executable_path[100];
    int n = readlink(pid_exe, executable_path, 100);
    executable_path[n] = '\0';

    printf("process status : %s\n", state);
    printf("memory : %s\n", vm);
    if (strncmp(executable_path, home, strlen(home)) == 0)
    {
        char relative_executable_path[100];
        strcpy(relative_executable_path, "~");
        strcat(relative_executable_path, &executable_path[strlen(home)]);
        printf("executable path : %s\n", relative_executable_path);
    }
    else
    {
        printf("executable path : %s\n", executable_path);
    }
}