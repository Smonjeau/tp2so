#include <syscalls.h>
#include <std_io.h>
#include <std_lib.h>
#include <anonymous_image.h>
#include <shell_cmds.h>
#define NULL (void *) 0

/* ------------------------------------------- SHELL UTILITIES ------------------------------------------------------ */

void printHelp(int argc, char **argv){
	printf("---------------------------------------------------\n", 0);

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
	printf("- filter            filters input vowels\n", 0);
	printf("- cat               to exec cat\n", 0);
	printf("- wc                to count lines\n", 0);
    printf("- line              to draw a line (for testing)\n", 0);
    printf("- loop              to start loop (for testing)\n", 0);
    printf("- phyloproblem      to execute phylo. Then use a, r and x\n\n", 0);
    printf("- \"command &\" for BG process\n", 0);
    printf("- \"command1 | command2\" for pipe\n", 0);

	printf("---------------------------------------------------\n", 0);


	kill(-1);
}


void clearScreen(int argc, char **argv){
	putChar('\f');

	kill(-1);
}


void printWarning(int argc, char **argv){
	printf(">> Error:\n", 0, 0);
	printf("Command not found\n", 0);
	printf("If you want to see the command manual type 'help'.\n", 0);

	kill(-1);
}


void printTime(int argc, char **argv){
	Time t;
	getTime(&t);

	printf("\nTime now: %2d:%2d:%2d\n", 0, 3, t.hours, t.minutes, t.seconds);

	kill(-1);
}


/* ------------------------------------------- EXCEPTION TESTING ------------------------------------------------------ */


void divZeroException(int argc, char **argv){
	int a;
	a = 2 / 0;

	kill(-1);
}


void invalidOpcode();

void invOpcodeException(int argc, char **argv){
	invalidOpcode();

	kill(-1);
}


/* ------------------------------------------- USER APPS ------------------------------------------------------ */

void displayImage(int argc, char **argv){

    // drawImage(20, 200, anon_img, anon_width, anon_height);
    
	kill(-1);
}


void line(int argc, char **argv){

	static int c=0;
	c+=1;

	for(int y=50*c; y<50*(c+1); y++)
		for(int x=0; x<1024; x++)
			for(int i=0; i<999; i++)
				draw(x, y, 0xFF0000);

	kill(-1);

}


void filter(int argc, char **argv) {
	char bufferAux[50] = {0}; //Buffer auxiliar donde guardare las vocales
	int idxBuffer = 0;

	char c; //Buffer para letra por letra
	do {
		read(0, &c, 1); //Leo un byte de stdin
		if(c == '\r')
			c = '\n';
		else if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')
			bufferAux[idxBuffer++] = c;
		else if (c == '\b' && idxBuffer > 0)
			idxBuffer--;//bufferAux[--idxBuffer] = 0;
		bufferAux[idxBuffer] = 0;
		if(c != EOT)
			putChar(c); //Al ser hijo de la shell, se hereda tambien el fd 1 STDOUT.
		if(c == '\n' || c == EOT) {
			printf(bufferAux, 0);
			idxBuffer = 0;
			bufferAux[0] = 0;
			putChar('\n');
		}
	} while(c != EOT);


	bufferAux[idxBuffer] = 0;
	
	//Ahora imprimo las vocales
	printf("\n", 0);
	printf(bufferAux, 0); //Nuevamente, hacemos print a STDOUT

	kill(-1); //Exit
}


void cat(int argc, char ** argv) {
	char bufferAux[80] = {0};
	int idxBuffer = 0;

	char c; //Buffer para letra por letra
	do {
		read(0, &c, 1); //Leo un byte de stdin
		if(c == '\r')
			c = '\n';
		else if (c == '\b' && idxBuffer > 0)
			bufferAux[--idxBuffer] = 0;
		else if(c != EOT) {
			putChar(c); //Al ser hijo de la shell, se hereda tambien el fd 1 STDOUT.
			bufferAux[idxBuffer++] = c;
		}
		
			
		if(c == '\n' || c == EOT) {
			bufferAux[idxBuffer] = 0;
			printf(bufferAux, 0);
			idxBuffer = 0;
			bufferAux[0] = 0;
			putChar('\n');
		}
	} while(c != EOT);


	bufferAux[idxBuffer] = 0;

	printf("\n", 0);
	printf(bufferAux, 0); //Nuevamente, hacemos print a STDOUT

	kill(-1); //Exit
}


void wc(int argc, char ** argv) {

	int lines = 1;

	char c;
	do{
		read(0, &c, 1);

		if(c == '\r' || c == '\n'){
			lines += 1;
			c = '\n';
		}
		if(c != EOT)
			putChar(c);
	} while(c != EOT);


	printf("\nLines: %d\n", 1, lines);

	kill(-1);

}


void loop(int argc, char **argv){
  int pid = getPid();
  int aux = 1;
  while(1){
    if(aux%4==0)
      printf("Soy el proceso %d y voy a seguir haciendo esto hasta que me mates con kill eh\n",1,pid);
    else  
      printf("Hola!! Soy el proceso %d. Saludos, en un rato vuelvo!\n",1,pid);
    aux++;
    nap(1000000000);

  }
}

extern void phyloAdmin(int argc, char ** argv);

void phylo(int argc, char **argv){
	phyloAdmin(argc,argv);
}

