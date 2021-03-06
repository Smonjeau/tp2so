// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//-V::576

#include <syscalls.h>
#include <std_io.h>
#include <std_lib.h>
#include <shell_cmds.h>


void printMemDump(int argc, char **argv){

	char *sourceStr = argv[1];

	uint64_t sourceHex = 0;
	for (int i = 2; sourceStr[i]; i++){
		char c = '0';
		if (sourceStr[i] >= 'a' && sourceStr[i] <= 'z')
			c = 'a';
		else
			c = 'A';
		sourceHex = sourceHex * 16 + sourceStr[i] - c;
	}

	char *src = (char *) sourceHex;
	char *dst = src + 32;

	memDump((void *)src, (void *)dst);

	putChar('\n');
	for (int i = 0; i < 33; i += 8){
		printf("%2x: %2x %2x %2x %2x %2x %2x %2x %2x\n", 0, 9, src + i, src[i], src[i + 1], src[i + 2], src[i + 3],
			   src[i + 4], src[i + 5], src[i + 6], src[i + 7]);
	}

	kill(-1);
	
}


void printMemStatus(int argc, char **argv){

	int total_mem=0;
    int  avail_mem=0;
    int occ_mem=0;

    memStatus(&total_mem,&avail_mem,&occ_mem);
    
    printf("Total memory: %d KB\n",1,total_mem);
    printf("Free memory: %d KB\n",1,avail_mem);
    printf("Occupied memory: %d KB\n",1,occ_mem);

	kill(-1);
    
}