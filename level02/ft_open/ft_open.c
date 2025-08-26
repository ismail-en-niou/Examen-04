#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    if (!file || !argv || (type != 'r' && type != 'w'))
        return -1;
    pid_t pid ;
    int fd[2];

    if (pipe(fd) == -1)
        return -1;

    pid = fork();
    if (pid == -1)
    {
        close(fd[0]);
        close(fd[1]);
        return -1;
    }
    if (pid == 0)
    {
        if (type == 'r')
        {
            dup2(fd[1] , STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
        }else 
        {
            dup2(fd[0] , STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
        }
        execvp(file,argv);
        exit(1);
    }
    if (type == 'r')
    {
        close(fd[1]);
        return fd[0];
    }else
    {
        close (fd[0]);
        return(fd[1]);
    }
    return -1;
}





