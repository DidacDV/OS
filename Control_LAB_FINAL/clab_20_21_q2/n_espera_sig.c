#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int *vpid, n;


void error_y_exit(char *s) {
    char buff[100];
    sprintf(buff, "%s error", s);
    write(1, buff, strlen(buff));
    exit(2);
}

void Usage() {
    char buff[100];
    sprintf(buff, "Invalid argument\n Correct usage: ./n_espera_sig N\n");
    write(1, buff, strlen(buff));
    exit(1);
}

int locate_pid(int pid) {
    for (int i = 0; i < n; ++i) {
        if (vpid[i] == pid) return i;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) Usage();
    n = atoi(argv[1]);
    int exit_status, exit_code;
    vpid = malloc(n * sizeof(int));
    char buff1[100];

    for (int i = 0; i < n; ++i) {
        vpid[i] = fork();
        if (vpid[i] < 0) error_y_exit("fork");
        else if (vpid[i] == 0) {
            sprintf(buff1,"My PID: %d\n", getpid());
            write(1,buff1, strlen(buff1));
            execlp("./espera_sig", "espera_sig", NULL);
            error_y_exit("execlp");
        }
    }

    int fd = creat("exit_status.int", S_IRUSR|S_IWUSR|S_IRGRP);     
    int pid;
    while ((pid = waitpid(-1, &exit_code, 0)) > 0) {
        if (WIFEXITED(exit_code)) {
            exit_status = WEXITSTATUS(exit_code);
            int pos = locate_pid(pid);
            lseek(fd, pos * 8, SEEK_SET);                   
            write(fd, &pid, sizeof(int));
            write(fd, &exit_status, sizeof(int));  
        }  
    }
    if (pid < 0 && errno != ECHILD) {
        error_y_exit("waitpid");
    }

    free(vpid);
}