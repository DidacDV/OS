#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>


int id = 0;
int N;

void produce_child() {
    char buff[100];
    int pid = fork();
    if (pid == 0) {
        sprintf(buff, "%d", id);
        execl("hijo1", "hijo1",buff, NULL);
        error(1, errno, "execlp");  
    }
    else if (pid < 0) error(1, errno, "fork");

    sprintf(buff,"Father: %d, Child: %d, with id: %d\n", getpid(), pid, id);
    if (write(2, buff, strlen(buff)) < 0) error(1, errno, "write");

    id++;
}

void usr2() {
    ++N;
    char buff[100];
    sprintf(buff, "Process: %d, has received SIGUSR2, N: %d\n", getpid(), N);
    if (write(2,buff,strlen(buff))< 0) error(1, errno, "write");
    produce_child();
    
}

void Usage() {
    error(1, 0, "Invalid argument.\nCorrect usage: ./spawnA N");
}


int main(int argc, char *argv[]) {

    char buff[120];
    if (argc != 2) Usage();
    N = atoi(argv[1]);

    sprintf(buff, "Process pid=%d created, with N=%d\n", getpid(), N);
    if (write(2,buff,strlen(buff))< 0) error(1,errno,"write");

    struct sigaction sa;
    sa.sa_handler = usr2;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR2,&sa, NULL) < 0) error(1, errno, "sigaction");

    for (int i = 0; i < N; ++i) produce_child();


    while (1) {
        int pid;
        if ((pid = waitpid(-1,NULL,0)) < 0) error(1, errno, "waitpid");
        sprintf(buff, "Process pid=%d has the corpse of %d\n", getpid(), pid);
        if (write(2,buff, strlen(buff)) < 0) error(1, errno, "write");

        produce_child();

    }
  
}