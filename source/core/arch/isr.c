#include <system.h>

isr_t interrupt_handlers[256];

void install_interrupt_handler(u8int irq_number, isr_t handler)
{
    	interrupt_handlers[irq_number] = handler;
}


// Interrupt Service Routine
void isr_handler(registers_t regs)
{
    	u8int int_no = regs.int_no & 0xFF;
    	//printf(" INT:0x%x ",int_no);
    	if (interrupt_handlers[int_no] != 0)
    	{
        	isr_t handler = interrupt_handlers[int_no];
        	handler(&regs);
        	return;
    	}
}

// This gets called from ASM interrupt handler stub.
void irq_handler(registers_t regs)
{   
    	if (regs.int_no >= 40)    
        	outb(0xA0, 0x20); // Send EOI (end of interrupt) signal to slave (master doesnt need that)
   	outb(0x20, 0x20); // Send EOI (end of interrupt) signal to master (slave needs that too)
	
       	isr_t handler = interrupt_handlers[regs.int_no];
       	handler(&regs);

}
