// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//-V::576

#include <stdint.h>
#include <stdio.h>
#include <syscalls.h>

#define MINOR_WAIT 1000000       // To prevent a process from flooding the screen
#define WAIT 50000000            // Long enough to see these processes being run at least twice
#define TOTAL_PROCESSES 3


void bussy_wait(uint64_t n){
  uint64_t i;
  for (i = 0; i < n; i++);
}


void endless_loop_prio(){
  uint64_t pid = getPid();

  while(1){
    printf("%d ", 1, pid);
    bussy_wait(MINOR_WAIT);
  }
}


void test_prio(){

  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  printf("Creating processes...\n\n", 0);

  for(i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = startProcess(endless_loop_prio, 0, (void *) 0, "endless_loop", 0);

  bussy_wait(WAIT);

  printf("\n\nChanging priorities...\n\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        nice(pids[i], 138);
        break;

      case 1:
        nice(pids[i], 119);
        break;
        
      case 2:
        nice(pids[i], 100);
        break;
    }
  }

  bussy_wait(WAIT);

  printf("\n\nBlocking processes...\n\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    block(pids[i]);

  printf("\nChanging priorities while blocked...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
    nice(pids[i], 119);
  }

  printf("\n\nUnblocking processes...\n\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    block(pids[i]);

  bussy_wait(WAIT);

  printf("\n\nKilling processes...\n\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    kill(pids[i]);

  kill(-1);
  
}