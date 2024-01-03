#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

void trat_sig(int s) {
    char buff[100];
    if (s == SIGINT) {
        sprintf(buff, "Terminado por SIGINT\n");
        write(2, buff, strlen(buff));
        exit(1);
    }
    else if (s == SIGUSR1) {
        sprintf(buff, "Terminado por SIGUSR1\n");
        write(2, buff, strlen(buff));
        exit(2);
    }
    else if (s == SIGUSR2) {
        sprintf(buff, "Terminado por SIGUSR2\n");
        write(2, buff, strlen(buff));
        exit(3);
    }
}

void error_y_exit(char *s) {
    char buff[100];
    sprintf(buff, "%s error", s);
    write(1, buff, strlen(buff));
    exit(2);
}

int main(int argc, char *argv[]) {
    sigset_t mask, mask_o;
    sigfillset(&mask_o);
    if (sigprocmask(SIG_BLOCK, &mask_o, NULL) < 0) error_y_exit("sigprocmask");
    struct sigaction sa;
    sa.sa_handler = trat_sig;
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) < 0) error_y_exit("sigaction_1");
    if (sigaction(SIGUSR1, &sa, NULL) < 0) error_y_exit("sigaction_2");
    if (sigaction(SIGUSR2, &sa, NULL) < 0) error_y_exit("sigaction_3");

    sigfillset(&mask);
    sigdelset(&mask, SIGINT);
    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGUSR2);
    if (sigsuspend(&mask) < 0) error_y_exit("sigsuspend");
}