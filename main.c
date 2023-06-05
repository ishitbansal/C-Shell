#include "headers.h"

char previous_directory[1000] = "\033[1;31mcd: OLDPWD not set\033[0m"; // stores absolute path of previous directory
char current_directory[1000];                                          // stores absolute path of current directory
char home[1000];                                                       // stores home directory of shell
bg_proc bg_processes[100];
bg_proc foreground_process;          // stores background processes
int exit_commands[100];              // stores bg_processes exit commands that have to be printed
int printTime = 0;                   // to check if we have to print time in prompt after foreground process
int original_input, original_output; // stores stdin and stdout
int fgProc = 0;                      // if foreground process is running or not

void command_run(char command[])
{
    char *a = strtok(command, "\t "); // a stores the first command and command is tokenised

    if (a == NULL)
    {
        return;
    }
    if (strcmp(a, "echo") == 0)
    {
        echo(command);
    }
    else if (strcmp(a, "pwd") == 0)
    {
        pwd();
    }
    else if (strcmp(a, "cd") == 0)
    {
        cd(command);
    }
    else if (strcmp(a, "ls") == 0)
    {
        ls(command);
    }
    else if (strcmp(a, "history") == 0)
    {
        history(command);
    }
    else if (strcmp(a, "pinfo") == 0)
    {
        pinfo(command);
    }
    else if (strcmp(a, "discover") == 0)
    {
        discover(command);
    }
    else if (strcmp(a, "jobs") == 0)
    {
        jobs(command);
    }
    else if (strcmp(a, "sig") == 0)
    {
        sig(command);
    }
    else if (strcmp(a, "fg") == 0)
    {
        fg(command);
    }
    else if (strcmp(a, "bg") == 0)
    {
        bg(command);
    }
    else if (strcmp(a, "quit") == 0)
    {
        exit(0);
    }
    else
    {
        foreground(a, command);
    }
}

void input_output_redirection(char process[], int bg)
{
    int write = 0, append = 0, read = 0, error = 0, r = 0, a = 0, w = 0;
    char write_file[100], append_file[100], read_file[100];

    int m = strlen(process);
    for (int k = 0; k < m; k++)
    {
        if (process[k] == '<')
        {
            if (read == 0)
            {
                read = 1;
                r = k;
            }
            else
            {
                error = 1;
                printf("\033[1;31m<: only one input file expected\033[0m\n");
                break;
            }
        }
        else if (process[k] == '>')
        {
            if (k != m - 1 && process[k + 1] == '>')
            {
                if (append == 0)
                {
                    append = 1;
                    a = k + 1;
                    k++;
                }
                else
                {
                    error = 1;
                    printf("\033[1;31m>>: only one output file expected\033[0m\n");
                    break;
                }
            }
            else
            {
                if (write == 0)
                {
                    write = 1;
                    w = k;
                }
                else
                {
                    error = 1;
                    printf("\033[1;31m>: only one output file expected\033[0m\n");
                    break;
                }
            }
        }
    }

    if (error == 0 && write == 1 && append == 1)
    {
        error = 1;
        printf("\033[1;31merror: output to only one file is allowed\033[0m\n");
    }

    if (error)
    {
        return;
    }

    if (read == 1)
    {
        if (write == 1)
        {
            char temp[100];
            strncpy(temp, &process[r + 1], w - r - 1);
            temp[w - r] = '\0';

            char *str = strtok(temp, " ");
            strcpy(read_file, str);
        }
        else if (append == 1)
        {
            char temp[100];
            strncpy(temp, &process[r + 1], a - r - 2);
            temp[a - r - 1] = '\0';

            char *str = strtok(temp, " ");
            strcpy(read_file, str);
        }
        else
        {
            char temp[100];
            strcpy(temp, &process[r + 1]);

            char *str = strtok(temp, " ");
            strcpy(read_file, str);
        }
    }
    if (write == 1)
    {
        char temp[100];
        strcpy(temp, &process[w + 1]);

        char *str = strtok(temp, " ");
        strcpy(write_file, str);
    }
    else if (append == 1)
    {
        char temp[100];
        strcpy(temp, &process[a + 1]);

        char *str = strtok(temp, " ");
        strcpy(append_file, str);
    }

    char command[1000];

    int fd_input, fd_output;

    if (read == 1)
    {
        fd_input = open(read_file, O_RDONLY);
        if (fd_input < 0)
        {
            printf("\033[1;31merror:input file does not exist\033[0m\n");
            return;
        }
        dup2(fd_input, STDIN_FILENO);

        strncpy(command, process, r);
        command[r] = '\0';
    }
    if (write == 1)
    {
        fd_output = open(write_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd_output, STDOUT_FILENO);

        if (read == 0)
        {
            strncpy(command, process, w);
            command[w] = '\0';
        }
    }
    else if (append == 1)
    {
        fd_output = open(append_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        dup2(fd_output, STDOUT_FILENO);

        if (read == 0)
        {
            strncpy(command, process, a - 1);
            command[a - 1] = '\0';
        }
    }
    if (read == 0 && write == 0 && append == 0)
    {
        strcpy(command, process);
    }

    if (bg == 1)
    {
        background(command);
    }
    else
    {
        fgProc = 1;
        command_run(command);
        fgProc = 0;
    }

    if (read == 1)
    {
        close(fd_input);
    }
    if (write == 1 || append == 1)
    {
        close(fd_output);
    }

    dup2(original_input, STDIN_FILENO);
    dup2(original_output, STDOUT_FILENO);
}

void ctrlC()
{
    printf("\n");
    if (fgProc == 0)
    {
        prompt(current_directory);
    }
    else
    {
        fgProc = 0;
    }
    fflush(stdout);
}

void ctrlZ()
{
    if (fgProc == 1)
    {
        kill(foreground_process.pid, SIGTSTP);
        int index = -1;
        for (int j = 0; j < 100; j++)
        {
            if (bg_processes[j].pid != -__INT32_MAX__)
            {
                index = j;
            }
        }
        index++;
        bg_processes[index].pid = foreground_process.pid;
        bg_processes[index].n = index + 1;
        strcpy(bg_processes[index].command, foreground_process.command);

        foreground_process.pid = -__INT_MAX__;
        fgProc = 0;
        printf("\n");
    }
    else
    {
        printf("\n");
        prompt(current_directory);
        fflush(stdout);
    }
}

int main()
{
    getcwd(home, sizeof(home));

    initializeBg();
    initializeExitCommands();

    signal(SIGINT, ctrlC);
    signal(SIGTSTP, ctrlZ);
    signal(SIGCHLD, checkExitStatus);

    // clears the whole screen
    // printf("\e[1;1H\e[2J");

    strcpy(current_directory, home);

    while (1)
    {
        printExitCommands();

        prompt(current_directory);

        char *input = inputString();
        printf("\n");

        storeHistory(input);

        char *token;
        token = strtok(input, ";");

        char commands[100][100]; // stores ; seperated commands

        int n = 0;
        while (token != NULL)
        {
            strcpy(commands[n++], token);
            token = strtok(NULL, ";");
        }

        original_input = dup(STDIN_FILENO);
        original_output = dup(STDOUT_FILENO);

        for (int j = 0; j < n; j++)
        {
            int x = strlen(commands[j]);
            int y = 0;
            for (int k = 0; k < x; k++)
            {
                char bg_process[100]; // stores & seperated commands
                if (commands[j][k] == '&')
                {
                    strncpy(bg_process, &commands[j][y], k - y);
                    bg_process[k - y] = '\0';
                    y = k + 1;
                    input_output_redirection(bg_process, 1);
                }
            }

            char fg_process[1000];
            strcpy(fg_process, &commands[j][y]);

            char pipe_commands[10][100];

            x = strlen(fg_process);
            y = 0;
            int l = 0;
            for (int k = 0; k < x; k++)
            {
                if (fg_process[k] == '|')
                {
                    strncpy(pipe_commands[l], &fg_process[y], k - y);
                    pipe_commands[l][k - y] = '\0';
                    y = k + 1;
                    l++;
                }
            }
            strcpy(pipe_commands[l++], &fg_process[y]);

            if (l == 1)
            {
                input_output_redirection(fg_process, 0);
            }

            else if (l > 1)
            {
                int curr_pipe_fd[2];
                int prev_pipe_fd[2];

                for (int k = 0; k < l; k++)
                {
                    if (pipe(curr_pipe_fd) < 0)
                    {
                        printf("\033[1;31mCould not create pipe\033[0m\n");
                        break;
                    }

                    if (k == 0)
                    {
                        dup2(original_input, STDIN_FILENO);
                        dup2(curr_pipe_fd[1], STDOUT_FILENO);
                        input_output_redirection(pipe_commands[k], 0);
                    }

                    else if (k == l - 1)
                    {
                        dup2(prev_pipe_fd[0], STDIN_FILENO);
                        dup2(original_output, STDOUT_FILENO);
                        input_output_redirection(pipe_commands[k], 0);
                        close(prev_pipe_fd[0]);
                        close(curr_pipe_fd[0]);
                        close(curr_pipe_fd[1]);
                        break;
                    }

                    else
                    {
                        dup2(prev_pipe_fd[0], STDIN_FILENO);
                        dup2(curr_pipe_fd[1], STDOUT_FILENO);
                        input_output_redirection(pipe_commands[k], 0);
                        close(prev_pipe_fd[0]);
                    }

                    prev_pipe_fd[0] = curr_pipe_fd[0];
                    prev_pipe_fd[1] = curr_pipe_fd[1];
                    close(curr_pipe_fd[1]);
                }

                dup2(original_input, STDIN_FILENO);
                dup2(original_output, STDOUT_FILENO);
            }
        }
    }
}
