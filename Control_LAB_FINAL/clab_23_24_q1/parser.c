#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void Usage(char *param){
	char buf[256];
	int len;
	
	len = sprintf(buf, "El programa %s necessita ningun o un parametre\n%s [nom_fitxer]\n", param, param);
	write(1, buf, len);
}

int main(int argc, char **argv){
	int fd = 0, offset=0, NLs=0, len, pos=0;
	void *start;
	int *ptr = sbrk(0);
	start = ptr;
	char c, buf[256];
	
	if (argc > 2){
		Usage(argv[0]);
		return 1;
	}
	
	if (argc==2){
		if ((fd = open(argv[1], O_RDONLY)) < 0){
			perror("Error en open");
			return 1;
		}
	}
	
	while(read(fd, &c, 1)>0){
		if (c == '\n'){
			sbrk(sizeof(int));
			ptr[NLs] = pos;
			NLs++;
		}
		pos++;
	}

	write(4, ptr, sizeof(int)*NLs);
	len = sprintf(buf, "El text te %d salts de linia i shan utilitzat %d Bytes del heap\n", NLs, (int)(sbrk(0)-start));
	write(2, buf, len);
	sbrk((-1)*NLs*sizeof(int));
	close(fd);
	return 0;
}
