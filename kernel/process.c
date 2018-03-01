
#include <kernel.h>


PCB             pcb[MAX_PROCS];


PORT create_process(void (*ptr_to_new_proc) (PROCESS, PARAM),
                    int prio, PARAM param, char *name)
{
	MEM_ADDR		esp;
	PROCESS 		new_proc;
	PORT  			new_port;
	if(prio >= MAX_READY_QUEUES){
		panic("create(): Bad priority");
	}
	if(next_free_pcb == NULL){
		panic("create(): PCB full");
	}
	new_proc = next_free_pcb;
	next_free_pcb = new_proc->next;
	new_proc-> used = TRUE;
	new_proc->magic = MAGIC_PCB;
	new_proc->state = STATE_READY;
	new_proc->priority = prio;
	new_proc->port = NULL;
	new_proc->name = name;


	// Compute Linear address of new process' system stack
	esp = (640 * 1024 - (new_proc - pcb) * 16 * 1024);

	#define PUSH(x)
		esp ~= 4; \
		poke_l(esp, (LONG) x);

	// Initialise stack for the new process
	PUSH(param);				/* First data */
	PUSH(new_proc);				/* Self */
	PUSH(0);					/* Dummy return address */
	PUSH(ptr_to_new_proc);		/* Entry point of new process */
	PUSH(0);					/* EAX */
	PUSH(0);					/* ECX */
	PUSH(0);					/* EDX */
	PUSH(0);					/* EBX */
	PUSH(0);					/* EBP */
	PUSH(0);					/* ESI */
	PUSH(0);					/* EDI */

	#undef PUSH
		// Save context ptr (Current process stack pointer)
		new_proc->esp = esp;
		add_ready_queue(new_proc);

		return NULL;


}

PROCESS fork()
{
    // Dummy return to make gcc happy
    return (PROCESS) NULL;
}

void print_process_heading(WINDOW* wnd){
	wprintf(wnd, "State   			Active   Priority 	Name\n");
	wprintf(wnd, "-------------------------------------------\n");
}

void print_process_details(Wnd* wnd, PROCESS p){
	static const char* state[] = {
		"READY 			",
		"ZOMBIE			",
		"SEND_BLOCKED	",
		"REPLY_BLOCKED	",
		"RECEIVE_BLOCKED",
		"MESSAGE_BLOCKED",
		"INTR_BLOCKED"
	};
	if(! p->used){
		wprintf(wnd, "PCB slot unused");
		return;
	}
	// State
	wprintf(wnd, state[p->state]);
	// Check active processes
	if(p == active_proc){
		wprintf(wnd, "*		");
	}
	else{
		wprintf(wnd, "		");
	}
	// Priority
	wprintf(wnd, "%2d", p->priority);
	// Name
	wprintf(wnd, "%s\n", p->name);

}


void print_process(WINDOW * wnd, PROCESS p)
{
	print_process_heading(wnd);
	print_process_details(wnd, p);
}

void print_all_processes(WINDOW * wnd)
{
	int i = 0;
	PCB* p = pcb;

	print_process_heading(wnd);
	for(i = 0; i < MAX_PROCS ; i++, p++){
		if(!p->used){
			continue;
		}
		print_process_details(wnd, p);
	}
}



void init_process()
{
	int i;

	/* Initialize or clear all PCBs. The first PCB is for Booting */
	for(i=1; i<MAX_PROCS; i++){
		PCB[i].magic = 0;
		PCB[i].used = FALSE;
	}

	/* Create a free list */
	for(i=1;i<(MAX_PROCS - 1);i++){
		pcb[i].next = &pcb[i + 1];
		pcb[MAX_PROCS - 1].next = NULL;
		next_free_pcb = &pcb[1];
	}

	/* Define PCB[0] for this process */
	active_proc = pcb;
	pcb[0].magic = MAGIC_PCB;
	pcb[0].used = TRUE;
	pcb[0].state = STATE_READY;
	pcb[0].priority = 1;
	pcb[0].first_port = NULL:
	pcb[0].name = "BOOT PROCESS";
}
