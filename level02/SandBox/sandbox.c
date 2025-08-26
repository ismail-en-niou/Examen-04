#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int g = 0;

void handeler(int a)
{
    (void )a ;
    g = 1;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0)
        return -1;
    if (pid == 0)
    {
        alarm(timeout);
        f();
        exit(1);
    }

    struct  sigaction sa;
    sa.sa_handler = handeler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM , &sa , NULL);
    alarm(timeout);
    int res = waitpid(pid , &status , 0);
    if (res == -1 || g == 1)
    {
        if (errno == 4 || g == 1)
        {
            kill(pid , SIGKILL);
            waitpid(pid , NULL , 0);
            if (verbose) printf("Bad function: timed out after %u seconds\n", timeout);
            return 0;
        }
        return -1;
    }
    if ( WIFEXITED(status))
    {
        if (WEXITSTATUS(status) == 0)
        {
            if (verbose) printf("Nice function!\n");
            return 1;
        }
        else
        {
            if (verbose) printf("Bad function: exited with code %d\n", WEXITSTATUS(status));
            return 0;
        }
    }
    if (WIFSIGNALED(status))
    {
        if (verbose) printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
        return 0;
    }
}

