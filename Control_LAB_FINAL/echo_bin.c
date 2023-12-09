
#include <error.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int
main (int argc, char *argv[])
{
  int i;

  for (i = 1; i < argc; i++)
    {
      int N = atoi (argv[i]);

      if (write (1, &N, sizeof (int)) < 0)
        error (1, errno, "write");
    }
}
