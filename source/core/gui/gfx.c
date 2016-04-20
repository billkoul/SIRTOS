/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	GRAPHICS
*/
#include <gui/gfx.h>	
#include <lib/types.h>	
#include <arch/task.h>	
	
extern unsigned char minute;
extern unsigned char hour;
extern unsigned char day;
extern unsigned char month;

char *_hours = "^1";
char *_mins = "^2";
char *_day = "^3";
char *_month = "^4";
char *double_buffer;
extern volatile int cursor_x, cursor_y;
extern unsigned workspace_color;
extern unsigned menubar_color;
extern int menubar_start;
extern int menubar_end;
volatile int time_hour = 24;
volatile char *c_buffer;

static void vsync()
{
      /* wait until any previous retrace has ended */
      while (inb(0x3DA) & 8);
      /* wait until a new retrace has just begun */
      while (!(inb(0x3DA) & 8));
}

static void g_flip() 
{
    	memcpy((long*)VIDEO_MEMORY, (long*)double_buffer,  0x300000);
}

void gfx_draw_workspace()
{
	//draw background
	for(int i=0;i<SCREEN_SIZE_X;i++)
	{
		for(int j=0;j<SCREEN_SIZE_Y;j++)
		{
			putPixel_32bpp(i, j, workspace_color);
		}
	}
}

void gfx_draw_menu_time()
{	

	int hour_choice = hour;
	if(time_hour == 12)
	{
		if(hour_choice > 12)
			hour_choice -= 12;
	}
	itoa(_hours, 10, hour_choice);
	itoa(_mins, 10, minute);
	itoa(_day, 10, day);
	itoa(_month, 10, month);

	int y_pos = menubar_start+10;

	draw_char(SCREEN_SIZE_X-142+(0*8),y_pos,'E',COLOR_BLACK);
	draw_char(SCREEN_SIZE_X-134+(0*8),y_pos,'N',COLOR_BLACK);
	draw_char(SCREEN_SIZE_X-126+(0*8),y_pos,'G',COLOR_BLACK);
	int i;
	//print day
	if(strlen(_day) == 1)
	{
		draw_char(SCREEN_SIZE_X-106+(0*8),y_pos,'0',COLOR_BLACK);
		draw_char(SCREEN_SIZE_X-106+(1*8),y_pos,_day[0],COLOR_BLACK);
	}
	else
	{
		for (i = 0; i < 2; i++)
			draw_char(SCREEN_SIZE_X-106+(i*8),y_pos,_day[i],COLOR_BLACK);
	}
	//print :
	draw_char(SCREEN_SIZE_X-90,y_pos,'/',COLOR_BLACK);	
	//print month
	if(strlen(_month) == 1)
	{
		draw_char(SCREEN_SIZE_X-80+(0*8),y_pos,'0',COLOR_BLACK);
		draw_char(SCREEN_SIZE_X-80+(1*8),y_pos,_month[0],COLOR_BLACK);
	}
	else
	{
		for (i = 0; i < 2; i++)
			draw_char(SCREEN_SIZE_X-80+(i*8),y_pos,_month[i],COLOR_BLACK);
	}
	i = 0;
	//print :
	draw_char(SCREEN_SIZE_X-64,y_pos,',',COLOR_BLACK);	
	//print hours		
	if(strlen(_hours) == 1)
	{
		draw_char(SCREEN_SIZE_X-54+(0*8),y_pos,'0',COLOR_BLACK);
		draw_char(SCREEN_SIZE_X-54+(1*8),y_pos,_hours[0],COLOR_BLACK);
	}
	else
	{
		for (i = 0; i < 2; i++)
			draw_char(SCREEN_SIZE_X-54+(i*8),y_pos,_hours[i],COLOR_BLACK);
	}
	i = 0;
	//print :
	draw_char(SCREEN_SIZE_X-38,y_pos,':',COLOR_BLACK);
	//print mins
	if(strlen(_mins) == 1)
	{
		draw_char(SCREEN_SIZE_X-28+(0*8),y_pos,'0',COLOR_BLACK);
		draw_char(SCREEN_SIZE_X-28+(1*8),y_pos,_mins[0],COLOR_BLACK);
	}
	else
	{
		for (i = 0; i < 2; i++)
			draw_char(SCREEN_SIZE_X-28+(i*8),y_pos,_mins[i],COLOR_BLACK);
	}		
}

void gfx_draw_menubar()
{
	//draw menu bar
	for(int i=0;i<SCREEN_SIZE_X;i++)
	{
		for(int j=menubar_start;j<menubar_end;j++)
		{
			putPixel_32bpp(i, j, menubar_color);
		}
	}

	char text[4] = "Menu";
	for (int i = 0; i < 4; i++)
		draw_char(10+(i*8),menubar_start+10,text[i],COLOR_BLACK);

	gfx_draw_menu_time();
}

void refresh()
{
	for(;;)
	{
		sleep(10);
		//vsync();
		gfx_draw_workspace();
		gfx_draw_menubar();
		draw_windows();
		print_mouse();
		g_flip();
	}
}

void init_gfx()
{
	double_buffer  = (long *)kmalloc(0x300000);
	memset((long *)double_buffer, 0x00000000,  0x300000);
	gfx_draw_workspace();
	gfx_draw_menubar();
	draw_windows();
}
