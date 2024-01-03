> [!WARNING]
> No esta terminado


## n_espera_sig
Para probar si funciona, <sub> En segundo plano </sub>
```bash
./n_espera_sig 3&
```

Cada vez que mateis a un hijo, usad
```bash
xxd exit_status.int
```
y comprobad que se escribe los 8 bytes de cada proceso en la posicion correcta.


## lee_signals
crear la pipe
```bash
mknod mis_eventos p
```
