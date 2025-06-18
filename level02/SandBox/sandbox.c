#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

void vv(int sg)
{
    (void)sg;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    pid_t pid ;
    int status ;

    if (!f)
        return -1;
    pid = fork();

    if (pid < 0)
        return -1;

    struct sigaction sa;
    sa.sa_handler = vv;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM,&sa,NULL);

    if (pid == 0)
    {
        f();
        exit(0);
    }
    alarm(timeout);
    pid_t w = waitpid(pid,&status, 0);

    if (w == -1)
    {
        if (errno == 4)
        {
            kill(pid , 9);
            waitpid(pid , NULL , 0);
            if (verbose )
                printf("Bad function: timed out after %d seconds\n",timeout);
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
        }
        else
        {
            if (verbose) printf("Bad function: exited with code %d\n",WEXITSTATUS(status));
            return 0;
        }
    }
    if (WIFSIGNALED(status))
    {
        if (verbose) printf("Bad function: %s\n",strsignal(WTERMSIG(status)));
        return 0;
    }
    return -1;
}