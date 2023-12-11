> [!IMPORTANT]
> try.c is only to have a process running to check the time at the first exercise (you can use it at other ones as well)
> 
> Furthermore you can execute it many times to have several process running, and check the max time exercice.

## nproc_time_max.c
``mknod()`` is on comment because at the first time you execute it the pipe will be created and at the second time would be no need to execute again ``mknod()``, what's more an error will be raised indicating so.
> This is only important if you execute several times this ``.c``  👍

