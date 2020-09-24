#ifndef __MEM__MANAGER__H
#define __MEM__MANAGER__H
void * malloc(int size);
void free (void * ptr);
void mem_status(int * memory_size, int * free_space, int * occupied_space);
#endif