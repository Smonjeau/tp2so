#include <scheduler.h>
#include <mem_manager.h>
#include <stddef.h>


/*
	Aclaración: para esta version del scheduler se utiliza unicamente el nivel de prioridad 100,
	se asigna un quantum de tiempo fijo y no se consideran procesos bloqueados.

	Se tienen 2 juegos de colas de procesos para "Activos" y "Expirados".
*/

procQueue actives[40];
procQueue expireds[40];


// Decide el quantum de tiempo que utilizará el proceso.
void assignQuantumTime(PCB pcb) {
	pcb->remainingTicks = 10;
}

// Decide el nivel de prioridad del proceso.
int getPriorityLevel(PCB pcb) {
	return 100;
}

// Añade proceso a una cola particular.
void queueProc(procQueue queue, PCB pcb) {
	if(queue.last == NULL) {
		//Empty queue
		queue.first = pcb;
		queue.last = pcb;
	} else {
		(queue.last)->nextPCB = pcb;
		queue.last = pcb;
	}
}



void * schedule(void *currContextRSP) {
	//PCB currentPCB;
	return currContextRSP;
}

/*
	if(currentPCB == NULL)
		return NULL; //No hay procesos aún

	if(currentPCB->procState == RUN) {
		currentPCB->remainingTicks--;
		if(currentPCB->remainingTicks > 0)
			return currentPCB->contextRSP;

		//Se terminó el tiempo asignado al proceso.
		assignQuantumTime(currentPCB);


	} else {
		//Por como se diseña el scheduler sabemos que si no esta en RUN
		//entonces esta en BLOCKED. Por lo tanto buscamos al siguiente proceso
		//con mayor prioridad y estado READY
		//Aun no programamos lo necesario para manejar procesos bloqueados.
		return NULL;
	}

	
	int priorityIdx;
	for(priorityIdx = 0; priorityIdx < 40; priorityIdx++) {
		currentPCB = actives[priorityIdx].first;
		if(currentPCB != NULL && currentPCB->contextRSP == currContextRSP) {
			if(currentPCB->procState)
			break;		
		}
	}


	//
	

	//Recorremos actives en busca del proceso current
	
	
	// Update the contextRSP of the running process and change it's procState to READY

    // Choose a new process and change it's procState to RUN

    // Return the contextRSP of the chosen process

    

}*/

int newProc(void * contextRSP) {
	PCB new = malloc(sizeof(struct PCB));
	if (new == NULL)
		return -1;
	new->contextRSP = contextRSP;
	new->procState = READY;
	assignQuantumTime(new);
	int priority = getPriorityLevel(new) - 100;
	queueProc(actives[priority], new);
	return 0;
}