#include "headers.h"
#include "echo.h"

void echo(char input[])
{
    input = strtok(NULL, "\t ");
    while (input != NULL)
    {
        printf("%s ", input);
        input = strtok(NULL, "\t ");
    }
    printf("\n");
}