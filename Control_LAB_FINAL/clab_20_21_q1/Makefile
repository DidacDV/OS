
all: spawnA hijo1 spawnB spawnC spawnD spawnF hijo2 echo_bin

spawnA: spawnA.c
	gcc -o spawnA spawnA.c

spawnB: spawnB.c
	gcc -o spawnB spawnB.c

spawnC: spawnC.c
	gcc -o spawnC spawnC.c

spawnD: spawnD.c
	gcc -o spawnD spawnD.c

spawnD: spawnF.c
	gcc -o spawnF spawnF.c

hijo1: hijo1.c
	gcc -o hijo1 hijo1.c

hijo2: hijo2.c
	gcc -o hijo2 hijo2.c

echo_bin: echo_bin.c
	gcc -o echo_bin echo_bin.c

clean: 
	rm spawnA hijo1 spawnB spawnC spawnD spawnF hijo2 echo_bin


