// -----------------------------------------------------------------------
//Sets VGA-compatible video modes without using the BIOS
//Chris Giese <geezer@execpc.com>	http://my.execpc.com/~geezer
//Release date: ?
//This code is public domain (no copyright).
//You can do whatever you want with it.
// link: http://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
// -----------------------------------------------------------------------
#define inportb inb
#define outportb outb

#include <arch/regs.h>
#include <system/user.h>
#include <initrd/initrd.h>
#include <lib/types.h>
#include <gui/screen.h>
#include <lib/stdio.h>
#include <arch/pokex.h>

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
#define VGA_CRTC_INDEX		0x3D4		
#define VGA_CRTC_DATA		0x3D5		
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
				VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

unsigned g_wd, g_ht;
extern int graphical_mode;
extern void getfont(unsigned char character);
extern char* font;
//control panel
extern user_t current_user;
extern u32int memsize;
extern unsigned char second;
extern unsigned char minute;
extern unsigned char hour;
extern unsigned char day;
extern unsigned char month;
extern unsigned int year;

unsigned char g_80x25_text[] =
{
/* MISC */
	0x67,
/* SEQ */
	0x03, 0x00, 0x03, 0x00, 0x02,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
	0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x50,
	0x9C, 0x0E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x0C, 0x00, 0x0F, 0x08, 0x00
};

unsigned char g_320x200x256[] =
{
	/* MISC */
	0x63,
	/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x0E,
	/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
	/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
	/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};

static void dump(unsigned char *regs, unsigned count)
{
	unsigned i;

	i = 0;
	printf("\t");
	for(; count != 0; count--)
	{
		printf("0x%02X,", *regs);
		i++;
		if(i >= 8)
		{
			i = 0;
			printf("\n\t");
		}
		else
			printf(" ");
		regs++;
	}
	printf("\n");
}

void dump_regs(unsigned char *regs)
{
	printf("unsigned char g_mode[] =\n");
	printf("{\n");
	/* dump MISCELLANEOUS reg */
	printf("/* MISC */\n");
	printf("\t0x%02X,\n", *regs);
	regs++;
	/* dump SEQUENCER regs */
	printf("/* SEQ */\n");
	dump(regs, VGA_NUM_SEQ_REGS);
	regs += VGA_NUM_SEQ_REGS;
	/* dump CRTC regs */
	printf("/* CRTC */\n");
	dump(regs, VGA_NUM_CRTC_REGS);
	regs += VGA_NUM_CRTC_REGS;
	/* dump GRAPHICS CONTROLLER regs */
	printf("/* GC */\n");
	dump(regs, VGA_NUM_GC_REGS);
	regs += VGA_NUM_GC_REGS;
	/* dump ATTRIBUTE CONTROLLER regs */
	printf("/* AC */\n");
	dump(regs, VGA_NUM_AC_REGS);
	regs += VGA_NUM_AC_REGS;
	printf("};\n");
}

void read_regs(unsigned char *regs)
{
	unsigned i;

	/* read MISCELLANEOUS reg */
	*regs = inportb(VGA_MISC_READ);
	regs++;
	/* read SEQUENCER regs */
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		outportb(VGA_SEQ_INDEX, i);
		*regs = inportb(VGA_SEQ_DATA);
		regs++;
	}
	/* read CRTC regs */
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		outportb(VGA_CRTC_INDEX, i);
		*regs = inportb(VGA_CRTC_DATA);
		regs++;
	}
	/* read GRAPHICS CONTROLLER regs */
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		outportb(VGA_GC_INDEX, i);
		*regs = inportb(VGA_GC_DATA);
		regs++;
	}
	/* read ATTRIBUTE CONTROLLER regs */
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)inportb(VGA_INSTAT_READ);
		outportb(VGA_AC_INDEX, i);
		*regs = inportb(VGA_AC_READ);
		regs++;
	}
	/* lock 16-color palette and unblank display */
	(void)inportb(VGA_INSTAT_READ);
	outportb(VGA_AC_INDEX, 0x20);
}

void write_regs(unsigned char *regs)
{
	unsigned i;

	/* write MISCELLANEOUS reg */
	outportb(VGA_MISC_WRITE, *regs);
	regs++;
	/* write SEQUENCER regs */
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		outportb(VGA_SEQ_INDEX, i);
		outportb(VGA_SEQ_DATA, *regs);
		regs++;
	}
	/* unlock CRTC registers */
	outportb(VGA_CRTC_INDEX, 0x03);
	outportb(VGA_CRTC_DATA, inportb(VGA_CRTC_DATA) | 0x80);
	outportb(VGA_CRTC_INDEX, 0x11);
	outportb(VGA_CRTC_DATA, inportb(VGA_CRTC_DATA) & ~0x80);
	/* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
	/* write CRTC regs */
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		outportb(VGA_CRTC_INDEX, i);
		outportb(VGA_CRTC_DATA, *regs);
		regs++;
	}
	/* write GRAPHICS CONTROLLER regs */
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		outportb(VGA_GC_INDEX, i);
		outportb(VGA_GC_DATA, *regs);
		regs++;
	}
	/* write ATTRIBUTE CONTROLLER regs */
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)inportb(VGA_INSTAT_READ);
		outportb(VGA_AC_INDEX, i);
		outportb(VGA_AC_WRITE, *regs);
		regs++;
	}
	/* lock 16-color palette and unblank display */
	(void)inportb(VGA_INSTAT_READ);
	outportb(VGA_AC_INDEX, 0x20);
}

static void set_plane(unsigned p)
{
	unsigned char pmask;

	p &= 3;
	pmask = 1 << p;
	/* set read plane */
	outportb(VGA_GC_INDEX, 4);
	outportb(VGA_GC_DATA, p);
	/* set write plane */
	outportb(VGA_SEQ_INDEX, 2);
	outportb(VGA_SEQ_DATA, pmask);
}

static unsigned get_fb_seg(void)
{
	unsigned seg;

	outportb(VGA_GC_INDEX, 6);
	seg = inportb(VGA_GC_DATA);
	seg >>= 2;
	seg &= 3;
	switch(seg)
	{
	case 0:
	case 1:
		seg = 0xA000;
		break;
	case 2:
		seg = 0xB000;
		break;
	case 3:
		seg = 0xB800;
		break;
	}
	return seg;
}

static void vpokeb(unsigned off, unsigned val)
{
	pokeb(get_fb_seg(), off, val);
}

static unsigned vpeekb(unsigned off)
{
	return peekb(get_fb_seg(), off);
}

void (*g_write_pixel)(unsigned x, unsigned y, unsigned c);

static void write_pixel8(unsigned x, unsigned y, unsigned c)
{
	unsigned wd_in_bytes;
	unsigned off;

	wd_in_bytes = g_wd;
	off = wd_in_bytes * y + x;
	vpokeb(off, c);
}

static void write_pixel8x(unsigned x, unsigned y, unsigned c)
{
	unsigned wd_in_bytes;
	unsigned off;

	wd_in_bytes = g_wd / 4;
	off = wd_in_bytes * y + x / 4;
	set_plane(x & 3);
	vpokeb(off, c);
}

static void dump_state(void)
{
	unsigned char state[VGA_NUM_REGS];

	read_regs(state);
	dump_regs(state);
}
// -----------------------------------------------------------------------------------

//windows
void simple_draw_window(int xi, int yi, int width, int height, char* title, int c)
{
	for(int y = yi; y < yi+15; y++)
	{
		for(int x = xi; x< xi+width; x++)
		{
			if(y == yi || x == xi || x == xi+width-1)
				g_write_pixel(x, y, VGA_COLOR_BLACK);
			else	
				g_write_pixel(x, y, VGA_COLOR_WHITE);
		}
	}

	for(int y = yi+15; y < yi+height; y++)
	{
		for(int x = xi; x< xi+width; x++)
		{
			if(y == yi+15)
				g_write_pixel(x, y, VGA_COLOR_BLACK);
			else if(x == xi || x == xi+width-1 || y == yi+height-1)
				g_write_pixel(x, y, VGA_COLOR_BLACK);
			else
				g_write_pixel(x, y, c);
		}
	}
	
	int text_size = strlen(title);
	int middle = floor((width/2)-(text_size/2));
	for(int i=0; i< text_size; i++)
		draw_char(xi+middle+2+i*8, yi+3, title[i], VGA_COLOR_BLACK);
}
//mem usage
void simple_draw_mem_usage(void)
{
	simple_draw_square(4, 62, 130, 25, "Mem Usage", VGA_COLOR_BLACK);
	char* _text2 = "";
	int m = get_memory_usage()/1024/1024;
	itoa(_text2, 10, m);

	//draw used memory
	int where = 5;
	for(int i=0; i< strlen(_text2); i++)
	{
		where += 8;
		draw_char(where, 75, _text2[i], VGA_COLOR_BLACK);
	}


	//draw MB/
	draw_char(where+8, 75, 'M', VGA_COLOR_BLACK);
	draw_char(where+16, 75, 'B', VGA_COLOR_BLACK);
	draw_char(where+24, 75, '/', VGA_COLOR_BLACK);
	where += 24;

	char* _text3 = "";	
	int mm = memsize/1024/1024;
	itoa(_text3, 10, mm);

	//draw memory size
	for(int i=0; i< strlen(_text3); i++)
	{
		where += 8;
		draw_char(where, 75, _text3[i], VGA_COLOR_BLACK);	
	}

	//draw MB
	draw_char(where+8, 75, 'M', VGA_COLOR_BLACK);
	draw_char(where+16, 75, 'B', VGA_COLOR_BLACK);
}
void simple_draw_square(int xi, int yi, int width, int height, char* title, unsigned color)
{
	int length = strlen(title);
	//draw top & bottom lines
	for(int x=xi; x<xi+width; x++)
	{
		if(x == xi+2)
		{
			for(int i=0; i< length; i++)
				draw_char(x+(i*8), yi-2, title[i], VGA_COLOR_BLACK);
		}
		else if(x > xi+4+(length*8) || x < xi+2)
			g_write_pixel(x, yi, color);
		g_write_pixel(x, yi+height, color);
	}
	//draw middle lines
	for(int y=yi; y<yi+height; y++)
	{
		g_write_pixel(xi, y, color);
		g_write_pixel(xi+width, y, color);
	}
}
//menu time
void simple_draw_menu_time(void)
{
	char *_hours = "1";
	char *_mins = "2";
	char *_secs = "3";
	 
	itoa(_hours, 10, hour);
	itoa(_mins, 10, minute);
	itoa(_secs, 10, second);

	for(int i=0; i< strlen(_hours); i++)
		draw_char(g_wd-80+(i*8), 3, _hours[i], VGA_COLOR_BLACK);

		draw_char(g_wd-64, 2, ':', VGA_COLOR_BLACK);

	for(int i=0; i< strlen(_mins); i++)
		draw_char(g_wd-56+(i*8), 3, _mins[i], VGA_COLOR_BLACK);

		draw_char(g_wd-40, 2, ':', VGA_COLOR_BLACK);

	for(int i=0; i< strlen(_secs); i++)
		draw_char(g_wd-32+(i*8), 3, _secs[i], VGA_COLOR_BLACK);
}
//draw files
void simple_draw_files(void)
{
	int i, j, w = 0;
	char *filename = "";
	struct dirent *node = 0;
	while ( (node = readdir_fs(fs_root, i)) != 0)
	{
 		filename = node->name;
  		fs_node_t *fsnode = finddir_fs(fs_root, node->name);
	
  		if ((fsnode->flags&0x7) == FS_DIRECTORY)
		{
			for(j=0; j< strlen(filename); j++)
				draw_char(162+(j*8), 50+(10*i), filename[j], VGA_COLOR_BLACK);
		}
		else
		{
			for(j=0; j< strlen(filename); j++)
				draw_char(162+(j*8), 50+(10*i), filename[j], VGA_COLOR_BLACK);
		}
  		i++;
		w=0;
	} 	
}
//draw user
void simple_draw_user(void)
{
	char* text = "user: ";
	char* _username = current_user.name;

	for(int j=0; j< strlen(text); j++)
		draw_char(4+(j*8), 50, text[j], VGA_COLOR_BLACK);

	for(int j=0; j< strlen(_username); j++)
		draw_char(52+(j*8), 50, _username[j], VGA_COLOR_BLACK);
}
//control panel
void simple_draw_control_panel(void)
{
	simple_draw_window(2, 25, 150, 165, "Control Panel", VGA_COLOR_WHITE);
	simple_draw_user();
	simple_draw_mem_usage();
}
//file manager
void simple_draw_file_manager(void)
{
	simple_draw_window(160, 25, 150, 165, "Files", VGA_COLOR_WHITE);
	simple_draw_files();
}
//descktop
void simple_draw_desktop(void)
{
	unsigned x, y;

	/* clear screen */
	for(y = 0; y < g_ht; y++)
		for(x = 0; x < g_wd; x++)
			g_write_pixel(x, y, VGA_COLOR_LTWHITE);
	/* draw 2-color X */
	for(y = 0; y < 15; y++)
	{
		for(x = 0; x< g_wd; x++)
			g_write_pixel(x, y, VGA_COLOR_WHITE);
	}
	
	char* text = "Menu";
	for(int i=0; i<4; i++)
		draw_char(5+(i*8), 3, text[i], VGA_COLOR_BLACK);
}
void init_graphics(void)
{
	write_regs(g_320x200x256);
	g_wd = 320;
	g_ht = 200;
	g_write_pixel = write_pixel8;
	simple_draw_desktop();
	simple_draw_menu_time();
	simple_draw_control_panel();
	simple_draw_file_manager();
}

void init_simple_vga()
{
	init_graphics();
	graphical_mode = 1;
	
	static bool go_back = false;

	while(go_back == false)
	{
		if(getch() == '\b')
			go_back = true;		
	}
	write_regs(g_80x25_text);
	show_menu();
}
