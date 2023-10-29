#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>



void error_y_exit(char *msg, int exit_status) {
    
    perror(msg);
    exit(exit_status);
}

void usage() {

    char buff[256];

    sprintf(buff,"Usage: PID word file_name\n");
    write(1,buff,strlen(buff));
    exit(1);
}

void main(int argc, char *argv[]) {

    if (argc != 4) usage();
    int pid = fork();
    if (pid < 0) error_y_exit("fork", 1);
    else if (pid == 0) {
        execlp("grep","grep",argv[2], argv[3], (char *)0);
        error_y_exit("execlp", 1);
    }
    int exit_status, exit_code = 0;
    if (waitpid(pid,&exit_status,0) < 0) error_y_exit("waitpid",1);

    if (WIFEXITED(exit_status)) exit_code = WEXITSTATUS(exit_status);
    
    char buff [256];

    sprintf(buff, "Termina grep palabra %s fichero %s : exit %d\n", argv[2], argv[3], exit_code);
    write(1, buff, strlen(buff));

}