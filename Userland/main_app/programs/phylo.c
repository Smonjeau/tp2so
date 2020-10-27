// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//-V::776,576

/*Solución basada en la propuesta por Tanenmbaum en su libro SISTEMAS OPERATIVOS MODERNOS*/

#include <syscalls.h>
#include <std_lib.h>
#include <std_io.h>
#define LEFT_PHYLO ((phylo->index+ phylo_count-1)% phylo_count)
#define RIGHT_PHYLO ((phylo->index +1) % phylo_count)
#define STARTERS 5
#define MAX_PHYLOS 20

#define KILL_ADMIN { kill(printer_pid);\
                    closeSem(mutex);\
                    closeSem(mutex_for_phylos_table);\
                    kill(-1);\
                    }
                    

typedef enum PhyloState{HUNGRY=0, EATING=1, THINKING=2} PhyloState;

int mutex = 30;
int phylo_sem = 100;
int mutex_for_phylos_table= 40;




typedef struct phylo{
    int pid;
    int sem;
    int index;
    PhyloState state;

}*phylo;

 phylo phylos [MAX_PHYLOS] = {0};

int chars_in_screen = 0;

int phylo_count=0;

void eat (phylo phylo){
    
    //Miro la mesa
    waitSem(mutex_for_phylos_table);
    int condition = (phylo->state == HUNGRY && phylos[LEFT_PHYLO]->state != EATING
        && phylos[RIGHT_PHYLO]->state != EATING);
    postSem(mutex_for_phylos_table);
    if(condition){
        // Podemos comer
        nap(1000000);
        phylo->state = EATING;

        postSem(phylo->sem);
    }
}

void grab_fork(phylo phylo){
    waitSem(mutex);
    phylo->state=HUNGRY;
    eat(phylo);
    postSem(mutex);
    waitSem(phylo->sem);
    nap(100000000);


}
void drop_fork (phylo phylo){
    waitSem(mutex);
    nap(100000000);
  
    phylo->state= THINKING;
    eat(phylos[LEFT_PHYLO]);
    eat(phylos[RIGHT_PHYLO]);
    postSem(mutex);

}

void phylo_life(int argc, char ** argv){
    while(1){
        int index= atoi(*argv);
        //sleep
        nap(6000000);
        grab_fork(phylos[index]);
        eat(phylos[index]);
        drop_fork(phylos[index]);
    }
}

void add_phylo(int id){

    openSem(phylo_sem,0);
   phylo new = malloc(sizeof(struct phylo)); //phylos[phylo_count];
 //   phylo new = phylos + phylo_count;
    new->state=THINKING;
    new->sem = phylo_sem++;
//    waitSem(mutex_for_phylos_table); //CAPAZ ES ESTOOo

    new->index=phylo_count;
    new->pid = id;

    phylos[phylo_count++] = new;
  //  postSem(mutex_for_phylos_table); //CAPAZ ES ESTOOo

}
void remove_last_phylo(){
 //   waitSem(mutex_for_phylos_table);

    phylo aux = phylos[--phylo_count];
    closeSem(aux->sem);
    kill(aux->pid);    
    free(aux);
 //  postSem(mutex_for_phylos_table);

}
void status_printer(){
    //Lo hago otro proceso, si hago que lo haga el phyloAdmin no tiene sentido, pq este se bloquea en el getChar, entonces no va a llegar a esta parte
    while(1){
        nap(20000000);
        //Estoy mirando la mesa
        waitSem(mutex_for_phylos_table);
        for(int i=0; i<phylo_count;i++){
            phylos[i]->state == EATING ? putChar('E') : putChar('.');
        }
        putChar('\n');
        postSem(mutex_for_phylos_table);
        nap(20000000);

    }

}
void phyloAdmin(int argc, char ** argv){
    int mutex=10;
    int aux;
    //char * arg;
    char * args [20] = {"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18"};
    int printer_pid = startProcess(status_printer,0,(void*)0,"status_printer",0);

    //Como puede fallar vamos probando hasta que tenemos un id de sem válido
    do{
        aux=openSem(mutex++,1);
    }while(aux == -1);
    mutex--; //para compensar el ultimo incremento
    openSem(mutex_for_phylos_table,1);
    while(phylo_count<STARTERS){
        waitSem(mutex_for_phylos_table); //si me rompo ya saben!!!!!!!
        add_phylo(startProcess(phylo_life,1,args+ phylo_count,"phylo",0));

        postSem(mutex_for_phylos_table);


    }

    //Primero creeé a los 5 filósofos que arrancan
    


    printf("Welcome to the philosophers problem! Press a to add one, r to remove and x to kill the process. Bye!\n",0);
    while(1){
        char c = getChar();
        if(c=='a'){
            waitSem(mutex_for_phylos_table);
            add_phylo(startProcess(phylo_life,1,args+phylo_count,"phylo",0));
            postSem(mutex_for_phylos_table);


        }
        else if(c=='r'){
            waitSem(mutex_for_phylos_table);
            remove_last_phylo();
            int condition = phylo_count == 0;
            postSem(mutex_for_phylos_table);
            if(condition){
            KILL_ADMIN;
            }
        
        }
        else if(c=='x')
        {
            waitSem(mutex_for_phylos_table);
            int aux = phylo_count;
            for(int i=0;i<aux;i++){
                remove_last_phylo();
            }
            postSem(mutex_for_phylos_table);
            KILL_ADMIN;
        }
     


    }
}




 