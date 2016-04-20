/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	PIT
*/
#include <arch/time.h>
#include <lib/types.h>
#include <arch/regs.h>
#include <arch/isr.h>
#include <arch/task.h>

volatile uint64_t cycle = 0;
volatile int ticks = 0;
volatile  u32int wait_ticks = 1;
extern task_t currentTask;
int cycle_duration;

static void timer_callback(registers_t* regs)
{
	if(ticks > cycle_duration) //every cycle lasts 100 ticks
	{
		ticks = 0;
		cycle++;
	}
	ticks++;
    	wait_ticks++;
         
    	//check if task reached required ticks or if is hard core (0)
    	if(currentTask.ticks_passed < currentTask.ticks || currentTask.deadline == deadline_UNLIMITED)
    	{
        	currentTask.ticks_passed++;
    	}
    	else
		switch_task(); //No time left for process
}

void sleep(int ticks_to_wait)
{
    	wait_ticks = 0;
    	while(wait_ticks < ticks_to_wait);
}

void init_timer(u32int frequency) 
{
  	install_interrupt_handler(IRQ0, &timer_callback); //Register the timer callback.

    	outb(0x43, 0x36); // Send the command byte.

    	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    	u8int low = (u8int)(frequency & 0xFF);
    	u8int high = (u8int)((frequency>>8) & 0xFF );

    	// Send the number to devide frequency
    	outb(0x40, low);
    	outb(0x40, high);
}
