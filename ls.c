#include "headers.h"
#include "ls.h"

extern char home[];

void bubbleSort(char strings[][100], int n)
{
    char temp[100];
    int swap_count = 0;

    if (n == 1)
    {
        return;
    }

    for (int i = 0; i < n - 1; i = i + 1)
    {
        if (strcmp(&strings[i][0], &strings[i + 1][0]) < 0)
        {
            continue;
        }
        else
        {
            strcpy(temp, &strings[i][0]);
            strcpy(&strings[i][0], &strings[i + 1][0]);
            strcpy(&strings[i + 1][0], temp);
            ++swap_count;
        }
    }

    if (swap_count > 0)
    {
        bubbleSort(strings, n - 1);
    }
}

struct stat getStats(char folder[], char directory[])
{
    struct stat filestat;
    char temp[1000];

    strcpy(temp, directory);
    strcat(temp, "/");
    strcat(temp, folder);
    stat(temp, &filestat);

    return filestat;
}

void printFolder(char folder[], char directory[])
{
    struct stat filestat = getStats(folder, directory);

    if (S_ISDIR(filestat.st_mode))
    {
        printf("\033[1;34m%s\n\033[0m", folder);
    }
    else if (filestat.st_mode & S_IXUSR)
    {
        printf("\033[1;32m%s\n\033[0m", folder);
    }
    else
    {
        printf("%s\n", folder);
    }
}

void printPermission(struct stat filestat)
{
    if (S_ISDIR(filestat.st_mode))
        printf("d");
    else
        printf("-");
    if (filestat.st_mode & S_IRUSR)
        printf("r");
    else
        printf("-");

    if (filestat.st_mode & S_IWUSR)
        printf("w");
    else
        printf("-");

    if (filestat.st_mode & S_IXUSR)
        printf("x");
    else
        printf("-");

    if (filestat.st_mode & S_IRGRP)
        printf("r");
    else
        printf("-");

    if (filestat.st_mode & S_IWGRP)
        printf("w");
    else
        printf("-");

    if (filestat.st_mode & S_IXGRP)
        printf("x");
    else
        printf("-");

    if (filestat.st_mode & S_IROTH)
        printf("r");
    else
        printf("-");

    if (filestat.st_mode & S_IWOTH)
        printf("w");
    else
        printf("-");

    if (filestat.st_mode & S_IXOTH)
        printf("x");
    else
        printf("-");
    printf(" ");
}

void printNumberOfLinks(struct stat filestat)
{
    printf("%ld ", filestat.st_nlink);
}

void printOwner(struct stat filestat)
{
    struct passwd *pw = getpwuid(filestat.st_uid);
    printf("%s ", pw->pw_name);
}

void printGroupOwner(struct stat filestat)
{
    struct group *gr = getgrgid(filestat.st_gid);
    printf("%s ", gr->gr_name);
}

void printSize(struct stat filestat)
{
    printf("%ld ", filestat.st_size);
}

void printLastModifiedTime(struct stat filestat)
{
    char time[100];
    strcpy(time, &(ctime(&filestat.st_mtime))[4]);
    time[strlen(time) - 9] = '\0';
    printf("%s ", time);
}

void print_ls_l(struct stat filestat)
{
    printPermission(filestat);
    printNumberOfLinks(filestat);
    printOwner(filestat);
    printGroupOwner(filestat);
    printSize(filestat);
    printLastModifiedTime(filestat);
}

void ls(char input[])
{
    input = strtok(NULL, "\t ");

    char directories[100][100];
    int n = 0; // stores no. of directories

    char files[100][100];
    int m = 0; // stores no. of files

    char not_existing[100][100];
    int p = 0; // stores no. of non-existing files/directories

    int flag1 = 0, flag2 = 0, flag3 = 0; // flag1 for -a, flag2 for-l, flag3 for -al

    while (input != NULL)
    {
        if (input[0] == '-' && strcmp(input, "-") != 0)
        {
            if (strcmp(input, "-a") == 0)
            {
                flag1 = 1;
            }
            else if (strcmp(input, "-l") == 0)
            {
                flag2 = 1;
            }
            else if (strcmp(input, "-al") == 0 || strcmp(input, "-la") == 0)
            {
                flag3 = 1;
            }
            else
            {
                printf("\033[1;31mls: invalid flag\033[0m\n");
                return;
            }
        }
        else
        {
            char path[1000];
            strcpy(path, input);
            if (input[0] == '~')
            {
                strcpy(path, home);
                strcat(path, &input[1]);
            }

            if (access(path, F_OK) != 0) // checks if file/directory exists or not
            {
                strcpy(not_existing[p++], path);
            }
            else
            {
                struct stat filestat;
                stat(path, &filestat);
                if (S_ISDIR(filestat.st_mode))
                {
                    strcpy(directories[n++], path);
                }
                else
                {
                    strcpy(files[m++], path);
                }
            }
        }
        input = strtok(NULL, "\t ");
    }

    for (int i = 0; i < p; i++)
    {
        printf("\033[1;31mls: cannot access '%s': No such file or directory\033[0m\n", not_existing[i]);
    }

    bubbleSort(files, m);
    bubbleSort(directories, n);

    // no flag and other files/folders given
    if (flag1 == 0 && flag2 == 0 && flag3 == 0)
    {
        if (n == 0 && m == 0 && p == 0)
        {
            n = 1;
            strcpy(directories[0], ".");
        }
        for (int i = 0; i < m; i++)
        {
            struct stat filestat;
            stat(files[i], &filestat);
            if (filestat.st_mode & S_IXUSR) // checks if file is executable or not
            {
                printf("\033[1;32m%s\n\033[0m", files[i]);
            }
            else
            {
                printf("%s\n", files[i]);
            }
        }
        if (m > 0 && n > 0)
        {
            printf("\n");
        }
        for (int i = 0; i < n; i++)
        {
            if (n > 1 || m > 0)
            {
                printf("%s:\n", directories[i]);
            }

            struct dirent *dir;
            DIR *dh = opendir(directories[i]);
            char folders[MAXFOLDERS][100];
            int k = 0;
            while ((dir = readdir(dh)) != NULL)
            {
                if (dir->d_name[0] == '.') // if hidden file continue
                {
                    continue;
                }
                strcpy(folders[k++], dir->d_name);
            }
            free(dir);
            closedir(dh);

            bubbleSort(folders, k);

            for (int j = 0; j < k; j++)
            {
                printFolder(folders[j], directories[i]);
            }
            if (n > 1 && i != n - 1)
            {
                printf("\n");
            }
        }
    }

    // -a
    else if (flag1 == 1 && flag2 == 0 && flag3 == 0)
    {
        // if no folder or file is given
        if (n == 0 && m == 0 && p == 0)
        {
            n = 1;
            strcpy(directories[0], ".");
        }
        for (int i = 0; i < m; i++)
        {
            struct stat filestat;
            stat(files[i], &filestat);
            if (filestat.st_mode & S_IXUSR)
            {
                printf("\033[1;32m%s\n\033[0m", files[i]);
            }
            else
            {
                printf("%s\n", files[i]);
            }
        }
        if (m > 0 && n > 0)
        {
            printf("\n");
        }
        for (int i = 0; i < n; i++)
        {
            if (n > 1 || m > 0)
            {
                printf("%s:\n", directories[i]);
            }

            struct dirent *dir;
            DIR *dh = opendir(directories[i]);
            char folders[MAXFOLDERS][100];
            int k = 0;
            while ((dir = readdir(dh)) != NULL)
            {
                strcpy(folders[k++], dir->d_name);
            }
            free(dir);
            closedir(dh);

            bubbleSort(folders, k);

            for (int j = 0; j < k; j++)
            {
                printFolder(folders[j], directories[i]);
            }
            if (n > 1 && i != n - 1)
            {
                printf("\n");
            }
        }
    }

    // -l
    else if (flag1 == 0 && flag2 == 1 && flag3 == 0)
    {
        int total = 0;

        // if no folder or file is given
        if (n == 0 && m == 0 && p == 0)
        {
            n = 1;
            strcpy(directories[0], ".");
        }
        for (int i = 0; i < m; i++)
        {
            struct stat filestat;
            stat(files[i], &filestat);
            print_ls_l(filestat);

            if (filestat.st_mode & S_IXUSR)
            {
                printf("\033[1;32m%s\n\033[0m", files[i]);
            }
            else
            {
                printf("%s\n", files[i]);
            }
        }
        if (m > 0 && n > 0)
        {
            printf("\n");
        }
        for (int i = 0; i < n; i++)
        {
            if (n > 1 || m > 0)
            {
                printf("%s:\n", directories[i]);
            }

            struct dirent *dir;
            DIR *dh = opendir(directories[i]);
            char folders[MAXFOLDERS][100];
            int k = 0;
            while ((dir = readdir(dh)) != NULL)
            {
                if (dir->d_name[0] == '.')
                {
                    continue;
                }
                strcpy(folders[k++], dir->d_name);
            }
            free(dir);
            closedir(dh);

            bubbleSort(folders, k);

            for (int j = 0; j < k; j++)
            {
                struct stat filestat = getStats(folders[j], directories[i]);
                total += filestat.st_blocks;
            }

            printf("total %d\n", total / 2);
            for (int j = 0; j < k; j++)
            {
                struct stat filestat = getStats(folders[j], directories[i]);
                print_ls_l(filestat);
                printFolder(folders[j], directories[i]);
            }
            if (n > 1 && i != n - 1)
            {
                printf("\n");
            }
        }
    }

    //-al
    else
    {
        int total = 0;

        // if no folder or file is given
        if (n == 0 && m == 0 && p == 0)
        {
            n = 1;
            strcpy(directories[0], ".");
        }
        for (int i = 0; i < m; i++)
        {
            struct stat filestat;
            stat(files[i], &filestat);
            print_ls_l(filestat);
            if (filestat.st_mode & S_IXUSR)
            {
                printf("\033[1;32m%s\n\033[0m", files[i]);
            }
            else
            {
                printf("%s\n", files[i]);
            }
        }
        if (m > 0 && n > 0)
        {
            printf("\n");
        }
        for (int i = 0; i < n; i++)
        {
            if (n > 1 || m > 0)
            {
                printf("%s:\n", directories[i]);
            }

            struct dirent *dir;
            DIR *dh = opendir(directories[i]);
            char folders[MAXFOLDERS][100];
            int k = 0;
            while ((dir = readdir(dh)) != NULL)
            {
                strcpy(folders[k++], dir->d_name);
            }
            free(dir);
            closedir(dh);

            bubbleSort(folders, k);

            for (int j = 0; j < k; j++)
            {
                struct stat filestat = getStats(folders[j], directories[i]);
                total += filestat.st_blocks;
            }

            printf("total %d\n", total / 2);
            for (int j = 0; j < k; j++)
            {
                struct stat filestat = getStats(folders[j], directories[i]);
                print_ls_l(filestat);
                printFolder(folders[j], directories[i]);
            }
            if (n > 1 && i != n - 1)
            {
                printf("\n");
            }
        }
    }
}