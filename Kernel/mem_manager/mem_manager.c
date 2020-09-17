#define BYTE_SIZE 8
#define MEM_SIZE 256
#define BLOCK_SIZE 8
#define BITMAP_SIZE (MEM_SIZE/BLOCK_SIZE)/BYTE_SIZE
//#include <stdio.h>
//#include <math.h>

char memory[MEM_SIZE] = {0};

char bitmap[BITMAP_SIZE] = {0};

void *malloco(int size){

    int nblocks = ceil(size / BLOCK_SIZE);
    char masks[] = {0x80,0x40,0x20,0x10,0x8,0x4,0x2,0x1};
    int freeBlocks = 0; void *address = memory;
    void * candidate_adrdress=NULL;
    int i,j;

    for( i=0; freeBlocks<nblocks && i<BITMAP_SIZE; i+=1){

        char blocksByte = bitmap[i];

        // Count free contiguous blocks in this byte

        for(j=0; freeBlocks<nblocks && j<BYTE_SIZE; j++,address++){

            if(((blocksByte & masks[j] ) / (int)pow(2,BYTE_SIZE-1-j))  % 2 == 0){
                freeBlocks++;
                candidate_adrdress = address;

            }else{
                candidate_adrdress =NULL;
                freeBlocks=0;
            }
        }

    }



    if(freeBlocks == nblocks) {
        int k=0,l;
        while (k<nblocks){
            for(l=j;k<nblocks && l<BYTE_SIZE;l++){
                //vamos poniendo unos
                bitmap[i] |= masks[l];
                k++;
            }
            j=0;
            i++;
        }
    }
    return  candidate_adrdress;

}