
#include <kernel.h>


PCB             pcb[MAX_PROCS];


PORT create_process(void (*ptr_to_new_proc) (PROCESS, PARAM),
                    int prio, PARAM param, char *name)
{
	
}


PROCESS fork()
{
    // Dummy return to make gcc happy
    return (PROCESS) NULL;
}




void print_process(WINDOW * wnd, PROCESS p)
{
}

void print_all_processes(WINDOW * wnd)
{
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
