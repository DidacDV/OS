#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>



void Usage() {
    error(1,0,"Invalid argument. \n Correct usage: ./proc_time PID");
}

int main(int argc, char *argv[]) {
    char buff[100], s[100];
    char c;
    if (argc != 2) Usage();
    int pid = atoi(argv[1]);                                     //could not be necessary
    sprintf(buff,"/proc/%d/stat",pid);

    int fd = open(buff,O_RDONLY);
    if (fd < 0) exit(255);

    int i = 0;
    while (i < 13) {                                                //13 spaces to value nº14 
        if (read(fd, &c, sizeof(char)) < 0) error(1, errno, "read");
        if (c == ' ') ++i;
    }
    i = 0;
    int p = 0, space;
    char number[100];
    while (i < 2) {
        if (read(fd, &c, sizeof(char)) < 0) error(1, errno, "read");
        if (c == ' ') {
            ++i;
            if (i == 1) space = p;
        }
        number[p] = c;
        ++p;
    }
    unsigned long t = atoi(&number[0]);
    t += atoi(&number[space]);
    int time = t/sysconf(_SC_CLK_TCK);              //command to convert to seconds (detailed on man proc) 
    


    //sprintf(s,"Tiempo en ejecución: %d segundos\n", time);      //Hasta que no llegueis al ejercicio 3 podeis usar este 
    sprintf(s,"%d", time);                                        
    if (write(1, s, strlen(s)) < 0) error(1,errno, "write");

}
