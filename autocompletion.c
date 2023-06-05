#include "headers.h"
#include "autocompletion.h"

extern char current_directory[];

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

int findCommonCharacters(char a[], char b[])
{
    int n = strlen(a);
    int m = strlen(b);

    int min;
    if (a < b)
    {
        min = n;
    }
    else
    {
        min = m;
    }

    int ans = 0;

    for (int i = 0; i < min; i++)
    {
        if (a[i] == b[i])
        {
            ans++;
        }
        else
        {
            break;
        }
    }

    return ans;
}

void printMatching(char input[], char prompt_string[])
{
    char directory[100];
    strcpy(directory, current_directory);

    int flag = 0;

    int n = strlen(input);
    for (int i = n - 1; i >= 0; i--)
    {
        if (input[i] == '/')
        {
            strncpy(directory, input, i + 1);
            directory[i + 1] = '\0';
            strcpy(input, &input[i + 1]);
            flag = 1;
            break;
        }
    }

    struct dirent *dir;
    DIR *dh = opendir(directory);
    char folders[100][20];
    int k = 0;
    if (dh != NULL)
    {
        while ((dir = readdir(dh)) != NULL)
        {
            if (dir->d_name[0] != '.')
            {
                strcpy(folders[k++], dir->d_name);
            }
        }
        free(dir);
        closedir(dh);
    }

    int len = strlen(input);

    char common_files[100][10];

    int j = 0;

    for (int i = 0; i < k; i++)
    {
        if (strncmp(input, folders[i], len) == 0)
        {
            strcpy(common_files[j++], folders[i]);
        }
    }

    if (j > 0)
    {
        char common[100];
        strcpy(common, common_files[0]);

        for (int i = 0; i < j; i++)
        {
            int temp = findCommonCharacters(common, common_files[i]);
            strncpy(common, common, temp);
            common[temp] = '\0';
        }

        if (j == 1)
        {
            struct stat filestat;
            char temp[1000];
            strcpy(temp, directory);
            strcat(temp, "/");
            strcat(temp, common_files[0]);
            stat(temp, &filestat);

            if (S_ISDIR(filestat.st_mode))
            {
                strcat(common, "/");
            }
            else
            {
                strcat(common, " ");
            }
        }

        if (j > 1)
        {
            for (int i = 0; i < k; i++)
            {
                if (strncmp(input, folders[i], len) == 0)
                {
                    printf("%s", folders[i]);
                    struct stat filestat;
                    char temp[1000];
                    strcpy(temp, directory);
                    strcat(temp, "/");
                    strcat(temp, folders[i]);
                    stat(temp, &filestat);

                    if (S_ISDIR(filestat.st_mode))
                    {
                        printf("/\n");
                    }
                    else
                    {
                        printf(" \n");
                    }
                }
            }
        }

        if (flag == 1)
        {
            strcat(prompt_string, directory);
        }
        strcat(prompt_string, common);
    }
    else
    {
        if (flag == 1)
        {
            strcat(prompt_string, directory);
        }
        strcat(prompt_string, input);
    }

    prompt(current_directory);
    printf("%s", prompt_string);
}

char *inputString()
{
    char *inp = malloc(sizeof(char) * 100);
    char c;

    setbuf(stdout, NULL);
    enableRawMode();
    memset(inp, '\0', 100);
    int pt = 0;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            if (c == 10)
            {
                break;
            }
            else if (c == 127)
            { // backspace
                if (pt > 0)
                {
                    if (inp[pt - 1] == 9)
                    {
                        for (int i = 0; i < 7; i++)
                        {
                            printf("\b");
                        }
                    }
                    inp[--pt] = '\0';
                    printf("\b \b");
                }
            }
            else if (c == 9)
            {
                char command[100];
                int i = 1;
                while (pt - i >= 0 && inp[pt - i] != 32)
                {
                    i++;
                }

                strncpy(command, &inp[pt - i + 1], i);
                command[i] = '\0';

                inp[pt - i + 1] = '\0';

                printf("\n");
                printMatching(command, inp);

                pt = strlen(inp);
            }
            else if (c == 4)
            {
                printf("\n");
                exit(0);
            }
        }
        else
        {
            inp[pt++] = c;
            printf("%c", c);
        }
    }

    disableRawMode();

    return inp;
}