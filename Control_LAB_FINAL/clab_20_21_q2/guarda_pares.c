#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <error.h>


void Usage() {
    char buff[100];
    sprintf(buff, "Invalid argument\n Correct usage: ./guarda_pares INT1 INT2\n");
    write(1, buff, strlen(buff));
    exit(1);
}


int main(int argc, char *argv[]) {
    int fd;
    if (argc != 3) Usage();
    if ((fd = creat("datos.int", S_IRUSR|S_IWUSR)) < 0) error(1, errno, "creat");
    int num[argc - 1];
    for (int i = 1; i <= argc - 1; ++i) {
        num[i] = atoi(argv[i]);
        if (write(fd, &num[i], sizeof(int)) < 0) error(1, errno, "write");
    }

}