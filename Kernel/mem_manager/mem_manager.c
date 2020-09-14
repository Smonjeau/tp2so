#include  <lib.h>
#define BYTE_SIZE 8
#define MEM_SIZE 10*1024*1024
#define BLOCK_SIZE 1024
#define BITMAP_SIZE (MEM_SIZE/BLOCK_SIZE)/BYTE_SIZE
#define NULL ((void*)0)
#define FIRST_HEAP_ADRESS ((char*) 0x700000)
//#define LAST_HEAP_ADRESS 0x170000 Creo q no hace falta


//char memory[MEM_SIZE] = {0};

char bitmap[BITMAP_SIZE] = {0};


typedef struct Assignation{
    void *startAddress;
    int size;
} Assignation;


Assignation assignations[BITMAP_SIZE];
int assignationCounter = 0;

int occupied_blocks = 0;
char oneMasks[] = {1, 2, 4, 8, 16, 32, 64, 128};
    
char zeroMasks[] = {0b11111110, 0b11111101, 0b11111011, 0b11110111,
    0b11101111, 0b11011111, 0b10111111, 0b01111111};


void malloc(int size, void ** ptr){

    int nblocks = div_ceil(size , BLOCK_SIZE);    // Number of blocks to assing

    int freeBlocks = 0;
    void *blockAdress = FIRST_HEAP_ADRESS, *candidateAddress = NULL;
    int bytePos = 0, bitPos = 0;

    for(bytePos=0; freeBlocks<nblocks && bytePos<BITMAP_SIZE; bytePos+=1){

        char blocksByte = bitmap[bytePos];

        // Count free contiguous blocks in this byte

        for(bitPos=0; freeBlocks<nblocks && bitPos<BYTE_SIZE; bitPos++, blockAdress+=BLOCK_SIZE){

            if((blocksByte >> bitPos) % 2 == 0){
                freeBlocks++;
                candidateAddress = candidateAddress==NULL ? blockAdress : candidateAddress;
            }else{
                candidateAddress =NULL;
                freeBlocks=0;
            }

        }

    }

    if(freeBlocks == nblocks) {

        int k=0;

        // Fill with ones the last blocks byte to assign

        for(int j=bitPos-1; k<nblocks && j>=0; j--, k++){
            bitmap[bytePos-1] |= oneMasks[j];
        }

        // Fill with ones the lefting blocks bytes to assign

        for(int i=bytePos-2; k<nblocks && i>=0; i--){
            for(int j=BYTE_SIZE-1; k<nblocks && j>=0; j--, k++){
                bitmap[i] |= oneMasks[j];
            }
        }

        // Create an assignation record

        Assignation newAssignation;
        newAssignation.size = nblocks;
        newAssignation.startAddress = candidateAddress;

        assignations[assignationCounter++] = newAssignation;
    
        *ptr = candidateAddress;
        occupied_blocks += nblocks;

    }else

      *ptr = NULL;    // Not enough space

    

}


void free(void *address){

    // Get the assignation record

    Assignation assignationRecord; int i;
    for(i=0; i<assignationCounter; i++){
        if(assignations[i].startAddress == address){
            assignationRecord = assignations[i];
            break;
        }
    }

    // Free the memory

    int nblocks = assignationRecord.size; int k=0;

    int bytePos = (((char *)address-FIRST_HEAP_ADRESS)/BLOCK_SIZE) / BYTE_SIZE;

    int bitPos = (((char *)address-FIRST_HEAP_ADRESS)/BLOCK_SIZE) % BYTE_SIZE;

    for(int j=bitPos; k<nblocks && j<BYTE_SIZE; j++, k++){
        bitmap[bytePos] &= zeroMasks[j];
    }

    for(int i=bytePos+1; k<nblocks; i++){
        for(int j=0; k<nblocks && j<BYTE_SIZE; j++, k++){
            bitmap[i] &= zeroMasks[j];
        }
    }
    occupied_blocks -= nblocks;
    // Remove the assignation record

    memcpy(assignations+i, assignations+i+1, sizeof(Assignation));
    assignationCounter -= 1;

}

void mem_status(int * memory_size, int * free_space, int * occupied_space){
    *memory_size = MEM_SIZE;
    *occupied_space = occupied_blocks * BLOCK_SIZE;
    *free_space = (BITMAP_SIZE - occupied_blocks) * BLOCK_SIZE;

}


// nblocks=3
// freeBlocks=3
// blockAddress=0x7  candidateAddress=0x6
// bytePos=2  bitPos=1

// bitmap = 00110011 00001110
// blocksByte = 00001100

// i=0  j=5  k=3

// bitmap = 11110011 00001111


// Fixed the filling of ones
// Fixed bug on block address increment and candidate address update
// Implemented free method
// Changed mask usage with more simple shift on free memory count
// Changed some variable names for better legibility