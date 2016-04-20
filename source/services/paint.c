/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	PAINT
*/
#include <gui/window.h>
#include <gui/gfx.h>
#include <arch/task.h>

extern volatile window_t window_list[];
extern volatile window_t current_window;
extern volatile char mouse_lbtn;
extern volatile int mx, my;
extern volatile int hx, hy; 
int paint_id;
int brush_size = 2;
int brush_shape = 1;
volatile unsigned brush_color = COLOR_BLACK;

void paint()
{
	while(window_list[paint_id].id != 0)
	{
		while(window_list[paint_id].id != 0 && mouse_lbtn && current_window.id == paint_id)
		{	
			sleep(1);
			if(mx >  window_list[paint_id].x && mx < window_list[paint_id].x + window_list[paint_id].width)
			{
				if(my >  window_list[paint_id].y + 45 && my < window_list[paint_id].y + 45 + window_list[paint_id].height)	
				{
					if(brush_shape == 1 && window_list[paint_id]._items < 998)
						window_add_graphic(mx-window_list[paint_id].x, my-window_list[paint_id].y, brush_size, brush_size, brush_color, paint_id, false, false);
					else if(brush_shape == 2 && window_list[paint_id]._items < 994)
					{
						window_add_graphic(mx-window_list[paint_id].x, my-window_list[paint_id].y, brush_size, brush_size, brush_color, paint_id, false, false);
						window_add_graphic(mx-window_list[paint_id].x+1, my-window_list[paint_id].y, brush_size, brush_size, brush_color, paint_id, false, false);
						window_add_graphic(mx-window_list[paint_id].x, my-window_list[paint_id].y-1, brush_size, brush_size, brush_color, paint_id, false, false);
						window_add_graphic(mx-window_list[paint_id].x, my-window_list[paint_id].y+1, brush_size, brush_size, brush_color, paint_id, false, false);
						window_add_graphic(mx-window_list[paint_id].x-1, my-window_list[paint_id].y, brush_size, brush_size, brush_color, paint_id, false, false);
					}
					break;
				}
			}
		}
	}
	exit();
}

void change_brush_size(int s)
{
	switch(s)
	{
		case 1:
			if(brush_size > 2)
				brush_size-=1;
			break;
		case 2:
			if(brush_size <= 5)
				brush_size+=1;
			break;			
	}
}

void change_brush_color(int c)
{
	switch(c)
	{
		case 1:
				brush_color = COLOR_BLACK;
				break;
		case 2:
				brush_color = COLOR_RED;
				break;	
		case 3:
				brush_color = COLOR_GREEN;
				break;	
		case 4:
				brush_color = COLOR_BLUE;
				break;		
		case 5:
				brush_color = COLOR_YELLOW;
				break;	
		case 6:
				brush_color = COLOR_PURPLE;
				break;		
	}
}

void change_brush_shape(int sh)
{
	switch(sh)
	{
		case 1:
				brush_shape = 1;
			break;
		case 2:
				brush_shape = 2;
			break;			
	}
}

void init_paint()
{
	paint_id = create_window(450, 450, "Paint",  COLOR_BACKGROUND, COLOR_BLACK, 1);
        window_add_item(2, 20, 100, 20, COLOR_BACKGROUND, COLOR_MENUBAR, "Brush size", paint_id, 0, 0); 
        window_add_item(100, 20, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "<", paint_id, change_brush_size, 1); 
        window_add_item(125, 20, 20, 20, COLOR_WHITE, COLOR_MENUBAR, ">", paint_id, change_brush_size, 2); 
        window_add_item(150, 20, 50, 20, COLOR_BACKGROUND, COLOR_MENUBAR, "Color", paint_id, 0,0); 
        window_add_item(200, 20, 20, 20, COLOR_BLACK, COLOR_MENUBAR, "", paint_id, change_brush_color, 1); 
        window_add_item(225, 20, 20, 20, COLOR_RED, COLOR_MENUBAR, "", paint_id, change_brush_color, 2); 
        window_add_item(250, 20, 20, 20, COLOR_GREEN, COLOR_MENUBAR, "", paint_id, change_brush_color, 3); 
        window_add_item(275, 20, 20, 20, COLOR_BLUE, COLOR_MENUBAR, "", paint_id, change_brush_color, 4); 
        window_add_item(300, 20, 20, 20, COLOR_YELLOW, COLOR_MENUBAR, "", paint_id, change_brush_color, 5); 
        window_add_item(325, 20, 20, 20, COLOR_PURPLE, COLOR_MENUBAR, "", paint_id, change_brush_color, 6); 
        window_add_item(350, 20, 50, 20, COLOR_BACKGROUND, COLOR_MENUBAR, "Shape", paint_id, 0, 0);  
        window_add_item(400, 20, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "o", paint_id, change_brush_shape, 1);  
        window_add_item(425, 20, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "*", paint_id, change_brush_shape, 2);  
        window_add_graphic(0, 45, 450, 2, COLOR_BLACK, paint_id, true, false); 
	start_task(paint, 0, deadline_MEDIUM, resources_MEDIUM);
}
