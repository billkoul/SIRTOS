#ifndef __types__
#define __types__

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned long		uint32_t;
typedef unsigned long long	uint64_t;
#define u_int8_t uint8_t
#define u_int16_t uint16_t
#define u_int32_t uint32_t
#define u_int64_t uint64_t
typedef unsigned int   	u32int;
typedef int   		s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char  u8int;
typedef  char  s8int;
typedef unsigned int	uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned int	dword;

typedef int size_t;

typedef short bool;
#define true	1
#define false	0

#define NULL	((void*) 0)

#define PACKED       __attribute__ ((__packed__))
#define ALIGNED(n)   __attribute__ ((aligned(n)))
#define fastcall     __attribute__ ((fastcall))

typedef int                 ino_t;
typedef unsigned      off_t;
typedef int                 uid_t;
typedef int                 gid_t;
typedef int                 pid_t;
typedef int                 mode_t;
typedef unsigned      nlink_t;
typedef unsigned      blksize_t;
typedef unsigned      blkcnt_t;
typedef long long      quad_t;
typedef unsigned long long  uquad_t;

#endif
