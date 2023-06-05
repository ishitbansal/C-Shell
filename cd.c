#include "headers.h"
#include "cd.h"

extern char previous_directory[];
extern char current_directory[];
extern char home[];

void cd(char input[])
{
    char directory[1000];

    input = strtok(NULL, "\t ");

    if (input == NULL) // if only cd is given
    {
        strcpy(directory, home);
    }
    else
    {
        strcpy(directory, input);
    }

    // check if there are more arguments
    input = strtok(NULL, "\t ");
    if (input != NULL)
    {
        printf("\033[1;31mcd: too many arguments\033[0m\n");
        return;
    }

    // if '-'
    if (strcmp(directory, "-") == 0)
    {
        printf("%s\n", previous_directory);
        if (chdir(previous_directory) == 0)
        {
            char temp[1000];
            strcpy(temp, previous_directory);
            strcpy(previous_directory, current_directory);
            strcpy(current_directory, temp);
        }
        return;
    }

    char path[1000];

    // if directory starts with '~'
    if (strncmp(directory, "~", 1) == 0)
    {
        strcpy(path, home);
        strcat(path, &directory[1]);
    }
    else
    {
        strcpy(path, directory);
    }

    if (strcmp(current_directory, home) == 0 && strcmp(directory, "..") == 0)
    {
        printf("%s\n", current_directory);
    }

    if (chdir(path) == 0)
    {
        strcpy(previous_directory, current_directory);
        getcwd(current_directory, 1000);
    }
    else // directory does not exist
    {
        if (access(path, F_OK) == 0) // checks if file exists or not
        {
            printf("\033[1;31mcd: %s: Not a directory\033[0m\n", path);
        }
        else
        {
            printf("\033[1;31mcd: %s: No such file or directory\033[0m\n", path);
        }
    }
}