#include "headers.h"
#include "history.h"

extern char home[];

void storeHistory(char input[])
{
    char hist_path[100];
    strcpy(hist_path, home);
    strcat(hist_path, "/");
    strcat(hist_path, "history.txt");

    char hist[20][100];
    int i = 0;

    FILE *fp = fopen(hist_path, "r");
    if (fp != NULL)
    {
        while (fgets(hist[i], 100, fp))
        {
            i++;
        }
        fclose(fp);
    }

    char temp[100];
    strcpy(temp, input);
    strcat(temp, "\n");

    if (i < 20)
    {
        if (i == 0)
        {
            strcpy(hist[i], temp);
            FILE *fp = fopen(hist_path, "w");
            for (int j = 0; j <= i; j++)
            {
                fprintf(fp, "%s", hist[j]);
            }
            fclose(fp);
        }
        else if (strcmp(input, hist[i - 1]) != 0)
        {
            strcpy(hist[i], temp);
            FILE *fp = fopen(hist_path, "w");
            for (int j = 0; j <= i; j++)
            {
                fprintf(fp, "%s", hist[j]);
            }
            fclose(fp);
        }
    }
    else if (i == 20)
    {
        if (strcmp(input, hist[i - 1]) != 0)
        {
            FILE *fp = fopen(hist_path, "w");
            for (int j = 1; j < i; j++)
            {
                fprintf(fp, "%s", hist[j]);
            }
            fprintf(fp, "%s", temp);
            fclose(fp);
        }
    }
}

void history(char input[])
{
    input = strtok(NULL, "\t ");

    if (input != NULL)
    {
        printf("\033[1;31mhistory: too many arguments\033[0m\n");
        return;
    }

    char hist_path[100];
    strcpy(hist_path, home);
    strcat(hist_path, "/");
    strcat(hist_path, "history.txt");

    char hist[20][100];
    int i = 0;

    FILE *fp = fopen(hist_path, "r");
    while (fgets(hist[i], 100, fp))
    {
        i++;
    }
    fclose(fp);

    if (i < 10)
    {
        for (int j = 0; j < i; j++)
        {
            printf("%s", hist[j]);
        }
    }
    else
    {
        for (int j = i - 10; j < i; j++)
        {
            printf("%s", hist[j]);
        }
    }
}
