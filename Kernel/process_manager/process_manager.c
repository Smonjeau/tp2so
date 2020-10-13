#include <process_manager.h>
#include <lib.h>
#include <mem_manager.h>
#include <stddef.h>
#include <screen_driver.h>
#include <interrupts.h>
#include <screen_driver.h>

/*
	Aclaración: para esta version del scheduler se utiliza unicamente el nivel de prioridad 100 y
	se asigna un quantum de tiempo fijo..
	Se tienen 2 juegos de colas de procesos para "Activos" y "Expirados".
*/



ProcQueue queue1[40] = {{0}};
ProcQueue queue2[40] = {{0}};

//De esta forma será facil hacer swap
ProcQueue * actives = queue1;
ProcQueue * expireds = queue2;

void * lastRSP = NULL;

PCB runningProc=NULL;
// Decide el quantum de tiempo que utilizará el proceso.
void assignQuantumTime(PCB pcb) {
	pcb->remainingTicks = 2;
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

//Quita proceso de una cola particular
void removeFromQueue(ProcQueue * queue, int pid) {
	//Debe ser llamada desde otra funcion que sea atómica SI O SI
	PCB prev = NULL;
	PCB curr = queue->first;
	while(curr != NULL && curr->pid != pid) {
		prev = curr;
		curr = curr->nextPCB;
	}
	if(curr != NULL) {
		if(prev == NULL) {
			queue->first = curr->nextPCB;
			if(queue->first == NULL)
				queue->last = NULL;
		} else {
			prev->nextPCB = curr->nextPCB;
			if(prev->nextPCB == NULL)
				queue->last = prev;
		}
	}
}

void sendToExpired(PCB pcb, ProcState newState, int priority) { 
	//Recibe priority como parametro por si dinamicamente se decide cambiarle la prioridad
	assignQuantumTime(pcb); //Le reseteamos su quantum time
	//Mantenemos el priority que tenia previamente //getPriorityLevel(pcb) - 100; //Recalculamos su priority
	pcb->procState = newState;
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
	int idx, found = 0;
	for(idx = 0; idx < 40; idx++)
		if(actives[idx].first != NULL)
			found = 1;

	if(found == 0)
		swapQueues();

	
}

void niceProcess(int pid, int priority) {
	if(priority < 100 || priority > 139)
		return;

	/* Debe ser atómica */
	_cli();
	int found = 0, inActives = 1, idxPriority;
	PCB currentPCB = NULL;

	//Busco en actives
	for(idxPriority = 0; idxPriority < 40 && found == 0; idxPriority++) {
		currentPCB = actives[idxPriority].first;

		while(currentPCB != NULL && currentPCB->pid != pid)
			currentPCB = currentPCB->nextPCB; 

		if(currentPCB != NULL && currentPCB->pid == pid)
			found = 1;	
		
	}
	if(found == 0) {
		inActives = 0;
		//Busco en expireds
		for(idxPriority = 0; idxPriority < 40 && found == 0; idxPriority++) {
			currentPCB = expireds[idxPriority].first;

			while(currentPCB != NULL && currentPCB->pid != pid)
				currentPCB = currentPCB->nextPCB; 

			if(currentPCB != NULL && currentPCB->pid == pid)
				found = 1;	
			
		}
	}

	if(found == 1) {
		idxPriority--; //Compenso el ultimo ++
		if(idxPriority != priority) {
			//Primero lo quitamos de esta cola
			if(inActives)
				removeFromQueue(actives + idxPriority, pid);
			else
				removeFromQueue(expireds + idxPriority, pid);

			priority -= 100;

			//Ahora lo añadimos a la nueva cola
			if(inActives)
				queueProc(actives + priority, currentPCB);
			else
				queueProc(expireds + priority, currentPCB);
		}
		
	}



	_sti();


}



PCB getPCB(int pid) {
	if(pid == 0)
		return;

	/* Debe ser atómica */
	_cli();
	int found = 0;
	PCB currentPCB = NULL;

	for(int idx=0; idx < 40 && found == 0; idx++) {
		currentPCB = actives[idx].first;

		while(currentPCB != NULL && currentPCB->pid != pid)
			currentPCB = currentPCB->nextPCB; 

		if(currentPCB != NULL && currentPCB->pid == pid)
			found = 1;		
	}

	if(found)
		return currentPCB;

	for(int idx = 0; idx < 40 && found == 0; idx++) {
		currentPCB = expireds[idx].first;

		while(currentPCB != NULL && currentPCB->pid != pid)
			currentPCB = currentPCB->nextPCB; 

		if(currentPCB != NULL && currentPCB->pid == pid)
			found = 1;		
	}

	return currentPCB;

	_sti();
}


int blockProcess(int pid){

	PCB pcb = getPCB(pid);
	if(pcb == NULL)
		return -1;

	pcb->procState = BLOCKED;

}


int unblockProcess(int pid){

	PCB pcb = getPCB(pid);
	if(pcb == NULL || pcb->procState != BLOCKED)
		return -1;

	pcb->procState = READY;

}


void * schedule(void *currContextRSP) {



	PCB currentPCB = NULL;
	int priorityIdx = 0;
	do {
		currentPCB = actives[priorityIdx].first;

		//Debemos considerar que sucede si el currentPCB está bloqueado
		while(currentPCB != NULL && currentPCB->procState == BLOCKED) {
			//Al proceso que le correspondía ejecutarse sigue bloqueado. Lo mando a expirados para la proxima tanda.
			//Primero lo sacamos de esta cola
			actives[priorityIdx].first = currentPCB->nextPCB; //Pues está al comienzo de la cola
			if(actives[priorityIdx].first == NULL)
				actives[priorityIdx].last = NULL; //Era el unico proceso
			

			sendToExpired(currentPCB, BLOCKED, priorityIdx);


			currentPCB = currentPCB->nextPCB; //Paso al siguiente en la cola, en busca de un proceso READY

		}
		
		priorityIdx++;

	} while(priorityIdx < 40 && currentPCB == NULL);


	if(priorityIdx == 40)
		return currContextRSP; //Recibí un RSP de un proceso no añadido al scheduler, no tendría sentido por ahora.

	priorityIdx--; //Compenso el ultimo ++

	

	if(currentPCB->procState == RUN) {
		currentPCB->remainingTicks--;
		if(currContextRSP < lastRSP)
			lastRSP = currContextRSP;
		currentPCB->contextRSP = currContextRSP; //Por si cambió
	} else if(currentPCB->procState == READY) { //Primer proceso
		currentPCB->procState = RUN;
	}



	
	if(currentPCB->remainingTicks > 0){
		runningProc = currentPCB;
		return currentPCB->contextRSP; //Continua

	}


	//Se terminó el tiempo asignado del proceso.
	actives[priorityIdx].first = currentPCB->nextPCB;
	if(actives[priorityIdx].first == NULL)
		actives[priorityIdx].last = NULL;
	sendToExpired(currentPCB, READY, priorityIdx);


	// Debo buscar al proximo a ejecutar. Aprovecho lo que ya recorri de las colas de activos.
	int idx;
	currentPCB = NULL;
	for(idx = priorityIdx; idx < 40 && currentPCB == NULL && currentPCB->procState != READY; idx++)
		currentPCB = actives[idx].first;


	

	if(idx == 40) {
		//No quedan mas procesos activos. Intercambiamos por los expirados.
		swapQueues();

		//Nuevamente recorro en busca del primer proceso listo
		currentPCB = NULL;
		for(idx = 0; idx < 40 && currentPCB == NULL && currentPCB->procState != READY; idx++)
			currentPCB = actives[idx].first;

		//Y si no encuentra READY? No deberia pasar pues siempre creamos al menos 1 proceso master
		
		currentPCB->procState = RUN;
		runningProc=currentPCB;
		return currentPCB->contextRSP;

	} else {
		idx--; //Compenso
		currentPCB->procState = RUN;
		runningProc=currentPCB;
		return currentPCB->contextRSP;
	}


}

static int pid=0;

int createProcessPCB(void *contextRSP, void * baseRSP, char * name){
	_cli();	

	if(lastRSP == NULL)
		lastRSP = contextRSP;
	else if(contextRSP < lastRSP)
		lastRSP = contextRSP;

	PCB new = malloc(sizeof(struct PCB));
	
	if (new == NULL)
		return -1;
	
    new->contextRSP = contextRSP;
	new->baseRSP=baseRSP;
	new->pid = pid++;
	new->name=name;
	new->nextPCB = NULL;

	assignQuantumTime(new);

	int priority = getPriorityLevel(new) - 100;

	
	new->procState = READY;

	queueProc(actives + priority, new);
	_sti();	

	return new->pid;

}

int createProcessPCBFromKernel(void *contextRSP){
	
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

	return new->pid;

}


int getPID(){

	return runningProc!=NULL ? runningProc->pid : -1;

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
		PCB prevPCB = NULL; //Lo necesitamos para el proceso de borrado en una cola
		for(priorityIdx = 0; priorityIdx < 40 && found == 0; priorityIdx++) {
			currentPCB = actives[priorityIdx].first;
			
			while(currentPCB != NULL && found == 0) {
				if(currentPCB->pid == pid) {
					found = 1;
				} else {
					prevPCB = currentPCB;
					currentPCB = currentPCB->nextPCB;
				}
			}			
		}
		if(found == 1) {

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
			for(priorityIdx = 0; priorityIdx < 40 && found == 0; priorityIdx++) {
				currentPCB = expireds[priorityIdx].first;
				while(currentPCB != NULL && found == 0) {
					if(currentPCB->pid == pid) {
						found = 1;
					} else {
						prevPCB = currentPCB;
						currentPCB = currentPCB->nextPCB;
					}
				}			
			}
			
			priorityIdx--; //Para compensar el ultimo ++ del for
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

	ProcState state = currentPCB->procState;
	free(currentPCB->baseRSP);
	free(currentPCB);
	_sti();	
	if(state == RUN) //El proceso se suicida
		_hlt(); //Espero a que llegue el tick para cambiar de proceso

	

	
}