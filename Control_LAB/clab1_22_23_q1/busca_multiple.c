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
    sprintf(buff,"Usage: ./busca_multiple word file_name1 [file_name2 ... file_name10]");
    write(1,buff,strlen(buff));
    exit(1);
}

void main(int argc, char *argv[]) {

    if (argc < 2 || argc > 12) usage();

    int n = argc - 2;

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGUSR1);
    sigaddset(&mask,SIGUSR2);
    if (sigprocmask(SIG_BLOCK,&mask, NULL) < 0) error_y_exit("sigprocmask");

    int pid[n];
    char buff[128];
    for (int i = 0; i < n; i++) {
        pid[i] = fork();

        if (pid[i] < 0) error_y_exit("fork");
        else if (pid[i] == 0) {
            if (i != 0) {
                sprintf(buff,"%d",pid[i - 1]);
            }
            else sprintf(buff,"%d",pid[i]);
            execlp("./busqueda_parcial_con_espera","./busqueda_parcial_con_espera", buff, argv[1], argv[i+2],NULL);
            error_y_exit("execlp");
        }
    }

    kill(pid[n-1],SIGUSR1);
    int pid;
    while ((pid  = waitpid(-1,NULL,0)) > 0);
    if (pid == -1) error_y_exit("waitpid");
}
