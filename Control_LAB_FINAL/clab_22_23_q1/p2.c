#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <error.h>
#include <signal.h>

int go;

void trat_alarm() {
    go = 1;
}


void Usage() {
    error(1, 0, "Inavalid argument.\n Correct usage: ./p2 X Y X Y X Y X Y... (X = NUmber, Y = 'i' or 'c')");
}

int main(int argc, char *argv[]) {
    int fdrd, fdwr;
    sigset_t mask;
    struct sigaction trat;
    trat.sa_flags = 0;
    trat.sa_handler = trat_alarm;
    if (argc == 1 || argc%2 == 0) Usage();

    if (mknod("./MYPIPE",S_IRUSR|S_IWUSR|__S_IFIFO, 0) < 0 && errno != EEXIST) error(1, errno, "mknod");

    for (int i = 1; i < argc; i += 2) {
        if ((fdwr = open("MYPIPE", O_RDWR)) < 0) error(1, errno, "open write");     //IMPORTANTE HA DE SER O_RDWR, si no se queda bloqueado       
        if ((fdrd = open("MYPIPE", O_RDONLY)) < 0) error(1, errno, "open read"); 
        int pid = fork();
        if (pid < 0) error(1, errno, "fork");
        else if (pid == 0) {
            sigemptyset(&mask);
            sigaddset(&mask, SIGUSR1);
            if (sigprocmask(SIG_SETMASK, &mask, NULL) < 0) error(1, errno, "sigprocmask"); 
            dup2(fdwr, 10);
            close(fdrd);
            execlp("./p1", "p1", argv[i], argv[i + 1], NULL);
            error(1, errno, "execlp");

        }
        else {
            int n = atoi(argv[i]);
            if (sigaction(SIGALRM, &trat, NULL) < 0) error(1, errno, "sigaction");
            go = 0;
            alarm(2);
            while(!go);
            kill(pid, SIGUSR1);
            int ret, *elemi;
            char *elemc;
            if (strcmp(argv[i + 1],"c") == 0) {
               char c; 
               int j = 0;
               elemc = malloc(n * sizeof(char));         //En verdad deberia ser más, pero malloc siempre reserva más de lo necesario :)
               close(fdwr);
               while ((ret = read(fdrd, &elemc[j], sizeof(char))) > 0) ++j; 
               elemc[j] = '\0';
               if (ret < 0) error(1, errno, "read char");
            }
            else {
               int j = 0;
               elemi = malloc(n * sizeof(int));
               close(fdwr);
               while ((ret = read(fdrd, &elemi[j], sizeof(int))) > 0) ++j;       
               if (ret < 0) error(1, errno, "read integer");
               
            }
            int exit_status;    
            if (waitpid(pid, &exit_status, 0) < 0) error(1, errno, "waitpid");
            if (WIFEXITED(exit_status)) { 
                char buff[100];
                int fd;    
                sprintf(buff, "salida-%d.dat", pid);                              
                if ((fd = creat(buff,S_IRUSR|S_IWUSR|S_IWGRP|S_IRGRP)) < 0) error(1, errno, "create");        //Se puede hacer con open() tambien 
                if (strcmp(argv[i + 1],"c") == 0) {
                    write(fd, elemc, strlen(elemc));
                    free(elemc);
                }
                else {
                    write(fd,elemi, sizeof(int) * n);
                    free(elemi);
                }

            }

        }

    }

    char buff2[100];
    sprintf(buff2, "Final del proceso principal\n");
    write(1, buff2, strlen(buff2));
}
