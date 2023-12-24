#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <signal.h>

void trat_usr1() {
    char buff[256];
    sprintf(buff, "El proceso %d comienza\n", getpid());
    write(1, buff, strlen(buff));
}


void Usage() {
    error(1, 0, "Invalid argument.\n 1.Correct usage: ./p1 number type -> c or i\n 2.Correct usage: ./p1-char number or ./p1-integer number");
}

int main(int argc, char *argv[]) {
    char c;
    if (argc == 1 || argc > 3 || (argc == 3 && (strcmp(argv[2],"c") != 0 && strcmp(argv[2],"i") != 0)) || (argc == 2 && (strcmp(argv[0],"./p1-char") != 0 && strcmp(argv[0],"./p1-integer") != 0))) Usage();

    sigset_t mask;
    struct sigaction trat;
    trat.sa_handler = trat_usr1;
    trat.sa_flags = 0;              //Not necessary
    if (sigaction(SIGUSR1, &trat, NULL) < 0) error(1, errno, "sigaction");
    sigfillset(&mask);
    sigdelset(&mask,SIGUSR1);
    sigsuspend(&mask);

    int n_elem = atoi(argv[1]);
    if ((argc == 3 && strcmp(argv[2],"c") == 0) || strcmp(argv[0],"./p1-char") == 0) {       //write char  
        for (int i = 0; i < n_elem; ++i) {
            if (i < 10) {
                c = '0' + i;
                write(10, &c, sizeof(char));
            }
            else {
                c = '0' + (i/10);
                write(10, &c, sizeof(char));
                c = '0' + (i%10);
                write(10, &c, sizeof(char));
            }
        }

    }
    else {                                                                //write integer
        for (int i = 0; i < n_elem; ++i) {   
            write(10, &i,sizeof(int));                                    //leer a partir de la pàg. 57 de Documentación_labs.pdf (para entender, si quereis)
        }
    }

    char buff[256];
    sprintf(buff, "El proceso %d ha terminado\n", getpid());
    write(1, buff, strlen(buff));
    exit(n_elem);

}
