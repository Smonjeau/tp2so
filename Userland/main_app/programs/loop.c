#include <syscalls.h>
#include <stdint.h>
#include <std_lib.h>

void nap(uint64_t limit){
    for (int i =0;i<limit;i++);
}

void loop(){
  int pid = getPid();
  int aux = 1;
  while(1){
    if(aux%4==0)
      printf("Soy el proceso %d y voy a seguir haciendo esto hasta que me mates con kill eh\\n",1,pid);
    else  
      printf("Hola!! Soy el proceso %d. Saludos, en un rato vuelvo!\\n",1,pid);
    aux++;
    nap(1000000000);

  }
}