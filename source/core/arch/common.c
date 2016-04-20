#include <lib/types.h>

// Write a byte out to the specified port.
void outb(u16int port, u8int value)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (value));
}

//Read a single 8-bit I/O port. 
u8int inb(u16int port)
{
    u8int ret;
    __asm__ __volatile__("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void outb_p(u16int port, u8int value)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (value));
    sleep(3);
}


u8int inb_p(u16int port)
{
    u8int ret;
    __asm__ __volatile__("inb %1, %0" : "=a" (ret) : "dN" (port));
    sleep(3);
    return ret;
}

u16int inw(u16int port)
{
    u16int ret;
    __asm__ __volatile__ ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void outw(u16int port, u16int value)
{
    __asm__ __volatile__ ( "outw %w0, %1" : : "a" (value), "id" (port) );
}

void outl (u16int port, u32int val)
{
     __asm__ __volatile__ ("outl %0,%1":: "a" (val), "d" (port));
}

u32int inl(u16int port)
{
     static u32int ret;
     __asm__ __volatile__ ("inl %1,%0":: "a" (ret), "dN" (port));
     return ret;
}

int memcmp(const void* aptr, const void* bptr, size_t size)
{
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for ( size_t i = 0; i < size; i++ )
		if ( a[i] < b[i] )
			return -1;
		else if ( b[i] < a[i] )
			return 1;
	return 0;
}

// Copy len bytes from src to dest.
void memcpy(u8int *dest, const u8int *src, u32int len)
{
    const u8int *sp = (const u8int *)src;
    u8int *dp = (u8int *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

// Write len copies of val into dest.
void memset(u8int *dest, u8int val, u32int len)
{
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

void* memmove(void* dstptr, const void* srcptr, size_t size)
{
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if ( dst < src )
		for ( size_t i = 0; i < size; i++ )
			dst[i] = src[i];
	else
		for ( size_t i = size; i != 0; i-- )
			dst[i-1] = src[i-1];
	return dstptr;
}

char* strchr (const char* s,int c)
{
  do {
    if (*s == c)
      {
	return (char*)s;
      }
  } while (*s++);
  return (0);
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}
