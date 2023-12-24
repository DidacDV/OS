#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <error.h>
#include <errno.h>

int total;

void trat_alarm() {

}

void trat_usr1() {
    char buff[100];
    total += 5 - alarm(0);
    sprintf(buff, "%d segundos en ejecucion\n", total);
    write(1,buff, strlen(buff));
    exit(0);
}

void Usage() {
    error(1, 0, "Invalid argument. \n Correct usage: ./max_5sec PID1 PID2 ... PIDN");
}

int main(int argc, char *argv[]) {
    char buff[256];
    struct sigaction sa; 
    int pid;
    if (argc < 2) Usage();
    sa.sa_handler = trat_alarm;
    sa.sa_flags = 0;
    sigset_t mask;

    if (sigaction(SIGALRM, &sa, NULL) < 0) error(1, errno, "sigaction alarm");
    
    sa.sa_handler = trat_usr1;
    if (sigaction(SIGUSR1, &sa, NULL) < 0) error(1, errno, "sigaction usr1");

    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGALRM);
    while (1) {
        alarm(5);
        sigsuspend(&mask);
        total += 5;
        pid = fork();
        if (pid < 0) error(1, errno, "fork");
        else if (pid == 0) {
            execvp("./nproc_time_max",argv);
            error(1, errno, "execvp");
        }    
    }
}
