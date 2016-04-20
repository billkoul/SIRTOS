/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	WINDOW MANAGER
*/
#include <arch/task.h>
#include <gui/window.h>
#include <gui/gfx.h>

#define MAX_WINDOWS 100
volatile window_t window_list[MAX_WINDOWS];
volatile info_window_t info_window_list[100];
volatile window_t current_window;

extern void init_settings();
volatile window_item_t item_list[1000];
volatile window_graphic_t graphic_list[1000];
extern volatile int mx, my;
extern volatile int mouse_x;
extern volatile int mouse_y;
extern volatile char mouse_lbtn;
int drawn = 0;
volatile int id = 0;
volatile int item_id = 0;
volatile int graphic_id = 0;
volatile int info_id = 0;
extern volatile int hx, hy; 
extern void hook();
extern volatile int menubar_start;
//functions
extern void init_file_manager();
extern void init_control_panel();
extern void init_paint();
extern void init_calculator();
extern void init_mem_browser();
extern void init_login();
extern void reboot();
extern void shutdown();
extern void init_g_shell();
void destroy_window(int id);
volatile int prev_win_wh[MAX_WINDOWS][5];
volatile int empty_id[MAX_WINDOWS];
extern bool MOUSE_LEFT_DOWN;
extern bool MOUSE_GRAB;
extern int window_GRABBED;
extern int GRAB_OFFSET_X;
extern int GRAB_OFFSET_Y;

int get_nextid()
{
	id++;
	return id;
}

int get_infoid()
{
	info_id++;
	return info_id;
}


int get_itemid()
{
	item_id++;
	return item_id;
}

int get_graphicid()
{
	graphic_id++;
	return graphic_id;
}

void draw_window(int n)
{
	int mouse_to_x;
	int mouse_to_y;

	if(MOUSE_LEFT_DOWN && MOUSE_GRAB && window_GRABBED == n && !window_list[n].maximized)
	{
		if(mx - GRAB_OFFSET_X >= 0 && mx - GRAB_OFFSET_X + window_list[n].width <= SCREEN_SIZE_X)
			window_list[n].x = mx - GRAB_OFFSET_X;
		if(my - GRAB_OFFSET_Y >= 0 && my - GRAB_OFFSET_Y + window_list[n].height <= SCREEN_SIZE_Y)
			window_list[n].y = my - GRAB_OFFSET_Y;
	}	
	int xi = window_list[n].x;

	if(window_list[n].maximized)
	{
		if(menubar_start == 0)
			window_list[n].y = 30;	
		else
			window_list[n].y = 0;	
	}
	int yi = window_list[n].y;

	int width = window_list[n].width;
	int height = window_list[n].height;
	char *title = window_list[n].title;

	unsigned WINDOWBAR_COLOR = COLOR_WINDOWBAR;
	if(current_window.id == n)
		WINDOWBAR_COLOR = 0x004B4B55;

	if(window_list[n].id > 2)
	{
		//topbar
		for(int y = yi; y < yi+15; y++)
		{
			for(int x = xi; x< xi+width; x++)
			{
				if(y == yi || x == xi || x == xi+width-1)
					putPixel_32bpp(x, y, COLOR_BLACK);
				else	
					putPixel_32bpp(x, y, WINDOWBAR_COLOR);
			}
		}

		//background
		for(int y = yi+15; y < yi+height; y++)
		{
			for(int x = xi; x< xi+width; x++)
			{
				if(y == yi+15)
					putPixel_32bpp(x, y, COLOR_BLACK);
				else if(x == xi || x == xi+width-1 || y == yi+height-1)
					putPixel_32bpp(x, y, COLOR_BLACK);
				else
					putPixel_32bpp(x, y, window_list[n].background);
			}
		}
	}
	else
	{
		if(n == 1) //menubar
		{
			if(menubar_start != 0)
				yi = menubar_start - 150;
			else
				yi = 30;
			
			for(int y = yi; y < yi+height; y++)
			{
				for(int x = xi; x< xi+width; x++)
				{
					if(y == yi)
						putPixel_32bpp(x, y, COLOR_BLACK);
					else if(x == xi || x == xi+width-1 || y == yi+height-1)
						putPixel_32bpp(x, y, COLOR_BLACK);
					else
						putPixel_32bpp(x, y, window_list[n].background);
				}
			}
		}
		else if(n == 2) //right click menu
		{
			window_list[n].x = hx;
			window_list[n].y = hy;
			for(int y = hy; y < hy+height; y++)
			{
				for(int x = hx; x< hx+width; x++)
				{
					if(y == hy)
						putPixel_32bpp(x, y, COLOR_BLACK);
					else if(x == hx || x == hx+width-1 || y == hy+height-1)
						putPixel_32bpp(x, y, COLOR_BLACK);
					else
						putPixel_32bpp(x, y, window_list[n].background);
				}
			}
		}
	}
	
	if(window_list[n].id > 2)
	{
		int text_size = strlen(title);
		int middle = floor((width/2)-(text_size/2));
		for(int i=0; i< text_size; i++)
			draw_char(xi+middle+2+i*8, yi+3, title[i], COLOR_WHITE);
	}

	if(window_list[n].id > 2)
	{
		//menubar
		int text_size = strlen(title);
		for(int j=0; j< text_size; j++)
			draw_char(50+drawn+(j*8), menubar_start+10, title[j], COLOR_BLACK);
		drawn += (text_size * 8) + 8;
	}


	//buffer
	if(window_list[n].type == 2)
	{
		int b_length = window_list[n].buffer_counter;
		for(int i =0; i<b_length; i++)
			draw_char(xi+(8*i), yi+20, window_list[n].buffer[i], window_list[n].forecolor);
	}

	//window items
	for(int item_n = 0; item_n < window_list[n]._items; item_n++)
	{
		if(n == 2)
		{
			xi = hx;
			yi = hy;
		}

		int xi2 = xi+ window_list[n].item[item_n].x;
		int yi2 = yi + window_list[n].item[item_n].y;
		int width2 =  window_list[n].item[item_n].width;
		int height2 =  window_list[n].item[item_n].height;

		//background
		for(int y = yi2; y < yi2+height2; y++)
		{
			for(int x = xi2; x< xi2+width2; x++)
			{
				putPixel_32bpp(x, y, window_list[n].item[item_n].background);
			}
		}
	
		int text_size2 = strlen(window_list[n].item[item_n].name);
		for(int j=0; j< text_size2; j++)
			draw_char(xi2+j*8+1, yi2+3, window_list[n].item[item_n].name[j], COLOR_BLACK);
	}

	//window graphics
	for(int graphic_n = 0; graphic_n < window_list[n]._graphics; graphic_n++)
	{
		//here we need re-organization of items in windows when we switch from maximized to minimized and vice versa
		//lets hack it for now
		int maxi = window_list[n].width;
		int wherex = window_list[n].graphic[graphic_n].x;
		int xi2;

		if(maxi <  wherex)
		{
			while(maxi <=  wherex + window_list[n].graphic[graphic_n].width)
			{
				wherex -= 1;
			}
		}
		xi2 = xi + wherex;
		int yi2;

		int mayi = window_list[n].height;
		int wherey =  window_list[n].graphic[graphic_n].y;
		if(mayi <  wherey)
		{
			while(maxi <=  wherey + window_list[n].graphic[graphic_n].height)
			{
				wherey -= 1;
			}
		}	
		yi2 = yi + wherey;
			
		int width2;
		int height2;
		//major hack
		if(window_list[n].graphic[graphic_n].fixed_x)
			width2 = window_list[n].width;
		else
			width2 =  window_list[n].graphic[graphic_n].width;
		if(window_list[n].graphic[graphic_n].fixed_y)
			height2 = window_list[n].height;
		else
			height2 =  window_list[n].graphic[graphic_n].height;

		//draw graphic
		for(int y = yi2; y < yi2+height2; y++)
		{
			for(int x = xi2; x< xi2+width2; x++)
			{
				putPixel_32bpp(x, y, window_list[n].graphic[graphic_n].color);
			}
		}	
	}
}

void draw_info_window(int n)
{
	int xi = info_window_list[n].x;
	int yi = info_window_list[n].y;
	int width =  info_window_list[n].width;
	int height =  info_window_list[n].height;

	//topbar
	for(int y = yi; y < yi+15; y++)
	{
		for(int x = xi; x< xi+width; x++)
		{
			if(y == yi || x == xi || x == xi+width-1)
				putPixel_32bpp(x, y, COLOR_BLACK);
			else	
				putPixel_32bpp(x, y, COLOR_WINDOWBAR);
		}
	}

	//background
	for(int y = yi+15; y < yi+height; y++)
	{
		for(int x = xi; x< xi+width; x++)
		{
			if(y == yi+15)
				putPixel_32bpp(x, y, COLOR_BLACK);
			else if(x == xi || x == xi+width-1 || y == yi+height-1)
				putPixel_32bpp(x, y, COLOR_BLACK);
			else
				putPixel_32bpp(x, y, info_window_list[n].background);
		}
	}
}

void draw_windows()
{
	drawn = 0;
	int j = 0;
	for(int i = 1; i<=id; i++)
	{
		if(empty_id[i] != i && window_list[i].id != 0 && window_list[i].visible != false)
		{	
			if(current_window.id != window_list[i].id)
				draw_window(i);
			else
				j = i;
		}
	}
	if(j!= 0)
		draw_window(j); //draw current window last

	for(int i = 1; i<=info_id; i++)
	{
		if(info_window_list[i].id != 0)
		{	
			draw_info_window(i);
		}
	}
}

void set_window(int n)
{
	current_window = window_list[n];
	current_window.id = window_list[n].id;
}

void create_toolbar(int x, int y, int width, int height, char *title, unsigned background, unsigned forecolor)
{
	int n = get_nextid();
	cli();
	window_list[n].id = n;
	window_list[n].x = x;
	window_list[n].y = y;
	window_list[n].z = 1;
	window_list[n].width = width;
	window_list[n].height = height;
	window_list[n].title = title;
   	window_list[n].background = background;
	window_list[n].forecolor = forecolor;
	window_list[n].visible = false;
	window_list[n]._items = 0;
	sti();
}

void maximize_window(int n)
{
	if(window_list[n].maximized)
		return;
	cli();
	window_list[n].maximized = true;

	prev_win_wh[n][0] = window_list[n].width;
	prev_win_wh[n][1] = window_list[n].height;
	prev_win_wh[n][2] = window_list[n].item[0].x;
	prev_win_wh[n][3] =window_list[n].item[1].x;
	prev_win_wh[n][4] = window_list[n].item[2].x;

	window_list[n].item[0].x = SCREEN_SIZE_X - (window_list[n].width - window_list[n].item[0].x);
	window_list[n].item[1].x = SCREEN_SIZE_X - (window_list[n].width - window_list[n].item[1].x);
	window_list[n].item[2].x = SCREEN_SIZE_X - (window_list[n].width - window_list[n].item[2].x);

	int iid1 = window_list[n].item[0].id;
	int iid2 = window_list[n].item[1].id;
	int iid3 = window_list[n].item[2].id;

	item_list[iid1].x = SCREEN_SIZE_X - (window_list[n].width - prev_win_wh[n][2]);
	item_list[iid2].x = SCREEN_SIZE_X - (window_list[n].width - prev_win_wh[n][3]);
	item_list[iid3].x = SCREEN_SIZE_X - (window_list[n].width - prev_win_wh[n][4]);

	window_list[n].x = 0;

	if(menubar_start == 0)
		window_list[n].y = 30;	
	else
		window_list[n].y = 0;	

	window_list[n].width = SCREEN_SIZE_X;
	window_list[n].height = SCREEN_SIZE_Y-30;
	sti();
}

void minimize_window(int n)
{
	if(!window_list[n].maximized)
		return;

	cli();

	window_list[n].width = prev_win_wh[n][0];
	window_list[n].height = prev_win_wh[n][1];

	window_list[n].x = SCREEN_SIZE_X / 2 - prev_win_wh[n][0]/2;	
	window_list[n].y = SCREEN_SIZE_Y / 2 - prev_win_wh[n][1]/2;

	window_list[n].item[0].x = prev_win_wh[n][2];
	window_list[n].item[1].x = prev_win_wh[n][3];
	window_list[n].item[2].x = prev_win_wh[n][4];

	int iid1 = window_list[n].item[0].id;
	int iid2 = window_list[n].item[1].id;
	int iid3 = window_list[n].item[2].id;

	item_list[iid1].x = prev_win_wh[n][2];
	item_list[iid2].x = prev_win_wh[n][3];
	item_list[iid3].x = prev_win_wh[n][4];

	window_list[n].maximized = false;
	sti();
}

int create_window(int width, int height, char *title, unsigned background, unsigned forecolor, int type)
{
	if(id >= MAX_WINDOWS)
		return;

	int w = width;
	int h = height;
	if(w > 1000)
		w = 1000;
	if(h > 700)
		h = 700;

	int n = get_nextid();
	cli();
	window_list[n].id = n;
	window_list[n].x = SCREEN_SIZE_X / 2 - w/2;
	window_list[n].y = SCREEN_SIZE_Y / 2 - h/2;
	window_list[n].z = 1;
	window_list[n].width = w;
	window_list[n].height = h;
	window_list[n].title = title;
   	window_list[n].background = background;
	window_list[n].forecolor = forecolor;
  	window_list[n].visible = true;
	window_list[n].type = type;
	window_list[n].buffer_counter = 0;
	window_list[n]._items = 0;
	window_list[n]._graphics = 0;
	window_list[n].maximized = false;
	window_add_item(width-13, 1, 12, 12, COLOR_WHITE, COLOR_MENUBAR, "X", n, destroy_window, n);
	window_add_item(width-28, 1, 12, 12, COLOR_WHITE, COLOR_MENUBAR, "+", n, maximize_window, n);
	window_add_item(width-43, 1, 12, 12, COLOR_WHITE, COLOR_MENUBAR, "-", n, minimize_window, n);
     	set_window(n);
	sti();
	return n;
}

int create_info_window(int width, int height, char *title, char *info, unsigned background, unsigned forecolor, int type)
{
	if(info_id >= 100)
		return;

	int w = width;
	int h = height;
	if(w > 1000)
		w = 1000;
	if(h > 700)
		h = 700;

	int n = get_infoid();
	cli();
	info_window_list[n].id = n;
	info_window_list[n].x = SCREEN_SIZE_X / 2 - w/2;
	info_window_list[n].y = SCREEN_SIZE_Y / 2 - h/2;
	info_window_list[n].z = 1;
	info_window_list[n].width = w;
	info_window_list[n].height = h;
	info_window_list[n].title = title;
	info_window_list[n].info = info;
   	info_window_list[n].background = background;
	info_window_list[n].forecolor = forecolor;
	info_window_list[n].type = type;
	sti();
	return n;
}


int window_add_graphic(int x, int y, int width, int height, unsigned color, int parent_id, bool fixed_x, bool fixed_y)
{
	int gid = get_graphicid();
	graphic_list[gid].parent_id = parent_id;
	graphic_list[gid].id = gid;
	graphic_list[gid].x = x;
	graphic_list[gid].y = y;
	graphic_list[gid].width = width;
	graphic_list[gid].height = height;
	graphic_list[gid].color = color;
	graphic_list[gid].fixed_x = fixed_x;
	graphic_list[gid].fixed_y = fixed_y;
	int number_of_graphics = window_list[parent_id]._graphics;
	window_list[parent_id].graphic[number_of_graphics] = graphic_list[gid];
	window_list[parent_id]._graphics++;
	return gid;
}

int window_add_item(int x, int y, int width, int height, unsigned bcolor, unsigned fcolor, char *name, int parent_id, void (*func)(int arg), int arg)
{
	int id = get_itemid(); // just a counter to know how many items we have
	cli();
	item_list[id].id = id;
	item_list[id].parent_id = parent_id;
	item_list[id].x = x;
	item_list[id].y = y;
	item_list[id].width = width;
	item_list[id].height = height;
	item_list[id].name = name;
	item_list[id].background = bcolor;
	item_list[id].forecolor = fcolor;
	item_list[id].func = func;
	item_list[id].arg = arg;
	int number_of_items = window_list[parent_id]._items;
	window_list[parent_id].item[number_of_items] = item_list[id];
	window_list[parent_id]._items++;
	sti();
	return id;
}

void destroy_window(int n)
{
	cli();
	empty_id[n] = n;
	window_list[n].visible = false;
	if(current_window.id == window_list[n].id)
	{
		for(int i = 1; i <= id; i++)
		{
			if(window_list[i].visible)
				current_window = window_list[i];
		}
	}
	sti();
}

void destroy_info_window(int n)
{
	cli();
	info_window_list[n].id = 0;
	sti();
}

void destroy_current_window()
{
	cli();
	empty_id[current_window.id] = id;

	current_window.id = 0;
	current_window.x = 0;
	current_window.y = 0;
	current_window.z = 0;
	current_window.width = 0;
	current_window.height = 0;
   	current_window.background = 0;
	current_window.forecolor = 0;
	current_window.visible = 0;
	current_window._items = 0;
	sti();
}

void init_window_manager()
{
	cli();
	//create menubar
	create_toolbar(1, 30, 110, 150, "menubar", COLOR_MENUBAR, COLOR_WHITE);
	window_add_item(2, 1, 106, 14, COLOR_WHITE, COLOR_MENUBAR, "Shell", 1, init_g_shell, 0);
	window_add_item(2, 16, 106, 14, COLOR_WHITE, COLOR_MENUBAR, "Files", 1, init_file_manager, 0);
	window_add_item(2, 31, 106, 14, COLOR_WHITE, COLOR_MENUBAR, "Calculator", 1, init_calculator, 0);
	window_add_item(2, 46, 106, 14, COLOR_WHITE, COLOR_MENUBAR, "Paint", 1, init_paint, 0);
	window_add_item(2, 61, 106, 14, COLOR_WHITE, COLOR_MENUBAR, "Mem Browser", 1, init_mem_browser, 0);
	window_add_item(2, 76, 106, 14, COLOR_WHITE, COLOR_MENUBAR, "Control Panel", 1, init_control_panel, 0);
	window_add_item(2, 91, 106, 14, COLOR_WHITE, COLOR_MENUBAR, "Login", 1, init_login, 0);
	window_add_item(2, 106, 106, 14, COLOR_WHITE, COLOR_MENUBAR, "Reboot", 1, reboot, 0);
	window_add_item(2, 121, 106, 14, COLOR_WHITE, COLOR_MENUBAR, "Shutdown", 1, shutdown, 0);
	//right click options
	create_toolbar(30, 30, 72, 80, "optionsbar", COLOR_MENUBAR, COLOR_WHITE);
	window_add_item(1, 1, 70, 12, COLOR_WHITE, COLOR_MENUBAR, "Copy", 2, 0, 0);
	window_add_item(1, 14, 70, 12, COLOR_WHITE, COLOR_MENUBAR, "Paste", 2, 0, 0);
	window_add_item(1, 27, 70, 12, COLOR_WHITE, COLOR_MENUBAR, "Settings", 2, init_settings, 0);
	sti();
}
