#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>



int id = 0;

void produce_child() {
    char buff[100];
    int pid = fork();
    if (pid == 0) {
        sprintf(buff, "%d", id);

        execl("hijo2", "hijo2",buff, NULL);
        error(1, errno, "execlp");  
    }
    else if (pid < 0) error(1, errno, "fork");

    sprintf(buff,"Father: %d, Child: %d, with id: %d\n", getpid(), pid, id);
    if (write(2, buff, strlen(buff)) < 0) error(1, errno, "write");

    id++;
}


void Usage() {
    error(1, 0, "Invalid argument.\nCorrect usage: ./spawnA N");
}


int main(int argc, char *argv[]) {
    int N;
    char buff[120];
    if (argc != 2) Usage();
    N = atoi(argv[1]);

    sprintf(buff, "Process pid=%d created, with N=%d\n", getpid(), N);
    if (write(2,buff,strlen(buff))< 0) error(1,errno,"write");

    int fd1 = open("NP", O_RDWR);    
    int fd2 = open("NP", O_RDONLY);
    if (fd1 < 0 || fd2 < 0) error(1, errno, "open pipe");
    if (dup2(fd2, 0) < 0) error(1, errno, "dup");
    
    for (int i = 0; i < N; ++i) produce_child();

    while (1) {
        int pid;
        if ((pid = waitpid(-1,NULL,0)) < 0) error(1, errno, "waitpid");
        sprintf(buff, "Process pid=%d has the corpse of %d\n", getpid(), pid);
        if (write(2,buff, strlen(buff)) < 0) error(1, errno, "write");

        produce_child();

    }

}