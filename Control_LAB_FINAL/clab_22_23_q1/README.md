## p1.c

*"En caso de ser “./p1-char.exe” escribirá chars, mientras que si es “./p1-integer.exe”* serán integers." del enunciado.

Cread un symbolic-link de p1

```bash
ln -s p1 p1-char
ln -s p1 p1-integer
```


## p2.c

No se porque pero al crear el fichero ``salida-pid.dat``, se me crea sin permisos de escritura para grupo.

He puesto dos maneras de abrir las pipes <sub> (no se si hay más) </sub>, ``OPCION 1`` y ``OPCION 2``, obviamente solo podeis hacer una a la vez, si poneis las intrucciones comentadas como **opcion 1** quitad las de la **opcion 2** y viceversa.
