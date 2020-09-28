#include <keyboard_driver.h>

#define BUFFER_LEN 8

int buffer[BUFFER_LEN]={0};
int pos=0;


void handleKeyStroke(){

    buffer[pos++]=read();
    
}


int getKey(){

    if(pos>0)
        return buffer[--pos];
    else
        return -1;

}