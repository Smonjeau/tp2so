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

## Manejo del programa
#### Cambiar de ventana
El programa está dividido en dos pantallas. Para poder acceder a cada una debe usar la tecla:
- **F1** para acceder a la Calculadora
- **F2** para acceder a la Shell

#### Uso de calculadora
Al usar la calculadora se pueden utilizar los siguientes símbolos:

- **{0,...,9}** todos los dígitos para armar números enteros negativos o positivos
- para armar números con parte decimal, se puede utilizar tanto la **coma (,)** como el **punto (.)**
- **+** y **-** para la suma y resta
- **x** (letra minúscula) y **%** para la multiplicación y la división
- **(** y **)** para cambiar la precedencia de los argumentos
- **=** o **Enter** para pedir el resultado

Aplican las mismas reglas que para la matemática.

Se puede borrar con:
- **Backspace** un caracter por vez
- **Esc** la línea entera

Se puede cambiar a la ventana de la Shell con **F2**

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

También se pueden utilizar:
- **F2** para volver a la Calculadora
- **F3** para scrollear la ventana
- **Tab** para guardar el valor de los registros

