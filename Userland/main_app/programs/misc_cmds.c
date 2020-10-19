#include <syscalls.h>
#include <std_io.h>
#include <std_lib.h>
#include <anonymous_image.h>
#include <shell_cmds.h>
#define NULL (void *) 0

/* ------------------------------------------- SHELL UTILITIES ------------------------------------------------------ */

void printHelp(void){
	printf("---------------------------------------------------\n", 0);

	printf("- help              to go to the Help Manual\n", 0);
	printf("- clear             to clear the screen\n\n", 0);
	
	printf("- time              to get the system time\n", 0);
	printf("- divzero           to execute a Div by Zero\n", 0);
	printf("- invopcode         to exectue an invalid opcode\n\n", 0);
	
	printf("- display anon      to show an image of Anonymous\n", 0);
	printf("- display matrix    to show an image of Matrix\n\n", 0);

	printf("- cputemp           to get the CPU Temp\n", 0);
	printf("- cpuinfo           to get the CPU Brand Data\n", 0);
	printf("- inforeg           to get the Register's Values\n", 0);
	printf("- storedreg         to get the Stored Register's Values\n\n", 0);
	
	printf("- memdump n         to print memory starting at n\n", 0);
	printf("- mem               to print heap status\n\n", 0);

	printf("- ps                to list all processes\n", 0);
	printf("- kill pid          to kill a process\n", 0);
	printf("- block pid         to block a process\n", 0);
	printf("- nice pid priority to change priority (100 - 139)\n", 0);
	printf("- pipe              to list all pipes\n", 0);
	printf("- filter            filters input vowels\n", 0);
	printf("- cat               to exec cat\n", 0);
    printf("- line              to draw a line (for testing)\n", 0);
    printf("- loop              to start loop (for testing)\n\n", 0);
		
	printf("---------------------------------------------------", 0);
}


void printWarning(){
	printf(">> Error:\n", 0, 0);
	printf("Command not found\n", 0);
	printf("If you want to see the command manual type 'help'.\n", 0);
}


void printTime(void){
	Time t;
	getTime(&t);

	printf("\nTime now: %2d:%2d:%2d\n", 0, 3, t.hours, t.minutes, t.seconds);
}


/* ------------------------------------------- EXCEPTION TESTING ------------------------------------------------------ */


void divZeroException(void){
	int a;
	a = 2 / 0;
}


void invalidOpcode();

void invOpcodeException(void){
	invalidOpcode();
}


/* ------------------------------------------- USER APPS ------------------------------------------------------ */

void displayImage(char *name, int x, int y){

    // drawImage(x, y, anon_img, anon_width, anon_height);
    
}


void line(int argc, char **argv){

	static int c=0;
	c+=1;

	for(int y=50*c; y<50*(c+1); y++)
		for(int x=0; x<1024; x++)
			for(int i=0; i<999; i++)
				draw(x,y,0xFF0000);

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
		if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')
			bufferAux[idxBuffer++] = c;
		putChar(c); //Al ser hijo de la shell, se hereda tambien el fd 1 STDOUT.
	} while(c != '\n');


	bufferAux[idxBuffer] = 0;
	//Ahora imprimo las vocales
	printf(bufferAux, 0); //Nuevamente, hacemos print a STDOUT

	kill(-1); //Exit
}
void cat(int argc, char ** argv) {
	char bufferAux[80] = {0}; //Buffer auxiliar donde guardare las vocales
	int idxBuffer = 0;
	char c; //Buffer para letra por letra
	do {
		read(0, &c, 1); //Leo un byte de stdin
		if(c == '\r')
			c = '\n';
		bufferAux[idxBuffer++] = c;
		putChar(c); //Al ser hijo de la shell, se hereda tambien el fd 1 STDOUT.
	} while(c != '\n');


	bufferAux[idxBuffer] = 0;
	//Ahora imprimo las vocales
	printf(bufferAux, 0); //Nuevamente, hacemos print a STDOUT

	kill(-1); //Exit
}

void startFilter() {
	startProcess(filter, 0, NULL, "filter", 0); //Creo proceso filter. Hereda fds.
}

void startCat() {
	startProcess(cat, 0, NULL, "cat", 0); //Creo proceso cat. Hereda fds.
}

void nap(uint64_t limit){
    for (int i =0;i<limit;i++);
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


void phylo(){

}

void phyloAdmin(int argc, char ** argv){
    // int phylo_count = 0;
    // int mutex=8;
    // int aux;
    // //Como puede fallar 
    // do{
    //     aux=openSem(mutex++,1)
    // }while(aux == -1);
    // mutex--;

    // while(phylo_count<5){
    //     createProcess(phylo, 0, (void *) 0, "phylo");
    // }
    // //Primero creo a los 5 filósofos que arrancan
    // //Leo a ver si tengo que agregar más o sacar
    // while(1){
    //     read(0,)...
    // }
}