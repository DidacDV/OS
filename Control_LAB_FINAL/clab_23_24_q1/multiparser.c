#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int i=0, npd;

void Usage(char *param){
	char buf[256];
	int len;
	
	len = sprintf(buf, "El programa %s necessita com a minim un parametre\n%s nom_fitxer [nom_fitxers...]\n", param, param);
	write(1, buf, len);
}

void func(int s){
	char buf[256];
	int len;
	
	len = sprintf(buf, "Despertant proces %d\n", i);
	write(1, buf, len);
	if ((npd = open("MIPIPE", O_WRONLY)) < 0){
		perror("Error en open file pipe parent");
		exit(1);
	}	
}

int main(int argc, char **argv){
	int pd[2], fd, len, status, pid;
	char c, buf[256];
	sigset_t mask;
	struct sigaction sa;
	
	if (argc==1){
		Usage(argv[0]);
		return 1;
	}
	
	if (sigfillset(&mask) < 0){
		perror("Error en sigfillset");
		return 1;
	}

	if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0){
		perror("Error en sigprocmask");
		return 1;
	}

	if (sigdelset(&mask, SIGALRM) < 0){
		perror("Error en sigdelset");
		return 1;
	}
	if (sigemptyset(&sa.sa_mask) < 0){
		perror("Error en sigemptyset");
		return 1;
	}
	sa.sa_flags = 0;
	sa.sa_handler = func;
	if (sigaction(SIGALRM, &sa, NULL) <0){
		perror("Error en sigaction");
		return 1;
	}
	if (mknod("MIPIPE", S_IFIFO|0664, 0) < 0){
		if (errno != EEXIST){
			perror("Error en mknod");
			return 1;
		}
	}
	
	for (i=1; i<argc; i++){
		if (pipe(pd) < 0){
			perror("Error en pipe");
			return 1;
		}
		
		pid = fork();
		
		switch(pid){
			case -1: //Error
				perror("Error en fork");
				return 1;
			case 0: //Child
				close(pd[1]);
				close(0); dup(pd[0]); close(pd[0]);
				sprintf(buf, "%s.out", argv[i]);
				if ((fd = open(buf, O_WRONLY|O_TRUNC|O_CREAT, 0640)) < 0){
					perror("Error en open file child");
					return 1;
				}
				dup2(fd, 4); close(fd);
				if ((npd = open("MIPIPE", O_RDONLY)) < 0){
					perror("Error en open file pipe en child");
					return 1;
				}
				len = sprintf(buf, "Proces child %d desbloquejat\n", i);
				write(1, buf, len);
				execlp("./parser", "./parser", NULL);
				perror("Error en execlp");
				return 1;
			default: //Parent
				alarm(1);
				sigsuspend(&mask);

				close(pd[0]);
				if ((fd = open(argv[i], O_RDONLY)) < 0){
					perror("Error en open file parent");
					return 1;
				}
				while(read(fd, &c, 1) > 0){
					write(pd[1], &c, 1);
				}
				close(fd);
				close(pd[1]);
								
				if (waitpid(-1, &status, 0) < 0){
					perror("Error en waitpid");
					return 1;
				}
				
				//Check waitpid
				if (WIFEXITED(status)){
					len = sprintf(buf, "Proces child %d amb pid %d ha finalitzat voluntariament\n", i, pid);
				} else {
					len = sprintf(buf, "Proces child %d amb pid %d ha finalitzat involuntariament\n", i, pid);
				}
				write(1, buf, len);
				close(npd);//Per evitar acumular file descriptors
		}
	}
	
	len = sprintf(buf, "Final execucio parent\n");
	write(2, buf, len);
	
	return 0;
}
