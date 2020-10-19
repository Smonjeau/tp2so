#include <syscalls.h>
#include <std_lib.h>

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID 5


int global;  //shared memory


void slowInc(int *p, int inc){

  int aux = *p;
  aux += inc;

  // yield();

  *p = aux;

}


void inc(int argc, char **argv){
    int sem = atoi(argv[0]);
    int value = atoi(argv[1]);
    int limit = atoi(argv[2]);

   if(sem && openSem(SEM_ID, 1) == -1){
     printf("Error opening sem\n", 0);
     return;
   }
  
   for (int i = 0; i < limit; i++){
    
     if(sem && waitSem(SEM_ID) == -1){
       printf("Error waiting sem\n", 0);
       return;
     }

     slowInc(&global, value);

     if(sem && postSem(SEM_ID) == -1){
       printf("Error posting sem\n", 0);
       return;
     }

   }

   if(sem && closeSem(SEM_ID) == -1){
     printf("Error closing sem\n", 0);
     return;
   }

   printf("Final value: %d\n", 1, global);


  
  kill(-1);

}


void test_sync(){

  int i;

  global = 0;


   printf("CREATING PROCESSES...(WITH SEM)\n", 0);

   for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
     char *argv1[3] = {"1","1","10000"};
     startProcess(inc, 3, argv1, "inc");

     char *argv2[3] = {"1","-1","10000"};
      startProcess(inc, 3, argv2, "dec");
   }
  kill(-1);

}

void test_no_sync(){
    int i;

    global = 0;

    printf("CREATING PROCESSES...(WITHOUT SEM)\n",0);

    for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
        char *argv1[3] = {"0","1","10000"};
        startProcess(inc, 3, argv1, "inc_nosync");

        char *argv2[3] = {"0","-1","10000"};
        startProcess(inc, 3, argv2, "dec_nosync");
    }
    kill(-1);
}