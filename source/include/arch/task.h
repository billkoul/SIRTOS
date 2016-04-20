#ifndef TASK_H
#define TASK_H

#include <arch/paging.h>
#include <lib/types.h>

#define deadline_DEAD 99
#define deadline_IDLE 0
#define deadline_LOW 10
#define deadline_MEDIUM 20
#define deadline_HIGH 30
#define deadline_UNLIMITED 1

#define resources_LOW 100
#define resources_MEDIUM 200
#define resources_HIGH 300
#define resources_AUTO 1000

// This structure defines a 'task' - a process
struct task
{
    int id;                // Process ID.
    int thread;            // 0 if not a thread or thread ID
    
    u32int esp, ebp;       // Stack and base pointers.
    u32int eip;            // Instruction pointer.
    
    int deadline;
    int ticks;       	   // ticks task needs to run (max 90(100) or 0 for infinite-only for hard core tasks)	
    int ticks_passed;      // ticks passed
    u32int cycle;   	// cycle ended
    u32int cycle_end;   // cycle ended
    int ready;
    
    int argc;
    char** argv;
    
    page_directory_t *page_directory; // Page directory.
    
    struct task *next;  
};

typedef struct task task_t;

void init_tasking();

void switch_task(); 

int fork(int deadline, int cycles);

// Causes the current process' stack to be forcibly moved to a new location.
void move_stack(void *new_stack_start, u32int size);

int getpid();

void start_task(void (*func)(void*), void *arg, int deadline, int cycles);

int kill_task(int pid);

void exit();

void set_task_priority (int prio);

//Threads
int new_thread(int deadline, int cycles);
int start_thread(task_t *thread, int attr, void (*func)(void*), void *arg, int deadline, int cycles);
int kill_thread(task_t *thread);

#endif
