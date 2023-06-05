#include "prompt.h"
#include "headers.h"

extern int printTime;
extern char home[];

void prompt(char directory[])
{
    char relative_directory[1000];

    if (strncmp(directory, home, strlen(home)) == 0)
    {
        strcpy(relative_directory, "~");
        strcat(relative_directory, &directory[strlen(home)]);
    }
    else
    {
        strcpy(relative_directory, directory);
    }

    char *user_name = getlogin();
    char system_name[1000];
    gethostname(system_name, sizeof(system_name));

    printf("<\033[1;32m%s@%s\033[0m", user_name, system_name);
    printf(":");
    printf("\033[1;34m%s\033[0m", relative_directory);
    if (printTime > 0) // if foreground process is just completed
    {
        printf("\033[1;36m took %ds\033[0m", printTime);
        printTime = 0;
    }
    printf("> ");
}