
## hijo1.c
 - Poned el MAXDELAY_S que querais
 - `test00.txt`, como dice el enunciado, es para probar, vosotros podeis crear más, si quereis, o modificar este, pero separad los numeros por **saltos de linea**.

## Ejemplo de salida de calc1.c
<p align ="center">
<img src="https://github.com/RogerCL24/OS/assets/90930371/ae42ab49-6311-42d1-a823-33aa2c5ab099"/>
</p>

## Ejemplo de salida de calc3.c 
- Con `MAXDELAY_S` = 20, probad el que querais.
<p align="center">
<img src="https://github.com/RogerCL24/OS/assets/90930371/147738df-4012-4aef-91ae-bf96e53ada85"/>
</p>

## calc4.c
La idea seria ejecutar de esta manera calc4
```bash
./calc4 10 < test00.txt
```

## dir.sh
- Suponiendo que sale lo siguiente
<p align="center">
<img src="https://github.com/RogerCL24/OS/assets/90930371/3f29e0f6-73f1-4bc8-b570-4b31e6e98cd1"/>
</p>

Es lo mismo que el numero de referencias en la tabla de inodos de cualquier examen final de teoria de SO:
- El primero `.`: Tiene 10 referencias, él mismo(1) + directorios hijo(8) + directorio padre(1) <sub> Que seria _clab_21_22_q1_ en mi caso.</sub>
- El segundo `..`: Él mismo(1)  <sub> _clab_21_22_q1_ (que es el padre de D1) </sub> + _D1_ (1) + Padre de  _clab_21_22_q1_.
- El tercero `D_1_1`: _D1_(1) <sub> Padre de _D_1_1_ </sub> + _D_1_1_ (1) + subdirectorios de _D_1_1_ (2).
- Y asi con los demas.

 Para borrar los directorios
```bash
rm -R D1
```

