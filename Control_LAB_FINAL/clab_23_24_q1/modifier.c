#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void Usage(char *param){
	char buf[256];
	int len;
	
	len = sprintf(buf, "El programa %s necessita com a minim dos parametres\n%s nom_fitxer num [num ...]\n", param, param);
	write(1, buf, len);
}

int main(int argc, char **argv){
	int fd = 0, fdint = 0, MAXline=0, offset=0, i=0, len, pos=0;
	char c, buf[256], guio = '-';
	
	if (argc<3){
		Usage(argv[0]);
		return 1;
	}
	
	if ((fd = open(argv[1], O_RDWR)) < 0){
		perror("Error en primer open");
		return 1;
	}
	
	sprintf(buf, "%s.out", argv[1]);
	if ((fdint = open(buf, O_RDONLY)) < 0){
		perror("Error en segon open");
		return 1;
	}
	
	MAXline = lseek(fdint, 0, SEEK_END) / 4;
	
	for (i=2; i<argc; i++){
		pos = atoi(argv[i]);
		if (pos > MAXline){
			len = sprintf(buf, "La linia %d no esta dins del rang daquest fitxer\n", pos);
			write(1, buf, len);
		} else {
			lseek(fdint, (pos-1)*4, SEEK_SET);
			read(fdint, &offset, 4);
			lseek(fd, offset, SEEK_SET);
			write(fd, &guio, 1);
		}
	}
	
	close(fd);
	close(fdint);
	
	return 0;
}
