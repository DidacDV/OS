#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int pid_2;

void trat_sigusr1 (int s) {
    char buff[256];
    sprintf(buff,"SIGNAL recibido %d\n", s);
    write(1, buff, strlen(buff));
}

void trat_sigusr2(int s) {

    char buff[256];
    sprintf(buff,"SIGNAL recibido %d\n", s);
    write(1, buff, strlen(buff));
    if (pid_2 != 0) kill(pid_2, SIGUSR2);
    exit(1);
}


void error_y_exit(char *msg, int exit_status) {
    
    perror(msg);
    exit(exit_status);
}

void usage() {

    char buff[256];

    sprintf(buff,"Usage: PID word file_name\n");
    write(1,buff,strlen(buff));
    exit(1);
}

void main(int argc, char *argv[]) {
    pid_2 = atoi(argv[1]);
    if (argc != 4) usage();
    struct sigaction trat;
    sigset_t mask;
    trat.sa_flags = 0;
    sigemptyset(&trat.sa_mask);
    trat.sa_handler = trat_sigusr1;
    if (sigaction(SIGUSR1,&trat,NULL) < 0) error_y_exit("sigaction", 1);
    trat.sa_handler = trat_sigusr2;
    if (sigaction(SIGUSR2,&trat, NULL) < 0) error_y_exit("sigaction", 1);
    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGUSR2);
    sigsuspend(&mask);

    int pid = fork();
    if (pid < 0) error_y_exit("fork", 1);
    else if (pid == 0) {
        execlp("grep","grep",argv[2], argv[3], (char *)0);
        error_y_exit("execlp", 1);
    }

    int exit_status, exit_code = 0;
    if (waitpid(pid,&exit_status,0) < 0) error_y_exit("waitpid",1);

    if (WIFEXITED(exit_status)) exit_code = WEXITSTATUS(exit_status);
    
    if (pid_2 != 0) {
        if (exit_code == 1) kill(pid_2,SIGUSR1);
        else if (exit_code == 0) kill(pid_2,SIGUSR2);
    }

    char buff [256];
    sprintf(buff, "Termina grep palabra %s fichero %s : exit %d\n", argv[2], argv[3], exit_code);
    write(1, buff, strlen(buff));

}