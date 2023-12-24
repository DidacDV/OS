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
#include <signal.h>

int total;

void trat_alarm() {

}

void trat_usr1() {
    char buff[100];
    total += 5 - alarm(0);
    sprintf(buff, "%d segundos en ejecución\n", total);
    write(1, buff, strlen(buff));
    exit(0);
}


void Usage() {
    error(1,0, "Invalid argument. \n Correct usage: ./nproc_time PID1 PID2 ... PIDN");
}


int main(int argc, int *argv[]) {
    total = 0;
    int max_time = 0, fd1, fd2, t, ret;
    struct sigaction sa;
    sigset_t mask;
    sa.sa_handler = trat_alarm;
    sa.sa_flags = 0;  
    char buff[100], c, number[100];
    if (argc < 2) Usage();
    int exit_stat, exit_code;
    if (mknod("./PipeA", S_IRUSR|S_IWUSR|__S_IFIFO, 0) < 0 && errno != EEXIST) error(1, errno, "mknod");

    if (sigaction(SIGALRM, &sa, NULL) < 0) error(1, errno, "sigaction alarm");
    
    sa.sa_handler = trat_usr1;
    if (sigaction(SIGUSR1, &sa, NULL) < 0) error(1, errno, "sigaction usr1");

    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGALRM);
    
    while (1) { 
        alarm(5);
        sigsuspend(&mask);
        total += 5;
        for (int i = 1; i < argc; ++i) { 
            int p = 0;
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
                while ((ret = read(fd1, &c, sizeof(char))) > 0) {
                    number[p] = c;
                    number[p + 1] = ' ';
                    ++p;
                }
                if (ret < 0) error(1, errno, "read");
                t = atoi(&number[0]);
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

}
