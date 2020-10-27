// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//-V::576

#include <syscalls.h>
#include <std_lib.h>
#include <stdint.h>

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID 10000

extern void _timer_tick();

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  aux += inc;
  _timer_tick();
  *p = aux;
}

void inc(int argc, char **argv){

    int sem = atoi(argv[0]);
    int value = atoi(argv[1]);
    uint64_t limit = atoi(argv[2]);

    if(sem && openSem(SEM_ID, 1) == -1){
        printf("Error opening sem\n", 0);
        kill(-1);
    }

    for (uint64_t i = 0; i < limit; i++){
      
      if(sem && waitSem(SEM_ID)){
        printf("Error waiting sem\n", 0);
        kill(-1);
      }

      slowInc(&global,value);

      if(sem && postSem(SEM_ID)== -1){
        printf("Error posting sem\n", 0);
        kill(-1);
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

  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...(WITH SEM)\n", 0);

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    char *argv1[3] = {"1","1","10000"};
    startProcess(inc, 3, argv1, "inc", 0);

    char *argv2[3] = {"1","-1","10000"};
    startProcess(inc, 3, argv2, "dec", 0);
  }

  kill(-1);

}


void test_no_sync(){

    uint64_t i;

    global = 0;

    printf("CREATING PROCESSES...(WITHOUT SEM)\n",0);
    
    for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
        char *argv1[3] = {"0","1","10000"};
        startProcess(inc, 3, argv1, "inc_nosync", 0);

        char *argv2[3] = {"0","-1","10000"};
        startProcess(inc, 3, argv2, "dec_nosync", 0);
    }

    kill(-1);

}