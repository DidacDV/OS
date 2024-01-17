#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

void trat_alrm() {
    char buff[256];
    sprintf(buff, "Tiempo limite\n");
    write(1, buff, strlen(buff));
    exit(0);
}


void Usage() {
    error(1, 0, "Invalid argument\n Correcr usage: ./p3 num_process seed max_seconds file_name");
}


int main(int argc, char *argv[]) {
    if (argc != 5) Usage();   
    int time = atoi(argv[3]);
    alarm(time);
    int exit_code = 0;
    sigset_t mask;
    sigfillset(&mask);
    if (sigprocmask(SIG_SETMASK, &mask, NULL) < 0) error(1, errno, "sigprocmask_1");
    int fd;
    close(1);
    if ((fd = creat(argv[4], S_IRUSR|S_IWUSR)) < 0) error(1, errno, "creat");       // = open(O_CREAT|O_WRONLY|O_TRUNC)
    struct sigaction sa;
    sa.sa_handler = trat_alrm;
    sa.sa_flags = 0;
    sigdelset(&mask, SIGALRM);
    if (sigprocmask(SIG_SETMASK, &mask, NULL) < 0) error(1, errno, "sigprocmask_2");
    sigaction(SIGALRM, &sa, NULL);   
    int n = atoi(argv[1]);
    int seed = atoi(argv[2]);
    for (int i = 0; i < n; ++i) {
        int pid = fork();
        if (pid < 0) error(1,errno, "fork");
        else if (pid == 0) {
            char buff[100];
            sprintf(buff, "%d", seed);
            execlp("./dummy", "dummy",buff, NULL);
            error(1, errno, "execlp");
        }
        else {
            int exit_status;
            if (waitpid(pid, &exit_status, 0) < 0) error(1, errno, "waitpid");
            if (WIFEXITED(exit_status)) {
                exit_code = WEXITSTATUS(exit_status);
                seed += exit_code;
            }
        }
    }

    char buff[256];
    sprintf(buff, "El total es %d\n", seed);
    write(1, buff, strlen(buff));

}
