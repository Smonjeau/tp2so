#include <std_io.h>
#include <std_lib.h>
#include <syscalls.h>
#include <test_util.h>

#define NULL ((void *) 0)

//TO BE INCLUDED
void endless_loop(){
  while(1);
}

uint32_t my_create_process(int rq){
  return startProcess(endless_loop, 0, NULL, "endless_loop", 1);
}

uint32_t my_kill(uint32_t pid){
  return kill(pid);
}

uint32_t my_block(uint32_t pid){
  return block(pid);
}

uint32_t my_unblock(uint32_t pid){
  return block(pid);
}

#define MAX_PROCESSES 10 //Should be around 80% of the the processes handled by the kernel

enum State {SERROR, SRUNNING, SBLOCKED, SKILLED};

typedef struct P_rq{
  uint32_t pid;
  enum State state;
}p_rq;


void test_proc(){

  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;

  printf("Begin of process test\n", 0);

  while (1){

    // Creates MAX_PROCESSES processes

    for(rq = 0; rq < MAX_PROCESSES; rq++){

      p_rqs[rq].pid = my_create_process(rq);  // TODO: Port this call as required

      if (p_rqs[rq].pid == -1){
        printf("ERROR creating process\n", 0);
        return;
      }else{
        p_rqs[rq].state = SRUNNING;
        alive++;
      }

    }

    // Randomly kills, blocks or unblocks processes until every one has been SKILLED

    while (alive > 0){

      for(rq = 0; rq < MAX_PROCESSES; rq++){

        action = GetUniform(2) % 2; 

        switch(action){

          // case 0:
          //   if (p_rqs[rq].state == SRUNNING || p_rqs[rq].state == SBLOCKED){

          //     if (my_kill(p_rqs[rq].pid) == -1){
          //       printf("ERROR killing process\n", 0);
          //       return;
          //     }

          //     p_rqs[rq].state = SKILLED; 
          //     alive--;
          //   }
          //   break;

          case 1:
            if (p_rqs[rq].state == SRUNNING){

              if(my_block(p_rqs[rq].pid) == -1){
                printf("ERROR blocking process\n", 0);
                return;
              }

              p_rqs[rq].state = SBLOCKED; 
            }
            break;

        }

      }

      // Randomly unblocks processes

      for(rq = 0; rq < MAX_PROCESSES; rq++){
        if (p_rqs[rq].state == SBLOCKED && GetUniform(2) % 2){

          if(my_unblock(p_rqs[rq].pid) == -1){
            printf("ERROR unblocking process\n", 0);
            return;
          }

          p_rqs[rq].state = SRUNNING; 

        }
      }

    }

  }
}
