// -----------------------------------------------------------------------------
// 		a part of the code here is Based on James Molloy's tutorial
// -----------------------------------------------------------------------------
#include <system.h>

volatile task_t *currentTask; //running task

volatile task_t *readyQueue; //linked list

extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern void alloc_frame(page_t*,int,int);
extern u32int initial_esp;
extern u32int read_eip();
extern void moveToPl3();
extern u32int cycle;
extern task_t nextTask;
int tasks_in_list = 0;
u32int nextPid = 1;
u32int nextThread = 1;
extern int cycle_duration;

void moveStack(void *new_stack_start, u32int size)
{
  	u32int i;
  	// Allocate some space for the new stack.
  	for( i = (u32int)new_stack_start;
       		i >= ((u32int)new_stack_start-size);
       		i -= 0x1000)
 	{ 
    	//usermode stack
    	alloc_frame( get_page(i, 1, current_directory), 0, 1);
  	}
  
  	//flush TLB
 	u32int pd_addr;
  	__asm__ __volatile__("mov %%cr3, %0" : "=r" (pd_addr));
  	__asm__ __volatile__("mov %0, %%cr3" : : "r" (pd_addr));

  	// Old ESP and EBP, read from registers.
  	u32int old_stack_pointer; __asm__ __volatile__("mov %%esp, %0" : "=r" (old_stack_pointer));
  	u32int old_base_pointer;  __asm__ __volatile__("mov %%ebp, %0" : "=r" (old_base_pointer));

  	// Offset to add to old stack addresses to get a new stack address.
  	u32int offset            = (u32int)new_stack_start - initial_esp;

  	// New ESP and EBP.
  	u32int new_stack_pointer = old_stack_pointer + offset;
 	u32int new_base_pointer  = old_base_pointer  + offset;

  	// Copy the stack.
  	memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, initial_esp-old_stack_pointer);

  	// Backtrace through the original stack, copying new values into
  	// the new stack.  
  	for(i = (u32int)new_stack_start; i > (u32int)new_stack_start-size; i -= 4)
  	{
    		u32int tmp = * (u32int*)i;
    		// If the value of tmp is inside the range of the old stack, assume it is a base pointer
    		// and remap it. This will unfortunately remap ANY value in this range, whether they are
    		// base pointers or not.
    		if (( old_stack_pointer < tmp) && (tmp < initial_esp))
    		{
      			tmp = tmp + offset;
      			u32int *tmp2 = (u32int*)i;
      			*tmp2 = tmp;
    		}
  	}

  	// Change stacks.
  	__asm__ __volatile__("mov %0, %%esp" : : "r" (new_stack_pointer));
  	__asm__ __volatile__("mov %0, %%ebp" : : "r" (new_base_pointer));
}

void init_tasking()
{
    	cli();
    
    	moveStack((void*)0x10000000, 0x2000); //userland stack

    	//kernel task
    	currentTask = readyQueue = (task_t*)kmalloc(sizeof(task_t));
   	currentTask->id = nextPid++;
    	currentTask->thread = 0;
    	currentTask->esp = currentTask->ebp = 0;
    	currentTask->eip = 0;
    	currentTask->deadline = deadline_HIGH;
    	currentTask->ticks = resources_LOW;
    	currentTask->ticks_passed = 0;
    	currentTask->cycle = 0;
    	currentTask->cycle_end = 0;
    	currentTask->ready = 1;
    	currentTask->page_directory = current_directory;
    	currentTask->next = 0;

   	tasks_in_list++;

    	sti();
}

void EDF_schedule()
{
	cli();
	currentTask->cycle_end = cycle;

	currentTask = (task_t*)readyQueue; //start at the beginning of sorted linked list

	while(currentTask->cycle_end == cycle) //if task finished in this cycle take next task
	{
		if(currentTask->next)
		{
			currentTask = currentTask->next;
		}
		else
			break;
	}

	currentTask->ticks_passed = 0; //zeroes ticks_passed
	currentTask->ticks = floor(cycle_duration/tasks_in_list); //calculate and give periodic type to task
	sti();
}


void switch_task()
{   	
    	u32int esp, ebp, eip; // Read esp, ebp now for saving later on.
     
    	if (!currentTask)
        	return;
                   
    	__asm__ __volatile__("mov %%esp, %0" : "=r"(esp));
    	__asm__ __volatile__("mov %%ebp, %0" : "=r"(ebp));

    	// One of two things could have happened when this function exits 
    	//   (a) We called the function and it returned the EIP as requested.
   	//   (b) We have just switched tasks, and because the saved EIP is essentially
    	//       the instruction after read_eip(), it will seem as if read_eip has just
    	//       returned.
   	 // In the second case we need to return immediately. To detect it we put a dummy
    	// value in EAX further down at the end of this function. As C returns values in EAX,
    	// it will look like the return value is this dummy value! (0x12345).
    	eip = read_eip();
	
    	if (eip == 0x12345) //0x12345 = magic bit for a task that's just switched
        	return;
    
    	//No, we didn't switch tasks. Let's save some register values and switch.
    	currentTask->eip = eip;
    	currentTask->esp = esp;
    	currentTask->ebp = ebp; 


	EDF_schedule();
 
    	eip = currentTask->eip;
    	esp = currentTask->esp;
    	ebp = currentTask->ebp;

    	DBGPRINTF(currentTask->id);

    	current_directory = currentTask->page_directory; // Make sure the memory manager knows we've changed page directory.
    	// * Stop interrupts so we don't get interrupted.
    	// * Temporarily puts the new EIP location in ECX.
    	// * Loads the stack and base pointers from the new task struct.
    	// * Changes page directory to the physical address (physicalAddr) of the new directory.
    	// * Puts a dummy value (0x12345) in EAX so that above we can recognise that we've just
    	//   switched task.
    	// * Restarts interrupts. The STI instruction has a delay - it doesn't take effect until after
    	//   the next instruction.
    	// * Jumps to the location in ECX (remember we put the new EIP in there).
    	__asm__ __volatile__("         \
      	cli;                 \
      	mov %0, %%ecx;       \
     	 mov %1, %%esp;       \
      	mov %2, %%ebp;       \
     	 mov %3, %%cr3;       \
      	mov $0x12345, %%eax; \
      	sti;                 \
      	jmp *%%ecx           "
                 : : "r"(eip), "r"(esp), "r"(ebp), "r"(current_directory->physicalAddr));
}

int fork(int deadline, int ticks)
{
    	cli();
    	int time;
 	//set maximum ticks to 9 so we can run at least 2 tasks in every cycle
	//(unless it has deadline 0 which means its hard core and only stops running on exit
	if(ticks == resources_AUTO)
		time = floor(cycle_duration/tasks_in_list);

    	task_t *parent_task = (task_t*)currentTask;

    	//clone directory space
    	page_directory_t *directory = clone_directory(current_directory);

    	//create stuct for new process
    	task_t *new_task = (task_t*)kmalloc(sizeof(task_t));

    	new_task->id = nextPid++;
    	new_task->thread = 0;
   	new_task->esp = new_task->ebp = 0;
    	new_task->eip = 0;
   	new_task->deadline = deadline;
    	new_task->ticks = time;
    	new_task->ticks_passed = 0;
    	new_task->cycle = cycle;
    	new_task->cycle_end = 0;
   	new_task->ready = 1;
    	new_task->page_directory = directory;
    	new_task->next = 0;

	tasks_in_list++;

    	// --------------------------------- re order list based on deadlines for EDF performance -------------------------------

    	task_t *tmp_task = (task_t*)readyQueue;
	task_t *prv_task = (task_t*)readyQueue;

	if(!tmp_task->next) //this is the first task (after kernel which is first in list always)
	{
		tmp_task->next = new_task;
	}
	else
	{
    		while (tmp_task->next) //there is a next task
		{
			prv_task = tmp_task; //previous task = current task
        		tmp_task = tmp_task->next; //current task = next task

			if(tmp_task->deadline >= new_task->deadline) //current task has later deadline than new task
			{
				prv_task->next = new_task;
				new_task->next = tmp_task;
			}
		}
	}

	kfree((void*)prv_task);
	kfree((void*)tmp_task);
	// ------------------------------------------------------------------------------------------------------------------

    	//entry point
    	u32int eip = read_eip();


    	if (currentTask == parent_task)
    	{
        	// We are the parent, so set up the esp/ebp/eip for our child.
        	u32int esp; __asm__ __volatile__("mov %%esp, %0" : "=r"(esp));
        	u32int ebp; __asm__ __volatile__("mov %%ebp, %0" : "=r"(ebp));
        	new_task->esp = esp;
        	new_task->ebp = ebp;
        	new_task->eip = eip;
        	sti();

        	return new_task->id;
    	}
    	else
    	{
        	
        	return 0; // We are the child.
    	}

}

void setTask_deadline (int deadline)
{
     cli();
     currentTask->deadline = deadline;
     sti();
}

int getpid()
{
	return currentTask->id;
}

void start_task(void (*func)(void*), void *arg, int deadline, int ticks)
{
	if (fork(deadline, ticks) == 0)
	{
    		func(arg);
    		exit();
    		for(;;);
  	}
}

int kill_task(int pid)
{
	task_t *task_d = 0;
	task_t *task_r = (task_t*)readyQueue;

     	for(;task_r->next!=0;task_r=task_r->next)
     	{
     		if(task_r->id == pid)
          		//We got the previous task
          		task_d=task_r;
     	}
    		if(!task_d)
          	return 1;
          	task_r = task_d->next;
          	task_d->next = task_r->next;
          
     	kfree((void*)task_r);   

	tasks_in_list--;  
     	return 0;
}

void exit()
{
     	cli();
     	currentTask->deadline = deadline_DEAD;
    	currentTask->ready = 0;
     
    	//Find previous task
     	task_t *task_d = 0;
     	task_t *task_r = (task_t*)readyQueue;
     	for(;task_r->next!=0;task_r=task_r->next)
     	{
        	if(task_r->next == currentTask)
          	{
               		task_d=task_r;
               		break;
          	}
     	}
     
     	//We didn't find the task
     	if(!task_d)
        	return;

     	task_d->next = currentTask->next;

     	kfree((void*)currentTask);

	tasks_in_list--;
     
     	sti(); 
     	switch_task(); //Don't waste any time
}

int new_thread(int deadline, int ticks)
{
    	cli();
	int time;
	if(ticks == resources_AUTO)
		 time = resources_AUTO;
    
    	task_t *parent_task = (task_t*)currentTask;

    	task_t *new_thread = (task_t*)kmalloc(sizeof(task_t));

    	new_thread->id = parent_task->id;
    	new_thread->thread = nextThread++;
    	new_thread->esp = parent_task->esp;
    	new_thread->esp = parent_task->ebp;
    	new_thread->eip = 0;
    	new_thread->deadline = parent_task->deadline;
    	new_thread->ticks = time;
    	new_thread->ticks_passed = 0;
    	new_thread->cycle = 0;
    	new_thread->cycle_end = 0;
    	new_thread->ready = 1;
    	new_thread->page_directory = parent_task->page_directory;
    	new_thread->next = 0;

	tasks_in_list++;

    	// --------------------------------- re order list based on deadlines for performance -------------------------------

    	task_t *tmp_task = (task_t*)readyQueue;
	task_t *prv_task = (task_t*)readyQueue;

	if(!tmp_task->next) //this is the first task (after kernel which is first in list always)
	{
		tmp_task->next = new_thread;
	}
	else
	{
    		while (tmp_task->next) //there is a next task
		{
			prv_task = tmp_task; //previous task = current task
        		tmp_task = tmp_task->next; //current task = next task

			if(tmp_task->deadline > new_thread->deadline) //current task has later deadline than new task
			{
				prv_task->next = new_thread;
				new_thread->next = tmp_task;
			}
		}
	}

	// ------------------------------------------------------------------------------------------------------------------

    	//entry point
   	u32int eip = read_eip();


    	if (currentTask == parent_task)
    	{
        	u32int esp; __asm__ __volatile__("mov %%esp, %0" : "=r"(esp));
        	u32int ebp; __asm__ __volatile__("mov %%ebp, %0" : "=r"(ebp));
        	new_thread->esp = esp;
        	new_thread->ebp = ebp;
        	new_thread->eip = eip;
        	sti();

        	return new_thread->id;
    	}
    	else
    	{
        	return 0;
    	}

}


int startThread(task_t *thread, int attr, void (*func)(void*), void *arg, int deadline, int ticks)
{
   	if (new_thread(deadline, ticks) == 0)
   	{
       		func(arg);
       		exit();
       		for(;;);
   	}

   	killThread(thread);
   	return 1;
}

int killThread(task_t *thread)
{
     	task_t *task_d = 0;
     	task_t *task_r = (task_t*)readyQueue;

     	for(;task_r->next!=0;task_r=task_r->next)
     	{
     		if(task_r->id == thread->id && task_r->thread == thread->thread)
         		 task_d=task_r;
     	}

     	if(!task_d)
          	return 1;
          	task_r = task_d->next;
          	task_d->next = task_r->next;
          
     	kfree((void*)task_r);     

	tasks_in_list--;
     	return 0;
}
