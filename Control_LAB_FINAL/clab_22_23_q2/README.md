> [!IMPORTANT]
> try.c is only to have a process running on background in order to check the time at the first exercise (you can use it at the other ones as well)
> 
> Furthermore you can execute it many times to have several process running, and check the max time exercice.

## max_5sec.c
Works but is incorrect, it does not use nproc_time_max program, i do not know how to execute execlp with a variable size of arguments, if anyone who reads this know how to implemnt it, glad to hear it.

> [!NOTE]
> **Update**: According to the teacher the correct implementation of max_5sec.c is using execvp() function which uses a vector as an argument.
