#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void
Usage ()
{
  error (1, 0, "Invalid argument.\nCorrect usage: hijo1 ident");
}

int
main (int argc, char *argv[])
{
  int id, minutes = 0;
  char s[80];

  if (argc != 2)
    Usage ();

  id = atoi (argv[1]);

  sprintf (s, "Hijo1[id=%02d pid=%5d] created\n", id, getpid ());
  if (write (2, s, strlen (s)) < 0)
    error (1, errno, "write");

  while (1)
    {
      sleep (60);

      minutes++;
      sprintf (s, "Hijo1[id=%02d pid=%5d] minutes=%d\n", id, getpid (),
               minutes);
      if (write (1, s, strlen (s)) < 0)
        error (1, errno, "write");
    }
}
