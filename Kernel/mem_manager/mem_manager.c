#include  <lib.h>

#define NULL ((void*)0)
#define FIRST_HEAP_ADRESS ((void*) 0x800000)
#define MEM_SIZE (8*1024*1024)
int div_ceil(int dividend, int divisor){
    if (dividend%divisor == 0)
        return  dividend / divisor;
    else
        return (dividend / divisor) +1;
}
#ifdef MM_BITMAP

/* ---------------------------------------------------------------------------------------------------------------------------
                                BITMAP IMPLEMENTATION
--------------------------------------------------------------------------------------------------------------------------- */
#define BYTE_SIZE 8
#define BLOCK_SIZE 1024
#define BITMAP_SIZE ((MEM_SIZE/BLOCK_SIZE)/BYTE_SIZE)



char bitmap[BITMAP_SIZE] = {0};


typedef struct Assignation{
    void *startAddress;
    int size;
} Assignation;


Assignation assignations[BITMAP_SIZE];
int assignationCounter = 0;

int occupied_bits = 0;
char oneMasks[] = {1, 2, 4, 8, 16, 32, 64, 128};
    
char zeroMasks[] = {0b11111110, 0b11111101, 0b11111011, 0b11110111,
    0b11101111, 0b11011111, 0b10111111, 0b01111111};






void * malloc(int size) {

    



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

    bytePos--;
    bitPos--;

    if(freeBlocks == nblocks) {

        int k=0;

        // Fill with ones the last blocks byte to assign

        for(int j=bitPos/*-1*/; k<nblocks && j>=0; j--, k++){
            bitmap[bytePos/*-1*/] |= oneMasks[j];
            //occupied_bits++; //Se refiere a los bloques ocupados
        }

        bytePos--;

        /*if(k < nblocks)
            k--; //Compenso*/

        // Fill with ones the lefting blocks bytes to assign

        //________ ________

        for(int i=bytePos/*-2*/; k<nblocks && i>=0; i--){
            for(int j=BYTE_SIZE-1; k<nblocks && j>=0; j--, k++){
                bitmap[i] |= oneMasks[j];
                //occupied_bits++;
            }

            /*if(k < nblocks)
                k--; //Compenso*/
        }

        occupied_bits += nblocks; //nblocks == k

        // Create an assignation record

        Assignation newAssignation;
        newAssignation.size = nblocks;
        newAssignation.startAddress = candidateAddress;

        assignations[assignationCounter++] = newAssignation;

    
        return candidateAddress;

    }

    return NULL;    // Not enough space

    

}

void * mallocFromKernel(int size) {
    _cli();
    void * result = malloc(size);
    _sti();
    return result;
}


void free(void *address){


    // Get the assignation record
    int assignationCounterIndex;

    Assignation assignationRecord; int i;
    for(i=0; i<assignationCounter; i++){
        if(assignations[i].startAddress == address){
            assignationRecord = assignations[i];
            assignationCounterIndex = i;
            break;
        }
    }

    // Free the memory

    int nblocks = assignationRecord.size; int k=0;

    int bytePos = ((address-FIRST_HEAP_ADRESS)/BLOCK_SIZE) / BYTE_SIZE;

    int bitPos = ((address-FIRST_HEAP_ADRESS)/BLOCK_SIZE) % BYTE_SIZE;

    /*_______1 11111111

      _______1
          &
      11111101*/


    for(int j=bitPos; k<nblocks && j<BYTE_SIZE; j++, k++){
        bitmap[bytePos] &= zeroMasks[j];
        //occupied_bits--;
    }

    /*if(k < nblocks)
        k--;*/
    bytePos++;

    for(int i=bytePos; k<nblocks && i < BITMAP_SIZE; i++){
        for(int j=0; k<nblocks && j<BYTE_SIZE; j++, k++){
            bitmap[i] &= zeroMasks[j];
            //occupied_bits--;
        }
    }
    // Remove the assignation record
    occupied_bits -= k;

    //memcpy(assignations+i, assignations+i+1, sizeof(Assignation));
    for(int i = assignationCounterIndex; i < (assignationCounter - 1); i++)
        memcpy(assignations+i, assignations+i+1, sizeof(Assignation));
    
    
    assignationCounter -= 1;

}

void mem_status(int * memory_size, int * free_space, int * occupied_space){
    
    *memory_size = MEM_SIZE / 1024; //en KB
    *free_space = (BITMAP_SIZE*BYTE_SIZE - occupied_bits) * BLOCK_SIZE / 1024;
    *occupied_space = occupied_bits * BLOCK_SIZE / 1024;


}

#else

/* ---------------------------------------------------------------------------------------------------------------------------
                                                    BUDDY SYSTEM IMPLEMENTATION
--------------------------------------------------------------------------------------------------------------------------- */
/*La implementación está pensada con un BT. La raiz arranca con un valor del total de la memoria disponible
y se va dividiendo recursivamente de a mitades, el valor mínimo es 512B. Si mi memoria es de 16B(2^4), voy a tener
como máximo 2^4 -1 nodos y 4-1 niveles
Para alocar voy recorriendo siempre por la izquierda. Si no se puede colocar a la izquierda voy por el nodo
de la derecha, que tiene el offset apropiado para no ir pisando la memoria*/
#define TOTAL_NODES  32767//63
#define TREE_HEIGHT 14  //El tamaño de la mem es 8MB = 2^23 B . Nodo mínimo es 512B
#define TRUE 1
#define FALSE 0
#define LEFT 1
#define RIGHT 2


typedef  struct node
{
    struct node * left;
    struct node * right;
    struct node * ascendant;
    int size;
    int l_subtree_occuppied;
    int r_subtree_occuppied;
    char free;
 
}node;
node buddy_tree [TOTAL_NODES] = {{0}};
int nodes_counter =0;
int bytes_occupied=0;

node * create_tree_node_rec (int size,node * ascendant, int levels_remaining){
    //Creo todo el arbol, tengo TREE_HEIGHT  niveles que crear
    if(!levels_remaining)
        return NULL;
    //Como no hay memoria dinámica, reservamos un array estático para ir pidiendo memoria para los nodos
    node * node = buddy_tree + (nodes_counter++);
    node->free=TRUE;
    node->ascendant=ascendant;
    node->size=size;
    node->l_subtree_occuppied=FALSE;
    node->r_subtree_occuppied=FALSE;
    //creo todo el arbol recursivamente
    node->left= create_tree_node_rec(size/2,node,levels_remaining-1);
    node->right= create_tree_node_rec(size / 2,node, levels_remaining - 1);


    return node;




}

void *  recursive_malloc (int size, node * node, int offset){
    if(size>node->size || node->free==FALSE )
        return NULL;
    if((size <= (node->size) /2)) {
        //Si es hoja no puedo seguir recorriendo el arbol en busca de un nodo mas chico
        if (node->right==NULL && node->left==NULL) {
            node->free = FALSE;
            bytes_occupied += node->size;
            return FIRST_HEAP_ADRESS + offset;
        }
        //Voy buscando por nodos más chicos

        if (node->left->free == TRUE) {
            void *aux = recursive_malloc(size, node->left, offset);
            if (aux) {
                node->l_subtree_occuppied=TRUE;
                return aux;
            }

        }
        if (node->right->free) {
            void *aux = recursive_malloc(size, node->right, offset + (node->size) / 2);
            if (aux) {
                node->r_subtree_occuppied=TRUE;
                return aux;

            }

        }
        return NULL;

    }
    //ahora me tengo q fijar que no este ocupado ningun subarbol
    if(node->l_subtree_occuppied==FALSE && node->r_subtree_occuppied==FALSE){
        node->free=FALSE;
        bytes_occupied += node->size;
        return FIRST_HEAP_ADRESS + offset;
    }

    return NULL;



}

void * malloc (int size){
    if(!nodes_counter)
        create_tree_node_rec(MEM_SIZE,NULL,TREE_HEIGHT+1);
    return recursive_malloc(size,buddy_tree,0);

}



int free_rec (void * adress, node * node, int offset, int type){
    if(node==NULL || (node->free==FALSE && node->l_subtree_occuppied==FALSE && node->r_subtree_occuppied==TRUE))
        return FALSE;
    if(FIRST_HEAP_ADRESS+offset == adress && node->free==FALSE){
        node->free=TRUE;
        bytes_occupied -= node->size;
        if(type==LEFT)
            node->ascendant->l_subtree_occuppied=FALSE;
        else if(type==RIGHT)
            node->ascendant->r_subtree_occuppied=FALSE;
        return TRUE; //lo libero
    }

    /*if(!free_rec(adress,node->left,offset,LEFT))
        if(free_rec(adress,node->right,offset + (node->size / 2),RIGHT))
            node->ascendant->r_subtree_occuppied=FALSE;*/
    if(((FIRST_HEAP_ADRESS+offset+node->size) >= adress) && !free_rec(adress, node->left,offset,LEFT))
        free_rec(adress,node->right,offset+(node->size/2),RIGHT);
                    

    return FALSE;
}
void free (void * adress){
    free_rec(adress,buddy_tree,0,LEFT);
}

void mem_status(int * memory_size, int * free_space, int * occupied_space){
    
    *memory_size =  MEM_SIZE / 1024; //en KB
    *free_space = (MEM_SIZE - bytes_occupied) /1024; //en kb . 
    int aux = div_ceil(bytes_occupied,1024);
    if(aux <= 0)
        aux=1;
     
    *occupied_space = aux; // en KB


}





#endif 