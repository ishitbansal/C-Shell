#ifndef __HEADERS_H
#define __HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <ctype.h>
#define MAXFOLDERS 400

#include "prompt.h"
#include "echo.h"
#include "pwd.h"
#include "cd.h"
#include "pinfo.h"
#include "discover.h"
#include "ls.h"
#include "foreground.h"
#include "background.h"
#include "history.h"
#include "autocompletion.h"
#include "bg.h"
#include "jobs.h"
#include "sig.h"
#include "fg.h"

typedef struct bg_
{
    int pid;
    int n;
    char command[100];
    int check; // checks it exit status is normal or abnormal
} bg_proc;

#endif