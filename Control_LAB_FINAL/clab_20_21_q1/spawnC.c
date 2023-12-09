#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>


int id = 0, N;
int *pids;

void init_child() {
    pids = malloc(N*sizeof(int));
    if (pids == NULL) error(1, errno, "Max memory reached");
    memset(pids,0, N*sizeof(int));      //We init to 0 the N blocks, this way save the pid is easier (1)
                                        //Instead, you can use a for loop to initialize the whole pointer to 0

}

void child_save(int pid) {
    for (int i = 0; i < N; i++) {
        if (pids[i] == 0) {             // (1)
            pids[i] = pid;
            return;
        }
    }
}

void delete_child(int pid) {
    for (int i = 0; i < N; ++i) {
        if (pids[i] == pid) {
            pids[i] = 0;
            return;
        }
    }
}

void usr1() {
    for(int i = 0; i < N; ++i) {
        char buff[100];
        sprintf(buff, "Father: %d, my child: %d, sigusr1 received\n", getpid(), pids[i]);
        if (write(2, buff, strlen(buff)) < 0) error(1, errno, "write");
    }
}

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

    child_save(pid);

    id++;
}


void Usage() {
    error(1, 0, "Invalid argument.\nCorrect usage: ./spawnA N\n");
}


int main(int argc, char *argv[]) {
    char buff[120];
    if (argc != 2) Usage();
    N = atoi(argv[1]);
    init_child();

    sprintf(buff, "Process pid=%d created, with N=%d\n", getpid(), N);
    if (write(2,buff,strlen(buff))< 0) error(1,errno,"write");

    struct sigaction sa;
    sa.sa_handler = usr1;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) < 0) error(1, errno, "sigaction");


    for (int i = 0; i < N; ++i) produce_child();

    while (1) {
        int pid;
        if ((pid = waitpid(-1,NULL,0)) < 0) error(1, errno, "waitpid");
        sprintf(buff, "Process pid=%d has the corpse of %d\n", getpid(), pid);
        if (write(2,buff, strlen(buff)) < 0) error(1, errno, "write");

        delete_child(pid);
        produce_child();

    }
    
}