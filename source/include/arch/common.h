#ifndef COMMON_H
#define COMMON_H

#include <lib/types.h>

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);
void outw(u16int port, u16int value);
u32int inl(u16int port);
void outl (u16int port, u32int val);
void outb_p(u16int port, u8int value);
u8int inb_p(u16int port);

void memcpy(u8int *dest, const u8int *src, u32int len);
void memset(u8int *dest, u8int val, u32int len);
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);

char* strcat(char *dest, const char *src);
char* strchr (const char* s,int c);

#define inportb inb
#define outportb outb
#define inportw inw
#define inportl inl
#define outportw outw
#define outportl outl

#define IDEP_IRQ        0x0E
#define IDES_IRQ        0x0F

#ifdef DEBUG
#define dprintf kprintf
#define DBGPRINTF(s) outb(0xe9, s);
#else
#define dprintf
#define DBGPRINTF(s)
#endif

#endif
