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

int g = 0;

void handel(int sig)
{
    (void) sig;
    g = 1;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    pid_t pid;
    int status;
    int result;
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
    sa.sa_handler = handel;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM , &sa , 0);
    alarm(timeout);
    result = waitpid(pid , &status , 0);
    if ( result == -1 || g )
    {
        if (errno ==  4 || g)
        {
            kill(pid , SIGKILL);
            waitpid(pid , NULL , 0);
            if (verbose) printf("Bad function: timed out after %u seconds\n", timeout);
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
        }else{
            if (verbose) printf("Bad function: exited with code %d\n", WEXITSTATUS(status));
            return 0;
        }
    }
    if (WIFSIGNALED(status))
    {
        if (verbose) printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
        return 0;
    }
    return -1;
}