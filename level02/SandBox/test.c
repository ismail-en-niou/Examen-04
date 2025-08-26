#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/resource.h>

void handelr(int a)
{
    (void )a;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    pid_t pid;
    int status;
    int res;

    pid = fork();
    if (pid == -1)
        return -1;
    if (pid == 0)
    {
        alarm(timeout);
        f();
        exit(0);
    }
    struct  sigaction sa;
    sa.sa_handler = handelr;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM , &sa , NULL);
    alarm(timeout);

    if (waitpid(pid , &status , 0) == -1)
    {
        if (errno == EINTR)
        {
            kill(pid , SIGKILL);
            waitpid(pid , NULL , 0);
            if (verbose) printf("Bad function: timed out after %d econds\n" , timeout);
            return 0;
        }
        return -1;
    }
    if (WIFEXITED(status))
    {
        if (WEXITSTATUS(status) == 0)
        {
            if (verbose) printf("Nice function!\n");
            return 1;
        }else {
            if (verbose) printf("Bad function: exited with code %d\n" , WEXITSTATUS(status));
            return 0;
        }
    }
    if (WIFSIGNALED(status))
    {
        if (verbose) printf("Bad function: %s\n" , strsignal(WTERMSIG(status)));
        return 0;
    }
    return -1;
}