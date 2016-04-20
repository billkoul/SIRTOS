/* Ties all header files together for kernel */
#define DEBUG

#ifdef DEBUG
#define dprintf kprintf
#define DBGPRINTF(s) outb(0xe9, s);
#else
#define dprintf
#define DBGPRINTF(s)
#endif

#include <arch/common.h>             // common functions
#include <arch/regs.h>               // regs struct
#include <arch/asm.h>                // cpu specific asm functions
#include <arch/isr.h>                // ISR/IRQ
#include <arch/descriptor_tables.h>  // IDT/GDT stuff
#include <arch/paging.h>             // paging setup
#include <arch/kheap.h>              // heap setup/MMU
#include <arch/task.h>               // tasks/multitasking
#include <arch/pokex.h>              // poke/peek byte inline ASM functions
#include <fs/fs.h>                   // VFS functions
#include <initrd/initrd.h>           // initial ramdisk
#include <system/ordered_array.h>    // ordered arrays for linked lists
#include <system/text.h>             // text mode functions
#include <system/timer.h>            // PIT timer
#include <system/multiboot.h>        // multiboot information
#include <system/syscalls.h>         // syscall defines/prototypes
#include <system/user.h>             // user functions
#include <system/version.h>          // current version
#include <drivers/keyb.h>            // Keyboard initialisation
#include <drivers/ps2.h>             // ps2 mouse
#include <lib/types.h>               // types
#include <lib/ctypes.h>              // ctypes
#include <lib/math.h>                // maths functions

/* For files without headers */
void init_timer(u32int frequency);
void timer_wait(int ticks);
void panic(registers_t* r);
void panic_assert(const char *file, u32int line, const char *desc);
void schedule();
void beep();
int init_pci();
