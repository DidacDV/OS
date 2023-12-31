all: p1 dummy p2 p3 dummy2 p4

p1: p1.c
	gcc -o p1 p1.c

dummy: dummy.c
	gcc -o dummy dummy.c

p2: p2.c
	gcc -o p2 p2.c

p3: p3.c
	gcc -o p3 p3.c

dummy2: dummy2.c
	gcc -o dummy2 dummy2.c

p4: p4.c 
	gcc -o p4 p4.c 

clean:
	rm p1 dummy p2 p3 dummy2 p4