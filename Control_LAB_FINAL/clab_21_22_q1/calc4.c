#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

//El canal de entrada estandar es el 0, ahora se hace read del fd == 0, en la linea de comandos redirigimos la entrada estandar
//al fichero test00.txt con el comando "< test00.txt". Para que hijo2 lea de la std inp. los numeros que lee calc4 se crea una
//pipe sin nombre con canal de lectura 0 (dup2(fd[0], 0)).

int sahih = 0;           


void check_child(int pid[], int N, int mode) {      //mode == 1 -> Use WNOHANG, mode == 0 -> Do not use it
    char buff[100];
    int exit_code, exit_status, ret;
    if (mode) {
        if ((ret = waitpid(-1, &exit_status, WNOHANG)) < 0) error(1, errno, "waitpid_1_1"); //if ret == 0, none child has died
    }
    else {
        if ((ret = waitpid(-1, &exit_status, 0)) < 0 && errno != ECHILD) error(1, errno, "waitpid_1_0");  
    }
    if (ret > 0 && WIFEXITED(exit_status)) {                                       
        exit_code = WEXITSTATUS(exit_status);
        if (exit_code == 0) {
            if (!sahih) sahih = 1;
            sprintf(buff,"%d ended correctly\n", ret);
            write(2, buff, strlen(buff));
            for (int i = 0; i < N; ++i) {
                if (kill(pid[i], SIGKILL) < 0 && errno != ESRCH) error(1, errno, "kill");   //Meanwhile the main process execute this line
            }                                                                              //child processes could die, ESRCH has to be ignored             
        }
        else  {
            sprintf(buff, "%d wrong execution (exit code %d)\n", ret, exit_code);
            write(2, buff, strlen(buff));
        }
    }
    else if (ret > 0) {
        int sign_code = WTERMSIG(exit_status);
        sprintf(buff,"%d wrong execution (signal %d: %s)\n", ret, sign_code, strsignal(sign_code));
        write(2,buff,strlen(buff));
    }
}


void Usage() {
    char buff[100];
    sprintf(buff,"Invalid argument\n Correct usage: ./calc4 N\n");
    write(1, buff, strlen(buff));
    exit(0);
}


int main(int argc, char *argv[]) {
    if (argc != 2) Usage();
    int N = atoi(argv[1]);
    int pid[N], ret, fd[2];
    char c;
    pipe(fd);
    while ((ret = read(0, &c, sizeof(char))) > 0) write(fd[1],&c, sizeof(char));
    close(fd[1]);
    
    if (ret < 0) error(1, errno, "read");
    for (int i = 0; i < N; ++i) {
        pid[i] = fork();
        if (pid[i] < 0) error(1, errno, "fork");
        else if (pid[i] == 0) {
            close(fd[1]);
            dup2(fd[0], 0);
            execlp("./hijo2", "hijo2", NULL);
            error(1, errno, "execlp");
        }
        else check_child(pid, N, 1);   
    }

    while (errno != ECHILD) check_child(pid, N, 0);
    if (!sahih) write(2, "Incalculable\n", 13);
}