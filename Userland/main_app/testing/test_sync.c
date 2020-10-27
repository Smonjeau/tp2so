// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//-V::576

#include <syscalls.h>
#include <std_lib.h>

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID 10


int global;  //shared memory


void inc(int argc, char **argv){

    int sem = atoi(argv[0]);
    int value = atoi(argv[1]);
    int limit = atoi(argv[2]);

    if(openSem(SEM_ID, 1) == -1){
        printf("Error opening sem\n", 0);
        kill(-1);
    }

    for (int i = 0; i < limit; i++){
      
      if(sem && waitSem(SEM_ID) == -1){
        printf("Error waiting sem\n", 0);
        kill(-1);
      }

      global += value;

      if(sem && postSem(SEM_ID) == -1){
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

  int i;

  global = 0;

  printf("CREATING PROCESSES...(WITH SEM)\n", 0);

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    char *argv1[3] = {"1","1","1000000"};
    startProcess(inc, 3, argv1, "inc", 0);

    char *argv2[3] = {"1","-1","1000000"};
    startProcess(inc, 3, argv2, "dec", 0);
  }

  kill(-1);

}


void test_no_sync(){

    int i;

    global = 0;

    printf("CREATING PROCESSES...(WITHOUT SEM)\n",0);
    
    char str[3];
    for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
        itoa(i,str,10,-1);
        char *argv1[3] = {"0","1","1000000"};
        startProcess(inc, 3, argv1, "inc_nosync", 0);

        char *argv2[3] = {"0","-1","1000000"};
        startProcess(inc, 3, argv2, "dec_nosync", 0);
    }

    kill(-1);

}