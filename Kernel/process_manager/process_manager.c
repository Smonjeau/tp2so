#include <process_manager.h>
//#include <context_manager.h> No existe este archivo
#include <mem_manager.h>
#include <stddef.h>
#include <screen_driver.h>

/*
	Aclaración: para esta version del scheduler se utiliza unicamente el nivel de prioridad 100,
	se asigna un quantum de tiempo fijo y no se consideran procesos bloqueados.
	Se tienen 2 juegos de colas de procesos para "Activos" y "Expirados".
*/


ProcQueue queue1[40];
ProcQueue queue2[40];

//De esta forma será facil hacer swap
ProcQueue * actives = queue1;
ProcQueue * expireds = queue2;

// Decide el quantum de tiempo que utilizará el proceso.
void assignQuantumTime(PCB pcb) {
	pcb->remainingTicks = 10;
}

// Decide el nivel de prioridad del proceso.
int getPriorityLevel(PCB pcb) {
	return 100;
}

// Añade proceso a una cola particular.
void queueProc(ProcQueue queue, PCB pcb) {
	if(queue.last == NULL) {
		//Empty queue
		queue.first = pcb;
		queue.last = pcb;
	} else {
		(queue.last)->nextPCB = pcb;
		queue.last = pcb;
	}
	pcb->nextPCB = NULL;
}

void sendToExpired(PCB pcb) {
	assignQuantumTime(pcb); //Le reseteamos su quantum time
	int priority = getPriorityLevel(pcb) - 100; //Recalculamos su priority
	pcb->procState = READY;
	queueProc(expireds[priority], pcb);
}

void swapQueues() {
	ProcQueue * aux;
	aux = expireds;
	expireds = actives;
	actives = aux;
}


void * schedule(void *currContextRSP) {

	return currContextRSP;

	/*Gran parte de este scheduler deberá ser alterado en próxima
	entrega pues no se está considerando caso de proceso bloqueado.*/

	PCB currentPCB;
	int priorityIdx;
	for(priorityIdx = 0; priorityIdx < 40; priorityIdx++) {
		currentPCB = actives[priorityIdx].first;
		if(currentPCB != NULL && currentPCB->procState == RUN)
			break; //Lo encontré
		
	}

	if(priorityIdx == 40)
		return NULL; //Recibí un RSP de un proceso no añadido al scheduler, no tendría sentido por ahora.

	currentPCB->contextRSP = currContextRSP; //Por si cambió
	currentPCB->remainingTicks--;
	if(currentPCB->remainingTicks > 0)
		return currContextRSP; //Continua el mismo proceso de antes.


	//Se terminó el tiempo asignado del proceso.
	actives[priorityIdx].first = currentPCB->nextPCB;
	if(actives[priorityIdx].first == NULL)
		actives[priorityIdx].last = NULL;
	sendToExpired(currentPCB);


	// Debo buscar al proximo a ejecutar. Aprovecho lo que ya recorri de las colas de activos.
	int idx;
	for(idx = priorityIdx; idx < 40; idx++) {
		currentPCB = actives[idx].first;
		if(currentPCB != NULL && currentPCB->procState == READY)
			break; //Lo encontré
	}

	if(idx == 40) {
		//No quedan mas procesos activos. Intercambiamos por los expirados.
		swapQueues();

		//Nuevamente recorro en busca del primer proceso listo
		for(idx = priorityIdx; idx < 40; idx++) {
			currentPCB = actives[idx].first;
			if(currentPCB != NULL && currentPCB->procState == READY)
				break; //Lo encontré
		}
		currentPCB->procState = RUN;
		return currentPCB->contextRSP;

	} else {
		currentPCB->procState = RUN;
		return currentPCB->contextRSP;
	}

}


int newProcess(void *main, int argc, char **argv){

	static int pid=0;

    void *contextRSP = createContext(argc, argv, main);

	PCB new = malloc(sizeof(struct PCB));
	
	if (new == NULL)
		return 1;
	
	new->procState = READY;
    new->contextRSP = contextRSP;
	new->pid = pid++;

	assignQuantumTime(new);

	int priority = getPriorityLevel(new) - 100;
	queueProc(actives[priority], new);

	return pid;

}