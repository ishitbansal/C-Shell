#include "headers.h"
#include "discover.h"

extern char home[];

void printRelativePath(char path[], int relative)
{
    char relative_path[1000];
    if (strncmp(path, home, strlen(home)) == 0 && relative == 1)
    {
        strcpy(relative_path, "~");
        strcat(relative_path, &path[strlen(home)]);
        printf("%s\n", relative_path);
    }
    else
    {
        printf("%s\n", path);
    }
}

void discover_(char directory[], char file[], char flags[], int relative)
{
    struct dirent *dir;
    DIR *dh = opendir(directory);
    if (dh == NULL)
    {
        return;
    }

    if ((strlen(file) == 0 && strcmp(flags, "f") != 0))
    {
        printRelativePath(directory, relative);
    }

    while ((dir = readdir(dh)) != NULL)
    {
        if (dir->d_name[0] == '.')
        {
            continue;
        }

        char new_path[500];
        strcpy(new_path, directory);
        if (new_path[strlen(new_path) - 1] != '/')
        {
            strcat(new_path, "/");
        }
        strcat(new_path, dir->d_name);

        struct stat filestat;
        stat(new_path, &filestat);
        if (S_ISDIR(filestat.st_mode))
        {
            if (strlen(file) != 0 && strcmp(file, dir->d_name) == 0 && strcmp(flags, "f") != 0)
            {
                printRelativePath(new_path, relative);
                continue;
            }
            discover_(new_path, file, flags, relative);
        }
        else if (strcmp(flags, "d") != 0)
        {
            if (strlen(file) == 0)
            {
                printRelativePath(new_path, relative);
            }
            else if (strcmp(file, dir->d_name) == 0)
            {
                printRelativePath(new_path, relative);
            }
        }
    }
    closedir(dh);
}

void discover(char input[])
{
    char arguments[5][100];
    int i = 0;
    input = strtok(NULL, "\t ");

    while (input != NULL)
    {
        strcpy(arguments[i++], input);
        input = strtok(NULL, "\t ");

        if (i > 4)
        {
            printf("\033[1;31mdiscover: too many arguments\033[0m\n");
            return;
        }
    }

    char target_directory[100];
    int flag1 = 0;
    int flag2 = 0;
    int relative = 0;
    char file[100];

    strcpy(target_directory, ".");
    strcpy(file, "\0");

    int x = 0;
    int y = 0;

    for (int j = 0; j < i; j++)
    {
        if (arguments[j][0] == '-')
        {
            if (strcmp(arguments[j], "-d") == 0)
            {
                flag1 = 1;
            }
            else if (strcmp(arguments[j], "-f") == 0)
            {
                flag2 = 1;
            }
            else
            {
                printf("\033[1;31mdiscover: invalid flag\033[0m\n");
                return;
            }
        }
        else if (arguments[j][0] == '"')
        {
            if (y > 0)
            {
                printf("\033[1;31mdiscover: multiple files given\033[0m\n");
                return;
            }
            strcpy(file, &arguments[j][1]);
            file[strlen(file) - 1] = '\0';
            y++;
        }
        else
        {
            if (x > 0)
            {
                printf("\033[1;31mdiscover: multiple target directories given\033[0m\n");
                return;
            }
            if (arguments[j][0] == '~')
            {
                strcpy(target_directory, home);
                strcat(target_directory, &arguments[j][1]);
                relative = 1;
            }
            else
            {
                strcpy(target_directory, arguments[j]);
            }
            x++;
        }
    }

    // only one flag
    if (flag1 ^ flag2 == 1)
    {
        // -d
        if (flag1 == 1)
        {
            discover_(target_directory, file, "d", relative);
        }

        // -f
        else if (flag2 == 1)
        {
            discover_(target_directory, file, "f", relative);
        }
    }

    // both flags -d -f
    else
    {
        discover_(target_directory, file, "df", relative);
    }
}