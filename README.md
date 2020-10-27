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





	printf("- help              to go to the Help Manual\n", 0);
	printf("- clear             to clear the screen\n\n", 0);

	printf("- time              to get the system time\n", 0);
	printf("- divzero           to execute a Div by Zero\n", 0);
	printf("- invopcode         to exectue an invalid opcode\n\n", 0);

	printf("- cputemp           to get the CPU Temp\n", 0);
	printf("- cpuinfo           to get the CPU Brand Data\n", 0);

	printf("- memdump n         to print memory starting at n\n", 0);
	printf("- mem               to print heap status\n\n", 0);

	printf("- ps                to list all processes\n", 0);
	printf("- kill pid          to kill a process\n", 0);
	printf("- block pid         to block a process\n", 0);
	printf("- nice pid priority to change priority (100 - 139)\n", 0);
	printf("- pipe              to list all pipes\n", 0);
	printf("- sem               to list all sems\n", 0);
	printf("- filter            filters input vowels\n", 0);
	printf("- cat               to exec cat\n", 0);
	printf("- wc                to count lines\n", 0);
    printf("- line              to draw a line (for testing)\n", 0);
    printf("- loop              to start loop (for testing)\n", 0);
    printf("- phyloproblem      to execute phylo. Then use a, r and x\n\n", 0);
    printf("@ \"command &\" for BG process\n", 0);
    printf("@ \"command1 | command2\" for pipe\n", 0);
    printf("@  testing commands available. (Sec. 12. Informe) \n", 0);