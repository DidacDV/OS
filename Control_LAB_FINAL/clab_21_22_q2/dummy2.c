#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {  
    int seed = atoi(argv[1]);
    sleep(seed);
    srand(seed);
    int random = rand()%10 + 1;         //Always inc
    write(1, &random, sizeof(int));
    exit(random);
}