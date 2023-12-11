all: proc_time try nproc_time nproc_time_max

proc_time: proc_time.c
	gcc -o proc_time proc_time.c

nproc_time: nproc_time.c
	gcc -o nproc_time nproc_time.c

nproc_time_max: nproc_time_max.c
	gcc -o nproc_time_max nproc_time_max.c

try: try.c
	gcc -o try try.c

clean: 
	rm proc_time try nproc_time nproc_time_max
