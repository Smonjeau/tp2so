# Manual de usuario

## Pasos previos
Para garantizar  el funcionamiento del TPE es necesario utilizar docker. Se puede instalar desde:
```
https://www.docker.com/
```

## Compilar y ejecutar
Utilice el siguiente comando en su terminal para compilar y ejecutar el programa desde la carpeta arqui-tpe

``` 
sudo ./run.sh o sudo ./run.sh buddy (el primero compila con bitmap MM, el segundo con buddy MM)
```

Se puede borrar con:
- **Backspace** un caracter por vez
- **Esc** la línea entera

Para correr un proceso en BG, concatenar un " &", ejemplo: "testmm &"
Para pipear dos procesos: p1 | p2




#### Uso de la Shell
Al usar la Shell se pueden utilizar los siguientes comandos

- **cputemp** para leer la temperatura del CPU
- **help** para acceder al manual de ayuda
- **printmem n** para imprimir 32 bytes de memoria a partir de **n**, donde n es un número hexadecimal que tiene el formato 0x...
- **cpuinfo** para visualizar la marca y el modelo del procesador
- **time** para desplegar la hora del sistema
- **divzero** para invocar a al interrupción de dividir por cero
- **invopcode** para invocar a la interrupción de realizar una operación inválida
- **clear** para limpiar la pantalla
- **mem** para consultar el estado del heap
- **line** para dibujar unas líneas en la pantalla
- **ps** para obtener info de los procesos
- **sem** para obtener info de los semáforos
- **pipe** para obtener info de los pipes
- **kill pid** para matar al proceso correspondiente al pid
- **block pid** para bloquear al proceso correspondiente al pid
- **nice pid priority** para cambiarle la prioridad al proceso específicado, (entre 100-139)
- **filter** para filtrar las vocales del input 
- **cat** para imprimir el stdin
- **wc** para contar las líneas del stdin
- **loop** para imprimir un mensaje en loop 




