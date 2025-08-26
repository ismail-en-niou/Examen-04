#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>


// int    picoshell(char **cmds[])
// {
//     int fd_prev = -1;
//     int fd[2];
//     pid_t pid ;
//     int st = 0;
//     int i = 0;

//     while (cmds[i])
//     {
//         if (cmds[i + 1])
//             pipe(fd);
//         pid = fork();
//         if (pid < 0)
// 		{
// 			if (fd_prev != -1)
// 				close(fd_prev);
// 			if (cmds[i + 1])
// 			{
// 				close(fd[0]);
// 				close(fd[1]);
// 			}
// 			return 1;
// 		}
//         if (pid == 0)
//         {
//             if (fd_prev != -1)
//             {
//                 dup2(fd_prev , 0);
//                 close(fd_prev);
//             }
//             else
//             {
//                 dup2(fd[1] , 1);
//                 close(fd[1]);
//                 close(fd[0]);
//             }
//             execvp(cmds[i][0], cmds[i]);
// 			exit(1);
//         }
//         if (fd_prev != -1)
//             close(fd_prev);
//         if (cmds[i + 1])
//         {
//             close(fd[1]);
//             fd_prev = fd[0];
//         }
//         i++;
//     }
//     while (wait(&st) != -1);
//     return 0;
// }
