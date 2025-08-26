#include <sys/_types/_pid_t.h>
#include <unistd.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    if (!file || !argv || (type != 'r' && type != 'w'))
        return -1;
    pid_t pid;
    int fds[2];
    if (pipe(fds) == -1)
        return -1;
    pid = fork();
    if (pid == -1)
    {
        close(fds[1]);
        close(fds[0]);
        return -1;
    }
    if (pid == 0)
    {
        if (type == 'r')
            dup2(fds[1] , 1);
        else
            dup2(fds[0] , 0);
        close(fds[1]);
        close(fds[0]);
        execvp(file , argv );
        exit(1);
    }
    if (type == 'r')
    {
        close (fds[1]);
        return fds[0];
    }else {
        close(fds[0]);
        return fds[1];
    }
    return -1;
}