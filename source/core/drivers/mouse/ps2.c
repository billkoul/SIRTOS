/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	PS2 MOUSE DRIVER
*/
#include <system.h>
#include <gui/gfx.h>
#include <gui/bmp.h>
#include <gui/window.h>

volatile unsigned char mouse_cycle=0;
volatile char mouse_byte[3];
volatile int mouse_x=10;
volatile int mouse_y=10;
volatile char mouse_lbtn, mouse_rbtn;
//Static functions prototypes
static void mouse_wait(unsigned char a_type);
static unsigned char mouse_read();
static void mouse_write(unsigned char a_write);
extern bool graphical_mode;
extern unsigned long g_cursor_x,g_cursor_y,g_bpp;
//These are the ACTUAL positions
volatile int mx, my;
//toolbar
extern volatile window_t window_list[];
extern volatile window_t current_window;
extern volatile window_item_t item_list[];
extern volatile int item_id;
extern volatile int id;
extern volatile int menubar_start;
//hold x,y
volatile int hx, hy = 0;
bool MOUSE_LEFT_DOWN = false;
bool MOUSE_GRAB = false;
int window_GRABBED = 0;
int GRAB_OFFSET_X;
int GRAB_OFFSET_Y;

void print_mouse()
{
	putPixel_32bpp(mx, my, COLOR_BLACK);
	int j = 0;
	for(int i=1; i<10; i++)
	{
		putPixel_32bpp(mx, my+i, COLOR_BLACK);
		if(i < 8)
			putPixel_32bpp(mx+i, my+i, COLOR_BLACK);
	}
	for(int i=0; i<8; i++)
	{
		putPixel_32bpp(mx+i, my+9-j, COLOR_BLACK);
		if(j<2)
			j++;
	}
	
	putPixel_32bpp(mx+3, my+7, COLOR_BLACK);
	putPixel_32bpp(mx+4, my+7, COLOR_BLACK);

	putPixel_32bpp(mx+3, my+8, COLOR_BLACK);
	putPixel_32bpp(mx+4, my+8, COLOR_BLACK);

	putPixel_32bpp(mx+4, my+9, COLOR_BLACK);
	putPixel_32bpp(mx+5, my+9, COLOR_BLACK);

	putPixel_32bpp(mx+4, my+10, COLOR_BLACK);
	putPixel_32bpp(mx+5, my+10, COLOR_BLACK);
}

void handle_click()
{
	if(mouse_lbtn)
	{
		MOUSE_LEFT_DOWN = true;
		if(mx > 2 && mx < 50) // menu click
		{
			if(my > menubar_start && my < menubar_start+30)
				window_list[1].visible = true;
				set_window(1);
		}
		else
		{
			//window grab
			for(int i = 3; i<=id; i++)
			{	
				if(window_list[i].visible == true && !MOUSE_GRAB && mx >window_list[i].x && mx < window_list[i].x + window_list[i].width)
				{
					if(my >window_list[i].y && my < window_list[i].y + 15)
					{
						current_window = window_list[i];
						MOUSE_GRAB = true;
						window_GRABBED = i;
						GRAB_OFFSET_X = mx - window_list[i].x;
						GRAB_OFFSET_Y = my - window_list[i].y;
						break;
					}
				}
			}
			//menubar items click
			for(int i = 1; i<=item_id; i++)
			{	
				if(window_list[1].visible) //if menu toolbar visible
				{
					if(mx > 1 && mx < 110) //all at the same x
					{
						int where;
						if(menubar_start != 0)
							where = menubar_start-150;
						else
							where = menubar_start+30;
						if(my > where + item_list[i].y && my < where + item_list[i].y+item_list[i].height) //what changes is the y
						{
								item_list[i].func(item_list[i].arg);
								break;
						}
					}
				}	
				//window items click
				else if(window_list[item_list[i].parent_id].visible == true && mx >window_list[item_list[i].parent_id].x + item_list[i].x && mx < window_list[item_list[i].parent_id].x + item_list[i].x+item_list[i].width)
				{
					if(my > window_list[item_list[i].parent_id].y+item_list[i].y && my < window_list[item_list[i].parent_id].y+item_list[i].y+item_list[i].height)
					{
							item_list[i].func(item_list[i].arg);
							break;
					}
				}			
			}
			window_list[1].visible = false;
		}
		window_list[2].visible = false;
	}
	else if(mouse_rbtn) // right click menu
	{
		hx = mx;
		hy = my;
		window_list[2].visible = true;
		set_window(2);
		MOUSE_LEFT_DOWN = false;
		MOUSE_GRAB = false;
		window_GRABBED = 0;
	}
	else
	{
		MOUSE_LEFT_DOWN = false;
		MOUSE_GRAB = false;
		window_GRABBED = 0;
	}
}

void mouse_handler()
{
	if(!graphical_mode)
       		return;
  	switch(mouse_cycle)
  	{
    		case 0:
      		mouse_byte[0]=inb(0x60);
      		mouse_cycle++;
   
      		mouse_lbtn = mouse_byte[0] & 0x1; //bit 0
      		mouse_rbtn = mouse_byte[0] & 0x2; //bit 1
		//mouse_mbtn = mouse_byte[0] & 0x4; //bit 1
      		break;

    		case 1:
      		mouse_byte[1]=inb(0x60);
      		mouse_cycle++;
      
      		mouse_x=mouse_byte[1];
      		mx += mouse_x;
      		if (mx >= SCREEN_SIZE_X-8) 
			mx = SCREEN_SIZE_X-8;
      		if (mx <= 1) 
			mx = 1;
      
      		break;

    		case 2:
      		mouse_byte[2]=inb(0x60);
      		mouse_cycle=0;
      
      		mouse_y=mouse_byte[2];
      		my -= mouse_y;
      		if (my >= SCREEN_SIZE_Y-8) 
			my = SCREEN_SIZE_Y-8;
      		if (my <= 1) 
			my = 1;
      
      		break;
  	}

	handle_click();
  	print_mouse();
}


static void mouse_wait(unsigned char a_type)
{
  	unsigned int _time_out=100000;
  	if(a_type==0)
  	{
    		while(_time_out--) //Data
    		{
      			if((inb(0x64) & 1)==1)
        			return;
    		}
    		return;
  	}
  	else
  	{
    		while(_time_out--) //Signal
    		{
      		if((inb(0x64) & 2)==0)
        		return;
    		}
    		return;
  	}
}

static void mouse_write(unsigned char a_write)
{
  	//Wait to be able to send a command
  	mouse_wait(1);
  	//Tell the mouse we are sending a command
  	outb(0x64, 0xD4);
  	//Wait for the final part
 	mouse_wait(1);
  	//Finally write
  	outb(0x60, a_write);
}

static unsigned char mouse_read()
{
  	//Get's response from mouse
  	mouse_wait(0);
  	return inb(0x60);
}

void init_ps2_mouse()
{
  	unsigned char _status;
  
  	mx = (int)SCREEN_SIZE_X/2;
  	my = (int)SCREEN_SIZE_Y/2;
 
  	//Enable the auxiliary mouse device
  	mouse_wait(1);
  	outb(0x64, 0xA8);
 
  	//Enable the interrupts
  	mouse_wait(1);
  	outb(0x64, 0x20);
  	mouse_wait(0);
  	_status=(inb(0x60) | 2);
  	mouse_wait(1);
  	outb(0x64, 0x60);
  	mouse_wait(1);
  	outb(0x60, _status);
 
  	//Tell the mouse to use default settings
  	mouse_write(0xF6);
  	mouse_read();  //Acknowledge
 
  	//Enable the mouse
  	mouse_write(0xF4);
  	mouse_read();  //Acknowledge

  	//Setup the mouse handler
  	install_interrupt_handler(IRQ12, &mouse_handler);
} 

int detect_ps2_mouse()
{
    	unsigned char tmp = mouse_read();
    	if(tmp != 0xFA)
         	return 0; //No mouse
    	else
         	return 1; //Mouse there
}
