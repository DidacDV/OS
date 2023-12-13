#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void Usage() {
    error(1,0, "Invalid argument. \n Correct usage: ./nproc_time PID1 PID2 ... PIDN");
}


int main(int argc, int *argv[]) {
    int max_time = 0, fd1, fd2, t, ret;
    char buff[100], c, number[100];
    if (argc < 2) Usage();
    int exit_stat, exit_code;
    for (int i = 1; i < argc; ++i) { 
        int p = 0;
        int pid = fork();
        int *fd = malloc(2*sizeof(int));
        pipe(fd);
        if (pid < 0) error(1,errno, "fork");
        else if (pid == 0) {
            dup2(fd[1], 1);
            close(fd[0]);
            execl("proc_time","./proc_time", argv[i], NULL);
            error(1, errno, "execlp");
        }
        else {
            close(fd[1]);
            while ((ret = read(fd[0], &c, sizeof(char))) > 0) {
                number[p] = c;
                number[p + 1] = ' ';
                ++p;
            }
            if (ret < 0) error(1, errno, "read");
            free(fd);
            t = atoi(&number[0]);
            if (t > max_time) max_time = t;
        }
    }
    
    while ((ret = waitpid(-1, &exit_stat, 0) > 0) { 
     if (WIFEXITED(exit_stat)) {
         exit_code = WEXITSTATUS(exit_stat);
         if (exit_code == 255) error(1,0,"There is a not existent PID");
     }
    }
    if (ret < 0) error(1, errno, "waitpid");
    sprintf(buff, "Max_time: %d",max_time);
    if (write(1,buff, strlen(buff)) < 0) error(1, errno, "write");


}
