#ifndef __ctypes__
#define __ctypes__

#define isascii(c)		(c >= 0 && c <= 127)
#define isdigit(c)		(c >= '0' && c <= '9')
#define islower(c)		(c >= 'a' && c <= 'z')
#define isupper(c)		(c >= 'A' && c <= 'Z')
#define tolower(c)		(isdigit(c) ? c : (islower(c) ? c : ((c - 'A') + 'a')))
#define toupper(c)		(isdigit(c) ? c : (isupper(c) ? c : ((c - 'a') + 'A')))

#define MIN(a, b)       ((a) < (b) ? (a) : (b))
#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define ABS(a)          ((a) < 0 ? -(a) : (a))
#define TRUNC(addr, align)      ((addr) & ~((align) - 1))
#define ALIGN_DOWN(addr, align) TRUNC(addr, align)
#define ALIGN_UP(addr, align)   ( ((addr) + (align) - 1) & (~((align) - 1)) )

#endif
