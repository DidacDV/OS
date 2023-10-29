#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int arkhal = 0, j = 0, size;
int pids[10];

void trat_alarm(int s) {
    for (int i = 0; i < size; i++) {
        kill(pids[i], SIGKILL);
    }
    j = size;

}

void trat_int (int s) {
    char buff[256];
    sprintf(buff,"He salido de la espera\n");
    write(1,buff,strlen(buff));
    kill(pids[j],SIGUSR1);
    ++j;
}

void trat_usr1 (int s) {
    arkhal = 1;
}

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
    struct sigaction trat;
    sigset_t mask;
    if (argc < 2 || argc > 11) usage();
    int n = argc - 1;
    size = n;
    char buff[256];
    for (int i = 0; i < n; i++) {
        pids[i] = fork();
        if (pids[i] < 0) error_y_exit("fork");
        else if (pids[i] == 0) {
            sigfillset(&mask);
            sigdelset(&mask,SIGUSR1);
            trat.sa_flags = 0;
            trat.sa_handler = trat_usr1;
            if (sigprocmask(SIG_SETMASK,&mask,NULL) < 0) error_y_exit("sigprocmask");
            if (sigaction(SIGUSR1,&trat,NULL) < 0) error_y_exit("sigaction");
            while(!arkhal);
            sprintf(buff,"%s\n",argv[i+1]);
            write(1,buff, strlen(buff));
            exit(i);
        }
    }

    trat.sa_flags = 0;
    trat.sa_handler = trat_int;
    if (sigaction(SIGINT, &trat, 0) < 0) error_y_exit("sigaction");
    
    trat.sa_handler = trat_alarm;
    if (sigaction(SIGALRM, &trat, 0) < 0) error_y_exit("sigaction");
    while (j < n) {
        alarm(2);
        sigfillset(&mask);
        sigdelset(&mask,SIGINT);
        sigdelset(&mask,SIGALRM);
        sigsuspend(&mask); 
    }


    int pid, exit_stat, exit_code, sign_code;
    char buff2 [256];
    int i = 0;
    while (i < n) {
        if ((pid = waitpid(-1,&exit_stat,0)) < 0) error_y_exit("waitpid");
        if (WIFEXITED(exit_stat)) {
            exit_code = WEXITSTATUS(exit_stat);
            sprintf(buff2,"pid: %d w/ exit code: %d\n", pid, exit_code);
        }
        else {
            sign_code = WTERMSIG(exit_stat);
            sprintf(buff2,"pid: %d exited by signal: %d\n", pid, sign_code);
        }
        write(1, buff2, strlen(buff2));
        ++i;
    }
}