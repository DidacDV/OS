#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

void Usage() {
    error(1, 0, "Invalid argument\n Correct usage: ./dummy seed_number");
}

int main(int argc, char *argv[]) {  
    if (argc != 2) Usage();
    int seed = atoi(argv[1]);
    sleep(seed);
    srand(seed);
    exit(rand()%12);
}
