#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

int sahih = 0, *correct_pids;                       //New  

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
            correct_pids[sahih] = ret;                              //New
            ++sahih;                                                //Number of processes ended correctly
            for (int i = 0; i < N; ++i) {
                if (kill(pid[i], SIGKILL) < 0 && errno != ESRCH) error(1, errno, "kill");   //Meanwhile the main process execute this line
            }                                                                              //child processes could die, ESRCH has to be ignored             
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
    sprintf(buff,"Invalid argument\n Correct usage: ./calc1 N file\n");
    write(1, buff, strlen(buff));
    exit(0);
}


int main(int argc, char *argv[]) {
    if (argc != 3) Usage();
    int N = atoi(argv[1]);
    correct_pids = malloc(N*sizeof(int));                               //New
    int pid[N], ret;

    for (int i = 0; i < N; ++i) {
        pid[i] = fork();
        if (pid[i] < 0) error(1, errno, "fork");
        else if (pid[i] == 0) {
            execlp("./hijo1", "hijo1", argv[2], NULL);
            error(1, errno, "execlp");
        }
        else check_child(pid, N, 1);   
    }

    while (errno != ECHILD) check_child(pid, N, 0);
    if (!sahih) write(2, "Incalculable\n", 13);
    else {                                                              //New
        char buff2[100];
        for (int i = 0; i < sahih; ++i) {
            sprintf(buff2,"%d ended correctly\n", correct_pids[i]);
            write(2, buff2, strlen(buff2));
        }
    }
    
}