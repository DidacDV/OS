

## n_espera_sig
Para probar si funciona, <sub> En segundo plano </sub>
```bash
./n_espera_sig 3&
```

Cada vez que mateis a un hijo, usad
```bash
xxd exit_status.int
```
y comprobad que se escriben los 8 bytes de cada proceso en la posicion correcta.


## lee_signals
Crear la pipe:
```bash
mknod mis_eventos p
```
# Comprobar que todos los programas funcionan
> [!NOTE]
> Todo en la misma terminal
1. Ejecutad el comando
  ```bash
  rm datos.int
  ```
2. Yo pongo 3 para probar los 3 signals,
  ```bash
  ./n_espera_sig 3&
  ```
3. Ahora os saldran los 3 pids, supongamos PID1, PID2 y PID3
  ```bash
  ./guarda_pares PID1 2
  ./guarda_pares PID2 10
  ./guarda_pares PID2 12
  ```
`2` -> **SIGINT**
`10` -> **SIGUSR1**
`12` -> **SIGUSR2**

4. Por ultimo,
  ```bash
  ./envia_signal&
  ./lee_signals datos.int
  ```




