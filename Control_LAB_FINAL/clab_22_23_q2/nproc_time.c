#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

void Usage() {
    error(1,0, "Invalid argument. \n Correct usage: ./nproc_time PID1 PID2 ... PIDN");
}


int main(int argc, int *argv[]) {
    
    if (argc < 2) Usage();
    int exit_stat, exit_code;
    for (int i = 1; i < argc; ++i) {
        int pid = fork();
        if (pid < 0) error(1,errno, "fork");
        if (pid == 0) {
            execl("./proc_time","proc_time", argv[i], NULL);
            error(1, errno, "execlp");
        }
        else {
            if (waitpid(pid, &exit_stat, 0) < 0) error(1, errno, "waitpid");
            if (WIFEXITED(exit_stat)) {
                exit_code = WEXITSTATUS(exit_stat);
                if (exit_code == 255) error(1,0,"There is a not existent PID");
            }
        }
    }
}
