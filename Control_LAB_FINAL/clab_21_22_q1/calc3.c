#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

int sahih = 0, N, *pid, k;
int time = 0;

void exterminate() {
    for (int i = 0; i < N; ++i) {
        if (kill(pid[i], SIGKILL) < 0 && errno != ESRCH) error(1, errno, "kill");   //Meanwhile the main process execute this line
    }                                                                              //child processes could die, ESRCH has to be ignored 
}

void trat_int() {
    write(2, "SIGINT\n", 7);
    exterminate();
    exit(0);
}

void trat_alrm() {
    char buff[100];
    ++time;
    sprintf(buff, "%d hijo/s y %d segundos en ejecucion\n", k, time);
    write(2,buff, strlen(buff));
    alarm(1);
}

void check_child(int pid[], int N, int mode) {      //mode == 1 -> Use WNOHANG, mode == 0 -> Do not use it
    char buff[100];
    int exit_code, exit_status, ret;
    if (mode) {
        if ((ret = waitpid(-1, &exit_status, WNOHANG)) < 0) error(1, errno, "waitpid_1_1"); //if ret == 0, none child has died
    }
    else {
        if ((ret = waitpid(-1, &exit_status, 0)) < 0 && errno != ECHILD) error(1, errno, "waitpid_1_0");  
    }
    if (ret > 0 && WIFEXITED(exit_status)) {                                       
        exit_code = WEXITSTATUS(exit_status);
        if (exit_code == 0) {
            if (!sahih) sahih = 1;
            sprintf(buff,"%d ended correctly\n", ret);
            write(2, buff, strlen(buff));
            exterminate();                                                                         
        }
        else  {
            sprintf(buff, "%d wrong execution (exit code %d)\n", ret, exit_code);
            write(2, buff, strlen(buff));
        }
    }
    else if (ret > 0) {
        int sign_code = WTERMSIG(exit_status);
        sprintf(buff,"%d wrong execution (signal %d: %s)\n", ret, sign_code, strsignal(sign_code));
        write(2,buff,strlen(buff));
    }
}


void Usage() {
    char buff[100];
    sprintf(buff,"Invalid argument\n Correct usage: ./calc3 N file\n");
    write(1, buff, strlen(buff));
    exit(0);
}


int main(int argc, char *argv[]) {
    if (argc != 3) Usage();
    N = atoi(argv[1]);
    int ret;
    pid = malloc(sizeof(int) * N);
    struct sigaction sa;
    sa.sa_handler = trat_int;
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) < 0) error(1, errno, "sigaction_1");
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = trat_alrm;
    if (sigaction(SIGALRM, &sa, NULL) < 0) error(1, errno, "sigaction_2");
    alarm(1);
    for (k = 0; k < N; ++k) {
        pid[k] = fork();
        if (pid[k] < 0) error(1, errno, "fork");
        else if (pid[k] == 0) {
            execlp("./hijo1", "hijo1", argv[2], NULL);
            error(1, errno, "execlp");
        }
        else check_child(pid, N, 1);   
    }

    while (errno != ECHILD) check_child(pid, N, 0);
    if (!sahih) write(2, "Incalculable\n", 13);
}
