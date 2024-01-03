#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


void error_y_exit(char *s) {
    char buff[100];
    sprintf(buff, "%s error", s);
    write(1, buff, strlen(buff));
    exit(2);
}


void Usage() {
    char buff[100];
    sprintf(buff, "Invalid argument\n Correct usage: ./n_espera_sig N");
    write(1, buff, strlen(buff));
    exit(1);
}


int main(int argc, char *argv[]) {
    if (argc != 2) Usage();
    int n = atoi(argv[1]), pid, exit_status, exit_code;
    char buff1[100];

    for (int i = 0; i < n; ++i) {
        pid = fork();
        if (pid < 0) error_y_exit("fork");
        else if (pid == 0) {
            sprintf(buff1,"My PID: %d", getpid());
            write(1,buff1, strlen(buff1));
            execlp("./espera_sig", "espera_sig", NULL);
            error_y_exit("execlp");
        }
    }

    int fd = creat("exit_status.int", S_IRUSR|S_IWUSR|S_IRGRP);

    while ((pid = waitpid(-1, &exit_code, 0)) > 0) {
        if (WIFEXITED(exit_code)) {
            exit_status = WEXITSTATUS(exit_code);
            write(fd, &pid, sizeof(int));
            write(fd, &exit_status, sizeof(int));  
        }  
    }

    if (pid < 0) error_y_exit("waitpid");
}