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
    int max_time = 0, t, ret;
    char buff[100], c, number[100];
    if (argc < 2) Usage();
    int exit_stat, exit_code;
    for (int i = 1; i < argc; ++i) { 
        int p = 0;
        int *fd = malloc(2*sizeof(int));
        pipe(fd);
        int pid = fork();
        if (pid < 0) error(1,errno, "fork");
        else if (pid == 0) {
            dup2(fd[1], 1);
            close(fd[0]);
            execlp("./proc_time","proc_time", argv[i], NULL);
            error(1, errno, "execlp");
        }
        else {
            close(fd[1]);
            if ((ret = read(fd[0], number, sizeof(number))) < 0) error(1, errno, "read");
            number[ret] = '\0';
            free(fd);
            t = atoi(number);
            if (t > max_time) max_time = t;
        }
    }
    
    for (int i = 1; i < argc; ++i) {
        if ((ret = waitpid(-1, &exit_stat, 0)) < 0) error(1, errno, "waitpid");
        if (WIFEXITED(exit_stat)) {
            exit_code = WEXITSTATUS(exit_stat);
            if (exit_code == 255) error(1,0,"There is a not existent PID");
     }  
    }
    sprintf(buff, "Max_time: %d\n",max_time);
    if (write(1,buff, strlen(buff)) < 0) error(1, errno, "write");


}
