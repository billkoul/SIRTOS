#include <system/syscalls.h>
#include <lib/types.h>
#include <arch/regs.h>

extern void printf (const char *format);

DEFN_SYSCALL1(printf, 0, const char*);

static void syscall_handler(registers_t *regs);

static void *syscalls[1] =
{
   &printf
};
u32int num_syscalls = 1;

void initialise_syscalls()
{
   // Register our syscall handler.
   install_interrupt_handler (0x80, &syscall_handler);
}

void syscall_handler(registers_t *regs)
{
   // check if the requested syscall number is valid.
   if (regs->eax >= num_syscalls)
       return;

   void *location = syscalls[regs->eax]; // Get the syscall location.

   // We don't know how many parameters the function wants, so we just
   // push them all onto the stack in the correct order. The function will
   // use all the parameters it wants, and we can pop them all back off afterwards
   int ret;
   __asm__ __volatile__ (" \
     push %1; \
     push %2; \
     push %3; \
     push %4; \
     push %5; \
     call *%6; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
   " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (location));
   regs->eax = ret;
} 
