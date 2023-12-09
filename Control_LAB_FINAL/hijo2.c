#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void
Usage ()
{
  error (1, 0, "Invalid argument.\nCorrect usage: hijo1 ident");
}

int
main (int argc, char *argv[])
{
  int id, minutes = 0, fd;
  char s[80];

  if (argc != 2)
    Usage ();

  id = atoi (argv[1]);

  sprintf (s, "Hijo2[id=%02d pid=%5d] created\n", id, getpid ());
  if (write (2, s, strlen (s)) < 0) error (1, errno, "write");

  if ((fd = open("input.txt", O_RDONLY)) < 0) error(1, errno, "open");


  while (1) {
     int pos;
     char c;
     if (read(0,&pos, sizeof(int))<0) error(1, errno, "read std");

     if (lseek(fd,pos,SEEK_SET) < 0) error (1, errno, "lseek");

     if (read(fd, &c, sizeof(char)) < 0) error(1, errno, "read fd");

    sprintf(s, "Hijo2[id=%02d pid=%5d] pos=%d char=%c", id, getpid(), pos, c);
    if (write(2,s,strlen(s)) < 0) error(1, errno, "write");
  }
}
