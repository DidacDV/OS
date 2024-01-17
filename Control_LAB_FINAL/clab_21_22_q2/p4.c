#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>


void Usage() {
    error(1, 0, "Invalid argument\n Correcr usage: ./p4 X Y {X -> natural, Y -> integer}");
}


int main(int argc, char *argv[]) {
    int exit_code, fd[2], pidn;
    if (argc != 3) Usage();
    int n = atoi(argv[1]);
    int seed = atoi(argv[2]), seed2 = seed;
    pipe(fd);                                                   
    int pid1 = fork();
    if (pid1 < 0) error(1, errno, "fork_1");
    else if (pid1 == 0) {
        int num, ret;
        char buff[100];
        close(fd[1]);                                        
        while ((ret = read(fd[0], &num, sizeof(int))) > 0) {    
            seed2 += num;
        }
        if (ret < 0) error(1, errno, "read");
        sprintf(buff, "Total de la pipe es %d\n", seed2);
        write(1, buff, strlen(buff));
    }
    else {
         close(fd[0]);
        for (int i = 0; i < n; ++i) {     
                int pidn = fork();
                if (pidn < 0) error(1,errno, "fork_2");
                else if (pidn == 0) {
                    char buff[100];
                    sprintf(buff, "%d", seed);
                    dup2(fd[1], 1);
                    close(fd[1]);
                    execlp("./dummy2", "dummy2",buff, NULL);
                    error(1, errno, "execlp");
                }
                else {
                    int exit_status;
                    if (waitpid(pidn, &exit_status, 0) < 0) error(1, errno, "waitpid");
                    if (WIFEXITED(exit_status)) {
                        exit_code = WEXITSTATUS(exit_status);
                        seed += exit_code;
                    }
                }
            
        }
        close(fd[1]);
        char buff[256];
        sprintf(buff, "El total es %d\n", seed);
        write(1, buff, strlen(buff));
    }
    

}
