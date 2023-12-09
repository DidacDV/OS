## Ejercicio F)
cread la pipe con nombre en la consola de comandos:
```bash
mknod NP p
```
NP es el nombre del bloque especial.

``p`` indica es un bloque especial tipo FIFO, una pipe

Despues para probar el programa:

```bash
./spawnF 2&
./echo_bin 3 > NP
```
Ponemos ``&`` para ejecutar en segundo plano (asi no tenemos que abrir otra terminal).

echo_bin nos tranforma el entero a binario para que nuestra pipe NP.
