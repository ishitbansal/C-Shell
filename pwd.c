#include "headers.h"
#include "pwd.h"

void pwd()
{
    char directory[1000];
    getcwd(directory, sizeof(directory));
    printf("%s\n", directory);
}