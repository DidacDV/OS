#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define MAXDELAY_S 6

//Al no ser un programa que tengamos que codificar nosotros no pongo Usage ni control de errores

int main(int argc, char *argv[]) {
    int sum = 0, i = 0, ini = 0;
    char c, buff[256], number[512];
    while (read(0, &c, sizeof(char)) > 0) {                
        if (c == '\n') {
            number[i] = c;
            sum += atoi(&number[ini]);
            ini = i + 1;
        }
        else number[i] = c;
        ++i;
    } 
    srand(getpid());
    sleep(rand()%MAXDELAY_S + 1);  
    int n_rand = rand()%10;   
    if (n_rand == 0) {
       sprintf(buff, "%d : %d\n",getpid(), sum);
        write(2, buff, strlen(buff));     
    }
    exit(n_rand);
}