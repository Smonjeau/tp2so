// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//-V::675

#include <process_manager.h>
#include <lib.h>
#include <mem_manager.h>
#include <stddef.h>
#include <screen_driver.h>
#include <interrupts.h>

int procCount = 0;


ProcQueue queue1[40] = {{0}};
ProcQueue queue2[40] = {{0}};

//De esta forma será facil hacer swap
ProcQueue * actives = queue1;
ProcQueue * expireds = queue2;

void * lastRSP = NULL;

PCB runningProc = NULL;
PCB foregroundProc = NULL;


void setForeground(PCB proc){
	if(proc != NULL) {
		if(proc->procState == BLOCKED)
			blockProcess(proc->pid, 1);

		foregroundProc = proc;
	}
}

int hasForeground(){

	return runningProc == foregroundProc;

}


// Decide el quantum de tiempo que utilizará el proceso.
void assignQuantumTime(PCB pcb) {
    if(pcb->pid == 0) {
    	//A la shell le asignamos de forma dinamica
    	//Cuantos mas procesos hay, mas quantum tiene
    	pcb->remainingTicks = 2 + procCount*2 ; //A la shell le damos mas quantum pues interactua con usuario
    } else if(pcb->pid == 1)
		pcb->remainingTicks = 1; //Al proceso dummy le damos el menor quantum posible
	else if(pcb->pid == 2) {
		//Al proceso que gestiona el stdout le asignamos de forma dinamica
    	//Cuantos mas procesos hay, mas quantum tiene
    	pcb->remainingTicks = 2 + procCount; //Al interactuar con el usuario es muy importante
	}
    else
        pcb->remainingTicks = 2;
}

// Decide el nivel de prioridad del proceso.
int getPriorityLevel(PCB pcb) {
	return 100;
}

// Añade proceso a una cola particular.
void queueProc(ProcQueue * queue, PCB pcb) {
	pcb->nextPCB = NULL;
	if(queue->last == NULL) {
		//Empty queue
		queue->first = pcb;
		queue->last = pcb;
	} else {
		(queue->last)->nextPCB = pcb;
		queue->last = pcb;
	}
	
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

int is_fd_free(int fd) {
	if(runningProc != NULL && runningProc->pipes[fd] == NULL)
		return 1;
	return 0;
}

void copyFileDescriptorsFromParent(PCB pcb) {
	//Heredamos los fds del ascendente
	if(runningProc != NULL) {
		for(int i = 0; i < MAX_PIPES; i++) {
			pcb->pipes[i] = runningProc->pipes[i];
			pcb->tipoBocas[i] = runningProc->tipoBocas[i];

			

			

			if(pcb->pipes[i] != NULL) {
				if(pcb->tipoBocas[i] == LECTURA)
					pcb->pipes[i]->open_read_ports++;
				else
					pcb->pipes[i]->open_write_ports++;
				
				(pcb->pipes[i])->open_ports++;
			}
		}
	}
}

pipe findPipe(int fd) {
    pipe found = NULL;
    if(runningProc != NULL && fd >= 0 && fd < MAX_PIPES && runningProc->pipes[fd] != NULL)
        found = runningProc->pipes[fd];
    return found;
}

void getProcName(int pid, char * buffer) {
	ProcQueue * queue;
	PCB pcb = NULL;

	//Recorro activos
	for(int priority = 0; priority < 40; priority++) {
		queue = actives + priority;
		pcb = queue->first;
		while(pcb != NULL) {
			if(pcb->pid == pid)		
				break;
			pcb = pcb->nextPCB;
		}
		if(pcb->pid == pid)		
			break;
	}
	if(pcb->pid != pid) {
		//Recorro expirados
		for(int priority = 0; priority < 40; priority++) {
			queue = expireds + priority;
			pcb = queue->first;
			while(pcb != NULL) {
				if(pcb->pid == pid)		
					break;
				pcb = pcb->nextPCB;
			}
			if(pcb->pid == pid)		
				break;
		}
	}

	if(pcb->pid == pid) {
		//Lo encontro
		int i = 0;
		while(pcb->name[i] != 0)
			buffer[i] = pcb->name[i];
		buffer[i] = 0;
	}

	
}

int assign_pipe_to_pcb(int  fds [2], pipe pipe_to_assign) {
	int i, j;
	pipe * aux = runningProc->pipes;
	int * tipoBocas = runningProc->tipoBocas;
	for(i = 0, j = 0; i < 2 && j < MAX_PIPES; j++) {
		if(aux[j] == NULL) {
			fds[i] = j;
			aux[j] = pipe_to_assign;
			tipoBocas[j] = i;
			i++;
		}
	}
	return i == 2;
}
int assign_pipe_to_pcb_forced(int fd, pipe new) {
	if(runningProc == NULL || fd < 0 || fd >= MAX_PIPES)
		return -1;
	runningProc->pipes[fd] = new;
	return 0;
}

void close_fd_proc(PCB pcb, int fd) {
    if(pcb != NULL && fd >= 0 && fd < MAX_PIPES) {

    	pipe aux = pcb->pipes[fd];
    	if(aux != NULL) {
	        
	        if(pcb->tipoBocas[fd] == ESCRITURA) {
	        	if(aux->open_write_ports == 1) { //Esta cerrando la ultima boca de escritura
					char c = EOT;
					for(int i=0; i<aux->open_read_ports; i++)
						pipe_write_nofd(aux, &c, 1);			
				}
	        }
	        aux->open_ports--;
			if(pcb->tipoBocas[fd] == LECTURA)
	        	aux->open_read_ports--;
	        else
	        	aux->open_write_ports--;

			pcb->pipes[fd] = NULL;
			
	        free_pipe_if_empty(aux);
    	}
        
    }
}

void close_fd(int fd) {
	close_fd_proc(runningProc, fd);
}

int dup_fd(int fd) {
	if(runningProc == NULL || fd < 0 || fd >= MAX_PIPES)
		return -1;

	int i = 0;
	while(runningProc->pipes[i++] != NULL);
	i--; //Compenso

	if(i > fd)
		return -1; //Fue a parar a un fd incluso mas grande

	pipe aux = runningProc->pipes[fd];
	runningProc->pipes[fd] = NULL;
	runningProc->pipes[i] = aux;
    runningProc->tipoBocas[i] = runningProc->tipoBocas[fd];
	return 0;


}


void copyPSInfoToBuffer(char * buffer, PCB pcb, int priority) {
	strcat("Pid: ", buffer);
	itoa(pcb->pid, buffer + strlen(buffer), 10, -1);

	strcat("\nName: ", buffer);
	strcat(pcb->name, buffer);
	
	strcat("\nState: ", buffer);
	switch (pcb->procState){
        case READY:
            strcat("READY", buffer);
            break;

        case RUN:
            strcat("RUNNING", buffer);
            break;

		case RECENTLY_BLOCKED:
        case BLOCKED:
            strcat("BLOCKED", buffer);
            break;

        case DEAD:
            strcat("DEAD", buffer);
            break;
	}

	strcat(" - Priority: ", buffer);
	itoa(priority, buffer + strlen(buffer), 10, -1);

	if(foregroundProc == pcb)
		strcat("- FG", buffer);
	else
		strcat("- BG", buffer);
	
	strcat("\nContext RSP: ", buffer);
	strcat("0x", buffer);
	itoa((uint64_t)pcb->contextRSP, buffer + strlen(buffer), 16, -1);

	strcat(" - Base RSP: ", buffer);
	strcat("0x", buffer);
	itoa((uint_fast64_t)pcb->segmentAddress, buffer + strlen(buffer), 16, -1);
	strcat("\n\n", buffer);
}

void ps(char * buffer) {

	if(buffer == NULL)
		return;

	ProcQueue * queue;
	PCB pcb;
	//Recorro activos

	strcpy("", buffer);

	for(int priority = 0; priority < 40; priority++) {
		queue = actives + priority;
		pcb = queue->first;
		while(pcb != NULL) {
			copyPSInfoToBuffer(buffer, pcb, priority+100);		
			pcb = pcb->nextPCB;
		}
	}

	//Recorro expirados
	for(int priority = 0; priority < 40; priority++) {
		queue = expireds + priority;
		pcb = queue->first;
		while(pcb != NULL) {
			copyPSInfoToBuffer(buffer, pcb, priority+100);
			pcb = pcb->nextPCB;
		}
	}

}

PCB findProc (int pid){
	ProcQueue * queue;
	PCB pcb;
	int found=0;
	for(int prio=0;prio<40 && ! found;prio++){
		queue = actives + prio;
		pcb = queue->first;
		if(pcb->pid==pid)
			found=1;
		while(pcb!=NULL)
			pcb=pcb->nextPCB;
	}
	for(int prio=0;prio<40 && !found;prio++){
		queue = expireds + prio;
		pcb = queue->first;
		if(pcb->pid==pid)
			found=1;
		while(pcb!=NULL)
			pcb=pcb->nextPCB;

	}
	if(!found)
		pcb=NULL;
	return pcb;
}

void swapIfNeeded() {
	int idx, found = 0;
	for(idx = 0; idx < 40 && found == 0; idx++)
		if(actives[idx].first != NULL)
			found = 1;

	if(found == 0)
		swapQueues();

	
}

void niceProcess(int pid, int priority) {
	if(priority < 100 || priority > 139)
		return;


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
			if(inActives == 1)
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

}



void blockProcess(int pid, int tick) {

	int found = 0, idx;
	PCB currentPCB = NULL;

	//Busco en actives
	for(idx = 0; idx < 40 && found == 0; idx++) {
		currentPCB = actives[idx].first;

		while(currentPCB != NULL && currentPCB->pid != pid)
			currentPCB = currentPCB->nextPCB; 

		if(currentPCB != NULL && currentPCB->pid == pid)
			found = 1;	
		
	}
	if(found == 0) {
		//Busco en expireds
		for(idx = 0; idx < 40 && found == 0; idx++) {
			currentPCB = expireds[idx].first;

			while(currentPCB != NULL && currentPCB->pid != pid)
				currentPCB = currentPCB->nextPCB; 

			if(currentPCB != NULL && currentPCB->pid == pid)
				found = 1;	
			
		}
	}

	if(found == 1) {

		idx--;
		if(currentPCB->procState == READY) {
			currentPCB->procState = BLOCKED;			
		} else if(currentPCB->procState == BLOCKED) {
			currentPCB->procState = READY;
		} else if(currentPCB->procState == RUN) {

			//Tengo que actualizar mi RSP


		    currentPCB->procState = RECENTLY_BLOCKED;
			// Renuncia al CPU
			//Solo en este caso bloqueamos y enviamos a expirados en vez de que lo haga el scheduler
			if(tick == 1)
			    _timer_tick();
		}
		
	}
	
}


void * schedule(void * currContextRSP) {



	PCB currentPCB = NULL;
	int priorityIdx;

	/* Para que sirve esta variable binaria? Resulta que cuando un proceso run se autobloquea, queda
	 * en su misma posicion pero estado bloqueado. Por lo tanto si se encunetra un proceso bloqueado
	 * en la primer posicion hallada entonces se trata de un proceso que se acaba de autobloquear*/
	//int primerProcEncontrado = 1;
	do {
		priorityIdx = 0;

		do {
			currentPCB = actives[priorityIdx].first;

			//Debemos considerar que sucede si el currentPCB está bloqueado
			while(currentPCB != NULL && (currentPCB->procState == BLOCKED || currentPCB->procState== RECENTLY_BLOCKED)) {
				//Al proceso que le correspondía ejecutarse sigue bloqueado. Lo mando a expirados para la proxima tanda.
				//Primero lo sacamos de esta cola
                if(currentPCB->procState == RECENTLY_BLOCKED){
                    //Le actualizamos el contexto
                    currentPCB->contextRSP=currContextRSP;

                }
				actives[priorityIdx].first = currentPCB->nextPCB; //Pues está al comienzo de la cola
				if(actives[priorityIdx].first == NULL)
					actives[priorityIdx].last = NULL; //Era el unico proceso
				

				sendToExpired(currentPCB, BLOCKED, priorityIdx);


				currentPCB = actives[priorityIdx].first; //Paso al siguiente en la cola, en busca de un proceso READY

			}
			
			priorityIdx++;

		} while(priorityIdx < 40 && currentPCB == NULL);
		if(priorityIdx == 40 && currentPCB == NULL) {
			swapQueues(); //Es necesario cuando un proceso se autobloquea y no quedan activos en estado READY
		}
	} while(priorityIdx == 40 && currentPCB == NULL);




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

	idx = priorityIdx;
	do {
		currentPCB = actives[idx].first;

		//Debemos considerar que sucede si el currentPCB está bloqueado
		while(currentPCB != NULL && currentPCB->procState == BLOCKED) {
			//Al proceso que le correspondía ejecutarse sigue bloqueado. Lo mando a expirados para la proxima tanda.
			//Primero lo sacamos de esta cola
			actives[idx].first = currentPCB->nextPCB; //Pues está al comienzo de la cola
			if(actives[idx].first == NULL)
				actives[idx].last = NULL; //Era el unico proceso
			

			sendToExpired(currentPCB, BLOCKED, idx);


			currentPCB = actives[idx].first; //Paso al siguiente en la cola, en busca de un proceso READY

		}
		
		idx++;

	} while(idx < 40 && currentPCB == NULL);


	

	if(idx == 40 && currentPCB == NULL) {
		//No quedan mas procesos activos. Intercambiamos por los expirados.
		swapQueues();

		//Nuevamente recorro en busca del primer proceso listo
		currentPCB = NULL;
		idx = 0;
		do {
			currentPCB = actives[idx].first;

			//Debemos considerar que sucede si el currentPCB está bloqueado
			while(currentPCB != NULL && currentPCB->procState == BLOCKED) {
				//Al proceso que le correspondía ejecutarse sigue bloqueado. Lo mando a expirados para la proxima tanda.
				//Primero lo sacamos de esta cola
				actives[idx].first = currentPCB->nextPCB; //Pues está al comienzo de la cola
				if(actives[idx].first == NULL)
					actives[idx].last = NULL; //Era el unico proceso
				

				sendToExpired(currentPCB, BLOCKED, idx);


				currentPCB = actives[idx].first; //Paso al siguiente en la cola, en busca de un proceso READY

			}
			
			idx++;

		} while(idx < 40 && currentPCB == NULL);


		//Y si no encuentra READY? No deberia pasar pues siempre creamos al menos 1 proceso master
		
		currentPCB->procState = RUN;
		runningProc=currentPCB;
		return currentPCB->contextRSP;

	} else {
		currentPCB->procState = RUN;
		runningProc = currentPCB;
		return currentPCB->contextRSP;
	}


}

static int pid=0;

int createProcessPCB(void * contextRSP, void * segmentAddress, char * name, int foreground) {

	if(lastRSP == NULL)
		lastRSP = contextRSP;
	else if(contextRSP < lastRSP)
		lastRSP = contextRSP;

	PCB new = malloc(sizeof(struct PCB));
	
	if (new == NULL)
		return -1;
	
    new->contextRSP = contextRSP;
	new->segmentAddress=segmentAddress;
	new->parent = runningProc;
	new->pid = pid++;
	new->name=name;
	new->nextPCB = NULL;
	for(int i = 0; i < MAX_PIPES; i++)
		new->pipes[i] = NULL;

	if(foreground)
		setForeground(new);

	assignQuantumTime(new);

	copyFileDescriptorsFromParent(new);

	int priority = getPriorityLevel(new) - 100;

	new->procState = READY;

	queueProc(actives + priority, new);
	procCount++;
    
	return new->pid;

}


int getPID(){

	return runningProc!=NULL ? runningProc->pid : -1;

}


void killProcess(int pid) {

	PCB currentPCB;
	int priorityIdx;
	if(pid == -1) { //	PID=-1 se referirá al proceso ejecutándose actualmente (para usarlo como exit)

        for(priorityIdx = 0; priorityIdx < 40; priorityIdx++) {
            currentPCB = actives[priorityIdx].first;
            if(currentPCB->pid == runningProc->pid)
                break;
        }

		actives[priorityIdx].first = runningProc->nextPCB;

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
			if(found == 1) {
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
			} else {
				return;
			}
			


		}
		

		swapIfNeeded();

				
	}


	// Close file descriptors

	ProcState state = currentPCB->procState;
	for(int pipe=0; pipe<MAX_PIPES; pipe++){
	    if(currentPCB->pipes[pipe] != NULL)
	        close_fd_proc(currentPCB, pipe);
	}


	// Set children's parent to own parent

	for(priorityIdx = 0; priorityIdx < 40; priorityIdx++) {
		PCB pcb = actives[priorityIdx].first;
		while(pcb != NULL){
			if(pcb->parent == currentPCB)
				pcb->parent = currentPCB->parent;

			pcb = pcb->nextPCB;
		}
	}

	for(priorityIdx = 0; priorityIdx < 40; priorityIdx++) {
		PCB pcb = expireds[priorityIdx].first;
		while(pcb != NULL){
			if(pcb->parent == currentPCB)
				pcb->parent = currentPCB->parent;

			pcb = pcb->nextPCB;
		}
	}

	// Return foreground to parent

	if(hasForeground())
		setForeground(currentPCB->parent);


	// Free the memory

    free(currentPCB->segmentAddress);
    free(currentPCB);
    procCount--;

	if(state == RUN) //El proceso se suicida
        _timer_tick();
	
}