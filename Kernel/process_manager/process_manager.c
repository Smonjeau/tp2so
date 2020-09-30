#include <process_manager.h>
#include <lib.h>
#include <mem_manager.h>
#include <stddef.h>
#include <screen_driver.h>
#include <interrupts.h>

/*
	Aclaración: para esta version del scheduler se utiliza unicamente el nivel de prioridad 100,
	se asigna un quantum de tiempo fijo y no se consideran procesos bloqueados.
	Se tienen 2 juegos de colas de procesos para "Activos" y "Expirados".
*/



ProcQueue queue1[40] = {{0}};
ProcQueue queue2[40] = {{0}};

//De esta forma será facil hacer swap
ProcQueue * actives = queue1;
ProcQueue * expireds = queue2;

void * lastRSP = NULL;

// Decide el quantum de tiempo que utilizará el proceso.
void assignQuantumTime(PCB pcb) {
	pcb->remainingTicks = 10;
}

// Decide el nivel de prioridad del proceso.
int getPriorityLevel(PCB pcb) {
	return 100;
}

// Añade proceso a una cola particular.
void queueProc(ProcQueue * queue, PCB pcb) {
	if(queue->last == NULL) {
		//Empty queue
		queue->first = pcb;
		queue->last = pcb;
	} else {
		(queue->last)->nextPCB = pcb;
		queue->last = pcb;
	}
	pcb->nextPCB = NULL;
}

void sendToExpired(PCB pcb) {
	assignQuantumTime(pcb); //Le reseteamos su quantum time
	int priority = getPriorityLevel(pcb) - 100; //Recalculamos su priority
	pcb->procState = READY;
	//queueProc(expireds[priority], pcb);
	queueProc(expireds + priority, pcb);
}

void swapQueues() {
	ProcQueue * aux;
	aux = expireds;
	expireds = actives;
	actives = aux;
}

void ps(void * buffer, int * procCant) {
	int structSize = sizeof(struct PCB);
	int count = 0;
	ProcQueue * queue;
	PCB pcb;
	//Recorro activos

	for(int priority = 0; priority < 40; priority++) {
		queue = actives + priority;
		pcb = queue->first;
		pcb->pid = 12;
		while(pcb != NULL) {
			memcpy(buffer + structSize * count++, pcb, structSize);
			pcb = pcb->nextPCB;
		}
	}

	//Recorro expirados
	for(int priority = 0; priority < 40; priority++) {
		queue = expireds + priority;
		pcb = queue->first;
		while(pcb != NULL) {
			memcpy(buffer + structSize * count++, pcb, structSize);
			pcb = pcb->nextPCB;
		}
	}


	*procCant = count;
	
}

void swapIfNeeded() {
	int idx;
	PCB auxPCB = NULL;
	for(idx = 0; idx < 40 && auxPCB == NULL; idx++)
		auxPCB = actives[idx].first;

	if(auxPCB == NULL)
		swapQueues();

	
}


void * schedule(void *currContextRSP) {

	/*Gran parte de este scheduler deberá ser alterado en próxima
	entrega pues no se está considerando caso de proceso bloqueado.*/

	PCB currentPCB;
	int priorityIdx;
	for(priorityIdx = 0; priorityIdx < 40; priorityIdx++) {
		currentPCB = actives[priorityIdx].first;
		if(currentPCB != NULL)// && currentPCB->procState == RUN)
			break; //Lo encontré
		
	}

	if(priorityIdx == 40)
		return currContextRSP; //Recibí un RSP de un proceso no añadido al scheduler, no tendría sentido por ahora.


	if(currentPCB->procState == RUN) {
		currentPCB->remainingTicks--;
		if(currContextRSP < lastRSP)
			lastRSP = currContextRSP;
		currentPCB->contextRSP = currContextRSP; //Por si cambió
	} else if(currentPCB->procState == READY) { //Primer proceso
		currentPCB->procState = RUN;
	}



	
	if(currentPCB->remainingTicks > 0)
		return currentPCB->contextRSP; //Continua el mismo proceso de antes.


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


int createProcessPCB(void *contextRSP){
	_cli();	
	static int pid=0;

	if(lastRSP == NULL)
		lastRSP = contextRSP;
	else if(contextRSP < lastRSP)
		lastRSP = contextRSP;

	PCB new = malloc(sizeof(struct PCB));
	
	if (new == NULL)
		return -1;
	
    new->contextRSP = contextRSP;
	new->pid = pid++;
	new->nextPCB = NULL;

	assignQuantumTime(new);

	int priority = getPriorityLevel(new) - 100;

	
	new->procState = READY;

	queueProc(actives + priority, new);
	_sti();	

	return pid;

}



void killProcess(int pid) {
	_cli(); //La hacemos no interrumpible

	PCB currentPCB;
	int priorityIdx;
	if(pid == -1) { //	PID=-1 se referirá al proceso ejecutándose actualmente (para usarlo como exit)
		//Buscamos proceso en estado RUN
		for(priorityIdx = 0; priorityIdx < 40; priorityIdx++) {
			currentPCB = actives[priorityIdx].first;
			if(currentPCB != NULL)// && currentPCB->procState == RUN)
				break; //Lo encontré. Se que está en estado RUN porque aun no consideramos estado bloqueado.
		}

		


		actives[priorityIdx].first = currentPCB->nextPCB;


		if(actives[priorityIdx].first == NULL) {

			actives[priorityIdx].last = NULL;
			swapIfNeeded();

			
			
		}


	} else {
		//Buscamos proceso con el mismo pid
		//Vamos por los activos
		int found = 0;
		PCB prevPCB = NULL; //Lo encesitamos para el proceso de borrado en una cola
		for(priorityIdx = 0; priorityIdx < 40 && !found; priorityIdx++) {
			currentPCB = actives[priorityIdx].first;
			
			while(currentPCB != NULL && !found) {
				if(currentPCB->pid == pid) {
					found = 1;
				} else {
					prevPCB = currentPCB;
					currentPCB = currentPCB->nextPCB;
				}
			}			
		}
		if(found) {

			priorityIdx--; //Para comensar el ultimo ++ del for
			if((actives[priorityIdx].first)->pid == pid) {
				//Es el primero
				actives[priorityIdx].first = currentPCB->nextPCB;
				if(actives[priorityIdx].first == NULL)
					actives[priorityIdx].last = NULL;
			} else {
				//No es el primero
				if((actives[priorityIdx].last)->pid == pid)
					actives[priorityIdx].last = prevPCB; //Es el ultimo	
				prevPCB->nextPCB = currentPCB->nextPCB;
			}

		} else {
			//Vamos por los expirados
			for(priorityIdx = 0; priorityIdx < 40 && !found; priorityIdx++) {
				currentPCB = expireds[priorityIdx].first;
				while(currentPCB != NULL && !found) {
					if(currentPCB->pid == pid) {
						found = 1;
					} else {
						prevPCB = currentPCB;
						currentPCB = currentPCB->nextPCB;
					}
				}			
			}
			
			priorityIdx--; //Para comensar el ultimo ++ del for
			if((expireds[priorityIdx].first)->pid == pid) {
				//Es el primero
				expireds[priorityIdx].first = currentPCB->nextPCB;
				if(expireds[priorityIdx].first == NULL)
					expireds[priorityIdx].last = NULL;
			} else {
				//No es el primero
				if((expireds[priorityIdx].last)->pid == pid)
					expireds[priorityIdx].last = prevPCB; //Es el ultimo	
				prevPCB->nextPCB = currentPCB->nextPCB;
			}

		}

		swapIfNeeded();

				
	}

	

	//free(currentPCB);
	_sti();	
	if(currentPCB->procState==RUN) //El proceso se suicida
		switchProcessContextBuenarda();

	
}

void * getLastContext() {
	return lastRSP;
}