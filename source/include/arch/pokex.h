/* poke/peek byte functions */
#ifndef POKE_X_H
#define POKE_X_H

#define	pokeb(S,O,V)		_farpokeb(0x10, 16uL * (S) + (O), V)
#define	peekb(S,O)		_farpeekb(0x10, 16uL * (S) + (O))
#define	pokew(S,O,V)		_farpokeb(0x10, 16uL * (S) + (O), V)
#define	pokel(S,O,V)		_farpokeb(0x10, 16uL * (S) + (O), V)
#define	peekl(S,O)		_farpeekl(0x10, 16uL * (S) + (O))

void _farpokeb(unsigned short selector,
	 unsigned long offset,
	 unsigned char value);
	 
unsigned char _farpeekb(unsigned short, unsigned long);

void _farpokew(unsigned short selector,
	 unsigned long offset,
	 unsigned char value);
	 
void _farpokel(unsigned short selector,
	 unsigned long offset,
	 unsigned char value);
	 
unsigned long _farpeekl(unsigned short selector, unsigned long offset);

#endif
