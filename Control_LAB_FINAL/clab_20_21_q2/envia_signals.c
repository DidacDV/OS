#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>


void error_y_exit(char *s) {
    char buff[100];
    sprintf(buff, "%s error", s);
    write(1, buff, strlen(buff));
    exit(2);
}

int main (int argc, char *argv[]) {
    int fd, num[2], ret;
    if ((fd = open("mis_eventos", O_RDONLY)) < 0) error_y_exit("open");

    while ((ret = read(fd, num, sizeof(num))) > 0) {
        char buff[100];
        kill(num[0], num[1]);
        sprintf(buff, "Enviando signal %d a PID %d\n", num[1], num[0]);
        write(1, buff, strlen(buff));
    }
    if (ret < 0) error_y_exit("read");
    exit(0);
}