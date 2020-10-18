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

  // int value = atoi(argv[0]);

  // if(openSem(SEM_ID, 1) == -1){
  //   printf("Error opening sem\n", 0);
  //   return;
  // }
  
  // for (int i = 0; i < 1000; i++){
    
    // if(waitSem(SEM_ID) == -1){
    //   printf("Error waiting sem\n", 0);
    //   return;
    // }

    // slowInc(&global, value);

    // if(postSem(SEM_ID) == -1){
    //   printf("Error posting sem\n", 0);
    //   return;
    // }

  // }

  // if(closeSem(SEM_ID) == -1){
  //   printf("Error closing sem\n", 0);
  //   return;
  // }

  // printf("Final value: %d\n", 1, global);

  // while(1);
  
  kill(-1);

}


void test_sync(){

  int i;

  global = 0;

  startProcess(inc, 0, (void *) 0, "inc");

  // printf("CREATING PROCESSES...(WITH SEM)\n", 0);

  // for(i = 0; i < 1; i++){
  //   char *argv1[1] = {"1"};
  //   startProcess(inc, 3, argv1, "inc");

  //   // char *argv2[1] = {"-1"};
  //   // startProcess(inc, 3, argv2, "dec");
  // }

}