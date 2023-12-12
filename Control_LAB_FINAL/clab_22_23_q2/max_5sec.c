#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <error.h>
#include <errno.h>

void trat_alarm() {

}

void trat_usr1() {
    char buff[100];
    sprintf(buff, "%d", getpid());
    execl("nproc_time","./nproc_time",buff,NULL);
}

void Usage() {
    error(1, 0, "Invalid argument. \n Correct usage: ./max_5sec PID1 PID2 ... PIDN");
}

int main(int argc, int *argv[]) {
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
        pid = fork();
        if (pid < 0) error(1, errno, "fork");
        else if (pid == 0) {
            execl("nproc_time_max", "./nproc_time_max", ,NULL);
            error(1, errno, "execlp");
        }    
    }
}