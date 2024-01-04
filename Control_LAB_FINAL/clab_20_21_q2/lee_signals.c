#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

void trat_alrm() {

}

void error_y_exit(char *s) {
    char buff[100];
    sprintf(buff, "%s error", s);
    write(1, buff, strlen(buff));
    exit(2);
}

void Usage(){
    char buff[100];
    sprintf(buff,"Invalid argument\n Correct usage: ./lee_signals file_name\n");
    write(1, buff, strlen(buff));
}

int main(int argc, char *argv[]) {
    if (argc != 2) Usage();
    struct sigaction sa;
    sa.sa_handler = trat_alrm;
    sa.sa_flags = 0;
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGALRM);
    if (sigaction(SIGALRM,&sa, NULL) < 0) error_y_exit("sigaction");
    int fd1 = open(argv[1], O_RDONLY);               //datos.int
    int fd2 = open("exit_status.int", O_RDONLY);
    int pipefd = open("mis_eventos", O_WRONLY);     //Se va a bloquear
    int num[2], ret, i = 0;
    while ((ret = read(fd1, num, sizeof(num))) > 0) {
        char buff[100];
        if (write(pipefd, num, sizeof(int) * 2) < 0) error_y_exit("write");         //write a pipe, se comprueba error
        alarm(5);
        sigsuspend(&mask);
        if (read(fd2, num, sizeof(int)) < 0) error_y_exit("read_2");
        sprintf(buff, "El proceso %d con PID %d ha terminado con estado %d", i, num[0], num[1]);
        write(1,buff, strlen(buff));
        ++i;
    }
    if (ret < 0) error_y_exit("read_1");
}