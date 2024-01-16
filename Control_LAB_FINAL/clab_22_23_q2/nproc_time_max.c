#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void Usage() {
    error(1,0, "Invalid argument. \n Correct usage: ./nproc_time PID1 PID2 ... PIDN");
}


int main(int argc, int *argv[]) {
    int max_time = 0, fd1, fd2, t, ret;
    char buff[100], number[100];
    if (argc < 2) Usage();
    int exit_stat, exit_code;
    if (mknod("./PipeA", S_IRUSR|S_IWUSR|__S_IFIFO, 0) < 0 && errno != EEXIST) error(1, errno, "mknod");
   
    for (int i = 1; i < argc; ++i) {
        int pid = fork();
        if (pid < 0) error(1,errno, "fork");
        else if (pid == 0) {
            close(1);
            if ((fd2 = open("PipeA" ,O_WRONLY)) < 0) error(1, errno, "open write");
            execlp("./proc_time","proc_time", argv[i], NULL);
            error(1, errno, "execlp");
        }
        else {
            if ((fd1 = open("PipeA", O_RDONLY)) < 0) error(1, errno, "open read");
            if ((ret = read(fd1, number, sizeof(number))) < 0) error(1, errno, "read");
            number[ret] = '\0';
            t = atoi(number);
            if (t > max_time) max_time = t;
            if (waitpid(pid, &exit_stat, 0) < 0) error(1, errno, "waitpid");
            if (WIFEXITED(exit_stat)) {
                exit_code = WEXITSTATUS(exit_stat);
                if (exit_code == 255) error(1,0,"There is a not existent PID");
            }
        }
    }
    sprintf(buff, "Max_time: %d\n",max_time);
    if (write(1,buff, strlen(buff)) < 0) error(1, errno, "write");

}
