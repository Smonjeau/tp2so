#define MEM_SIZE 10*1024*1024
#define BLOCK_SIZE 1024
#define BITMAP_SIZE (MEM_SIZE/BLOCK_SIZE)/8

char memory[MEM_SIZE] = {0};

char bitmap[BITMAP_SIZE] = {0};

void *malloc(int size){

    int nblocks = ceil(size / BLOCK_SIZE);

    int freeBlocks = 0; void *address = memory;

    for(int i=0; freeBlocks<nblocks && i<BITMAP_SIZE; i+=1){

        char blocksByte = bitmap[i];

        // Count free contiguous blocks in this byte
        
        for(int j=0; freeBlocks<nblocks && j<8; j++){  

            if((blocksByte >>= 1) % 2 == 0){
                freeBlocks += 1;
            }else{
                freeBlocks = 0;
                address = memory + (i*8+j+1)*BLOCK_SIZE;
            }
            
        }
        
    }

    if(freeBlocks == nblocks){
        // Allocate
    }else{
        // No space
    }

}

// 00011000 00000000
// address = memory
// blockByte = 00000000
// free = 3
// i = 1, j = 1