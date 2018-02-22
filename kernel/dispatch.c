
#include <kernel.h>

#include "disptable.c"


PROCESS         active_proc;


/* 
 * Ready queues for all eight priorities.
 */
PCB            *ready_queue[MAX_READY_QUEUES];

unsigned ready_procs;


/* 
 * add_ready_queue
 *----------------------------------------------------------------------------
 * The process pointed to by p is put the ready queue.
 * The appropiate ready queue is determined by p->priority.
 */

void add_ready_queue(PROCESS proc)
{
	int priority;
	assert(proc->magic == MAGIC_PCB);
	priority = proc->priority;

	/* If proc is the only process in the current priority level */
	if(ready_queue[priority] == NULL){
		ready_queue[priority] = proc;
		proc->next = proc;
		proc->prev = proc;
		ready_procs |= 1 << priority;
	}
	/* If there is some other process in this priority level */
	else{
		proc->next = ready_queue[priority];
		proc->prev = ready_queue[priority]->prev;
		ready_queue[priority]->prev->next = proc;
		ready_queue[priority]->prev = proc;
	}
	proc->state = STATE_READY;
}



/* 
 * remove_ready_queue
 *----------------------------------------------------------------------------
 * The process pointed to by p is dequeued from the ready
 * queue.
 */

void remove_ready_queue(PROCESS proc)
{
	int priority;
	assert(proc->magic == MAGIC_PCB);
	priority = proc->priority;
	/* If there are no other processes in the same priority level as proc */
	if(proc->next == proc){
		ready_queue[priority] = NULL;
		ready_procs &= ~(1 << priority);
	}
	/* If there are other processes in the same priority level as proc */
	else{
		ready_queue[priority] = proc->next;
		proc->next->prev = proc->prev;
		proc->prev->next = proc->next;
	}
}



/* 
 * become_zombie
 *----------------------------------------------------------------------------
 * Turns the calling process into a zombie. It will be removed from the ready
 * queue and marked as being in STATE_ZOMBIE.
 */

void become_zombie()
{
    active_proc->state = STATE_ZOMBIE;
    while (1);
}



/* 
 * dispatcher
 *----------------------------------------------------------------------------
 * Determines a new process to be dispatched. The process
 * with the highest priority is taken. Within one priority
 * level round robin is used.
 */

PROCESS dispatcher()
{
	PROCESS new_proc;
	int i = 0;

	/* Find queue with the highest priority that is not empty */
	i = table[ready_procs];
	assert(i != -1);
	/* Check if new process has the same priority as the existing process */
	if(i == active_proc->priority){
		new_proc = active_proc->next; 
	}
	/* Dispatch a process at a different priority level */
	else{
		new_proc = ready_queue[i];
	}
	return new_proc;
}



/* 
 * resign
 *----------------------------------------------------------------------------
 * The current process gives up the CPU voluntarily. The
 * next running process is determined via dispatcher().
 * The stack of the calling process is setup such that it
 * looks like an interrupt.
 */
void resign()
{
}



/* 
 * init_dispatcher
 *----------------------------------------------------------------------------
 * Initializes the necessary data structures.
 */

void init_dispatcher()
{
	int i = 0;
	for(i=0;i<MAX_READY_QUEUES;i++){
		ready_queue[i] = NULL;
	}

	ready_procs = 0;

	/* Setup first process */

	add_ready_queue(active_proc);


}
