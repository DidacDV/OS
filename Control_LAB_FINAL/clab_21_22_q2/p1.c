#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>


void Usage() {
    error(1, 0, "Invalid argument\n Correcr usage: ./p1 X Y {X -> natural, Y -> integer}");
}


int main(int argc, char *argv[]) {
    int exit_code = 0;
    if (argc != 3) Usage();
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