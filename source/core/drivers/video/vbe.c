#include <drivers/vbe.h>

#define FP_SEG(fptr) ((unsigned)((unsigned long)(fptr) >> 16))
#define FP_OFF(fptr)  ((unsigned)(fptr))

typedef struct __attribute__ ((packed)) {
    unsigned short di, si, bp, sp, bx, dx, cx, ax;
    unsigned short gs, fs, es, ds, eflags;
} regs16_t;
 
// tell compiler our int32 function is external
extern void int32(unsigned char intnum, regs16_t *regs);
extern long *double_buffer;

extern bool graphical_mode;

char mystr[256];
char *get_str();
int     xres,yres;                  /* Resolution of video mode used    */
int     bytesperline;               /* Logical CRT scanline length      */
int     curBank;                    /* Current read/write bank          */
unsigned int bankShift;             /* Bank granularity adjust factor   */
int     oldMode;                    /* Old video mode number            */
long    *screenPtr;             /* Pointer to start of video memory */
void    (*bankSwitch)(void);    /* Direct bank switching function   */

/*------------------------ VBE Interface Functions ------------------------*/

// Get SuperVGA information
int getVbeInfo()
{
    	regs16_t regs;
    	char *VbeInfo = (char *)&VbeInfoBlock;
    	regs.ax = 0x4F00;
    	regs.di = FP_OFF(VbeInfo);
    	regs.es = FP_SEG(VbeInfo);
    	int32(0x10, &regs);
    	if(regs.ax == 0x4F)
		return 0;
    	else
		return 1;
}

// Get video mode information given a VBE mode number
int getModeInfo()
{
    	regs16_t regs;
    	char *modeInfo = (char *)&ModeInfoBlock;
    	regs.ax = 0x4F01;
    	regs.cx = 0x4118;
    	regs.di = FP_SEG(modeInfo);
    	regs.es = FP_OFF(modeInfo);
    	int32(0x10, &regs);
    	if (regs.ax != 0x4F) 
		return 0;
    	if ((ModeInfoBlock.ModeAttributes & 0x1)
            && ModeInfoBlock.MemoryModel == memPK
            && ModeInfoBlock.BitsPerPixel == 8
            && ModeInfoBlock.NumberOfPlanes == 1)
        	return 1;
    	return 0;
}

// Set a VBE video mode
void setVBEMode()
{
    	regs16_t regs;
    	regs.ax = 0x4F02; 
    	regs.bx = 0x4118;
    	int32(0x10,&regs);
}

// Return the current VBE video mode
int getVBEMode(void)
{
    	regs16_t regs;
    	regs.ax = 0x4F03;
    	int32(0x10,&regs);
    	return regs.bx;
}

// Switch to text mode 80*25
void switch_to_textmode()
{
    	regs16_t regs;
	regs.ax = 0x0003;
	int32(0x10, &regs);
}

// Initialize graphics
int init_vbe_graphics()
{
	if(getVbeInfo() != 0)
		return 1;
	getModeInfo();

      	screenPtr = (long *)0xE0000000;

      	setVBEMode();

	memset((long *)0xE0000000, 0x00000000, 0x300000);

	graphical_mode = true;

	return 0;
}

/* -------------------------- Application Functions ------------------------ */
//lfb
void putPixel_32bpp(unsigned short x, unsigned short y, unsigned color) 
{
	unsigned offset = y*1024 + x;
 	double_buffer[offset] = color;
}
void putPixel_mem(unsigned short x, unsigned short y, unsigned color) 
{
	unsigned offset = y*1024 + x;
 	screenPtr[offset] = color;
}
