#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

// close, fork, wait, exit, execvp, dup2, pipe

int	picoshell(char **cmds[])
{
	pid_t	pid;
	int		pipefd[2];
	int		prev_fd;
	int		status;
	int		exit_code;
	int		i;

	prev_fd = -1;
	exit_code = 0;
	i = 0;
	while (cmds[i])
	{
		if (cmds[i + 1] && pipe(pipefd))
			return (1);
		pid = fork();
		if (pid == -1)
		{
			if (cmds[i + 1])
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			return (1);
		}
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				dup2(prev_fd, 0);
				close(prev_fd);
			}
			if (cmds[i + 1])
			{
				dup2(pipefd[1], 1);
				close(pipefd[0]);
				close(pipefd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmds[i + 1])
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		i++;
	}
	while (wait(&status) != -1)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			exit_code = 1;
	}
	return (exit_code);
}