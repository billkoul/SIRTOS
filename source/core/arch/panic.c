#include <system.h>

extern task_t* currentTask;

void panic(registers_t* r)
{
     printf("Task PID: %d\n",currentTask->id);
     printf("EIP: 0x%x ESP: 0x%x EBP: 0x%x uESP: 0x%x\n",r->eip,r->esp,r->ebp,r->useresp);
     printf("EDI: 0x%x ESI: 0x%x\n",r->edi,r->esi,r->ebp);
     printf("EAX: 0x%x EBX: 0x%x ECX: 0x%x EDX: 0x%x\n",r->eax,r->ebx,r->ecx,r->edx);
     printf("CS: 0x%x DS: 0x%x SS: 0x%x\n",r->cs,r->ds,r->ss);
     printf("INT: 0x%x ERROR: 0x%x\n",r->int_no,r->err_code);
     cli();
     halt();
}

void panic_assert(const char *file, u32int line, const char *desc)
{
    printf("Assertion failed (%d) at %s:%d\n",desc,file,line);
}
