# Manual de usuario

## Pasos previos
Para garantizar que el funcionamiento del TPE es necesario utilizar docker. Se puede instalar desde:
```
https://www.docker.com/
```

## Compilar y ejecutar
Utilice el siguiente comando en su terminal para compilar y ejecutar el programa desde la carpeta arqui-tpe

``` 
sudo ./run.sh
```

Se puede borrar con:
- **Backspace** un caracter por vez
- **Esc** la línea entera

#### Implementación actual
Contamos con un MM manejado por bitmap, un scheduler RR sin prioridades(aunque está armado para agregarlo) y sin estado BLOQUEADO. También está implementado el kill, que recibe el pid del proceso o matar (-1 para matarse a si mismo) y el ps.

#### Uso de la Shell
Al usar la Shell se pueden utilizar los siguientes comandos

- **cputemp** para leer la temperatura del CPU
- **help** para acceder al manual de ayuda
- **printmem n** para imprimir 32 bytes de memoria a partir de **n**, donde n es un número hexadecimal que tiene el formato 0x...
- **cpuinfo** para visualizar la marca y el modelo del procesador
- **inforeg** para imprimir el valor de todos los
registros
- **storedreg** para imprimir el valor de todos los
registros guardados en cierto momento
- **time** para desplegar la hora del sistema
- **divzero** para invocar a al interrupción de dividir por cero
- **invopcode** para invocar a la interrupción de realizar una operación inválida
- **display anon** para imprimir una imagen sorpresa
- **display matrix** para imprimir una imagen sorpresa
- **clear** para limpiar la pantalla
- **mem** para consultar el estado del heap
- **line** para dibujar unas líneas en la pantalla
- **ps** para obtener info de los procesos


También se pueden utilizar:
- **F2** para volver a la Calculadora
- **F3** para scrollear la ventana
- **Tab** para guardar el valor de los registros

