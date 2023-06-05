# OSN
## C Shell
<br>

> Name: Ishit Bansal

----

<br>

> To compile the shell run - ``` make ``` from the directory in which the shell is present

> To execute - ``` a.out ```   

> To exit from shell - ``` quit ``` or ``` Ctrl-D```

> Errors are printed in red

<br>

### Files: 
----

- prompt.c
    - Prints prompt
    - If foreground process is just completed, cumulative time taken is printed
    - If there are terminated background processes, there exit commands are printed
    - Tabs and spaces are handled
    - Input entered is tokenised by ';', so it is assumed that two seperate commands will have only one ';' between them
    - Input is then segregated by '&', so it is assumed that input will not begin with & and two seperate commands will have only one '&' between them
    - Color coding of prompt is done

<br>

- echo.c
    - Prints everything after echo command are removing excess spaces and tabs
    - Environment variables are not handles seperately , eg : 'echo ~' will print '~'

<br>

- cd.c
    - Changes to different directories
    - If more than one arguments are given, error is printed
    - If file is given after cd then, error is printed
    - If directory/file doesn't exist, error is printed

<br>

- pwd.c
    - Prints the absolute path of current working directory
    - If more arguments are given, then also pwd is printed (as in bash shell)

<br>

- ls.c
    - Lists all files/directories of the given path(s) as per given flag (-a,-l,-al)
    - Color coding of files,executables and directories is done
    - If invalid file/directory is given, error is printed
    - If invalid flag is given, error is printed

<br>

- foreground.c
    - Runs a command which is not builtin in the shell
    - Time taken to finish the process is printed in the next prompt
    - If commands does not exist, error is printed

<br>

- background.c
    - Runs a command which has a '&' in the end in background
    - Process number and its pid is printed
    - Exit command is printed after it terminates and enter key is pressed
    - If commands does not exist, error is printed

<br>

- pinfo.c
    - Prints the process-related info of shell program of itself or of a specific pid(if given)
    - If pid given is invalid, error is printed
    - If more than one arguments are given, error is printed

<br>

- discover.c
    - The command searches for files in a directory hierarchy and prints their path
    - Hidden files are not searched
    - It is assumed that there are at max 4 arguments
    - Flags handled are -d and -f (-df or -fd are not handled explicitly, use -d -f or -f -d instead)
    - If more than 4 arguments are given, error is printed
    - If multiple target directories or filenames are given error is printed
    - If invalid target directory is given, nothing is printed
    - If there are no paths to the given filename, nothing is printed
    - If invalid flag is given, error is printed

<br>

- history.c
    - history.txt is created(if not present) in the home directory
    - Every time, we write a command, it is saved in history.txt (at max 20)
    - Prints last 10 commands
    - If any more arguments are given, error is printed

<br>

- input-output redirection (in main.c)
    - assumed that file given is in the end of the command
    - else done as required
    - handled in background processes as well
    - Appropriate error printed for incorrect usage by the user

<br>

- pipes (in main.c)
    - pipe, redirects the output of the command on the left as input to the command on the right
    - done as required
    - explicitly not handled for background processes
    - Appropriate error printed for incorrect usage by the user
    - Two seperate commands will have only one '|' between them

<br>

- jobs.c
    - prints a list of all currently running background processes spawned by the shell in alphabetical order of the command name, along with their job number (a sequential number assigned by your shell), process ID, and their state, which can either be running or stopped
    - Flags handled are -r and -s (-rs or -sr -r -s are not handled explicitly, use jobs with no flag instead )
    - If invalid flag is given, error is printed
    - If more than 2 arguments are given, error is printed

<br>

- sig.c
    - Takes the job number of a running job and sends the signal corresponding to the signal number to that process
    - Appropriate error printed for incorrect usage by the user

<br>

- fg.c
    - Brings the running or stopped background job corresponding to job number to the foreground, and changes its state to running    
    - Appropriate error printed for incorrect usage by the user

<br>

- bg.c
    - Changes the state of a stopped background job to running (in the background).
    - Appropriate error printed for incorrect usage by the user

<br>

- signal handling (in main.c)

-  autocompletion.c 
    - When tab key is pressed, it will print all files which have the same starting letters and then again show the same prompt
    - If there is only one file/folder which has the same starting characters, the prompt is filled with that