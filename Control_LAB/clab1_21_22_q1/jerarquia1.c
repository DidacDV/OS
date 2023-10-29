#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void error_y_exit(char *msg) {
    perror(msg);
    exit(1);
}

void usage() {
    char buff[256];
    sprintf(buff, "Usage ./jerarquia1 file_name1 [.. file_name10]\n");
    write(1,buff,strlen(buff));
}

void main (int argc, char *argv[]) {
    if (argc < 2 || argc > 11) usage();
    int n = argc - 1;
    char buff[256];
    int pids[n];
    for (int i = 0; i < n; i++) {
        pids[i] = fork();
        if (pids[i] < 0) error_y_exit("fork");
        else if (pids[i] == 0) {
            sprintf(buff,"%s\n",argv[i+1]);
            write(1,buff, strlen(buff));
            exit(i);
        }
    }
    int pid, exit_stat, exit_code;
    char buff2 [256];
    for (int i = 0; i < n; i++) {
        if ((pid = waitpid(pids[i],&exit_stat,0)) < 0) error_y_exit("waitpid");
        if (WIFEXITED(exit_stat)) {
            exit_code = WEXITSTATUS(exit_stat);
        }
        sprintf(buff2,"pid: %d w/ exit code: %d\n", pid, exit_code);
        write(1, buff2, strlen(buff2));
    }
}