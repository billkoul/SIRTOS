/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	BROWSER
*/
#include <gui/window.h>
#include <gui/gfx.h>

unsigned COLOR_1 = 0X009999FF;
unsigned COLOR_2 = 0X0066CCFF;
unsigned COLOR_3 = 0X0099FF99;
unsigned COLOR_4 = 0X00003366;
unsigned COLOR_5 = 0x0064A5A5;
unsigned COLOR_6 = 0X00FFFFFF;
unsigned COLOR_7 = 0X00669900;
unsigned COLOR_8 = 0x004C6161;
unsigned COLOR_9 = 0x0000CC99;

unsigned volatile workspace_color = COLOR_WORKSPACE;
unsigned volatile menubar_color = COLOR_MENUBAR;
volatile int menubar_start = 0;
volatile int menubar_end = 30;
extern volatile window_t current_window;
int sett_id;
extern volatile int time_hour;

void set_workspace_color(unsigned  color)
{
	if(current_window.id == sett_id)
		workspace_color = color;
}

void set_menubar_color(unsigned  color)
{
	if(current_window.id == sett_id)		
		menubar_color = color;
}

void set_time(int  t)
{
	switch(t)
	{
		case 12:
			time_hour = 12;
			break;
		case 24:
			time_hour = 24;
			break;
	}
}


void set_menubar_start(int  start)
{
	if(current_window.id == sett_id)
	{
		if(start == 1)
		{
			menubar_start = 0;
			menubar_end = 30;
		}
		else if(start == 2)
		{
			menubar_start = SCREEN_SIZE_Y-30;
			menubar_end = SCREEN_SIZE_Y;
		}
	}
}

void init_settings()
{
	sett_id = create_window(350, 300, "Settings",  COLOR_BACKGROUND, COLOR_BLACK, 1);
	window_add_item(5, 20, 200, 20, COLOR_BACKGROUND, COLOR_MENUBAR, "Workspace color", sett_id, 0, 0);
	window_add_item(5, 45, 60, 20, COLOR_1, COLOR_MENUBAR, "9999FF", sett_id, set_workspace_color, COLOR_1);
	window_add_item(70, 45, 60, 20, COLOR_2, COLOR_MENUBAR, "66CCFF", sett_id, set_workspace_color, COLOR_2);
	window_add_item(135, 45, 60, 20, COLOR_3, COLOR_MENUBAR, "99FF99", sett_id, set_workspace_color, COLOR_3);
	window_add_item(200, 45, 60, 20, COLOR_4, COLOR_MENUBAR, "003366", sett_id, set_workspace_color, COLOR_4);
	window_add_item(265, 45, 60, 20, COLOR_5, COLOR_MENUBAR, "64A5A5", sett_id, set_workspace_color, COLOR_5);
	window_add_item(5, 70, 60, 20, COLOR_6, COLOR_MENUBAR, "FFFFFF", sett_id, set_workspace_color, COLOR_6);
	window_add_item(70, 70, 60, 20, COLOR_7, COLOR_MENUBAR, "669900", sett_id, set_workspace_color, COLOR_7);
	window_add_item(135, 70, 60, 20, COLOR_8, COLOR_MENUBAR, "4C6161", sett_id, set_workspace_color, COLOR_8);
	window_add_item(200, 70, 60, 20, COLOR_9, COLOR_MENUBAR, "00CC99", sett_id, set_workspace_color, COLOR_9);
	window_add_item(5, 95, 200, 20, COLOR_BACKGROUND, COLOR_MENUBAR, "Menu bar color", sett_id, 0, 0);
	window_add_item(5, 120, 60, 20, COLOR_1, COLOR_MENUBAR, "9999FF", sett_id, set_menubar_color, COLOR_1);
	window_add_item(70, 120, 60, 20, COLOR_2, COLOR_MENUBAR, "66CCFF", sett_id, set_menubar_color, COLOR_2);
	window_add_item(135, 120, 60, 20, COLOR_3, COLOR_MENUBAR, "99FF99", sett_id, set_menubar_color, COLOR_3);
	window_add_item(200, 120, 60, 20, COLOR_4, COLOR_MENUBAR, "003366", sett_id, set_menubar_color, COLOR_4);
	window_add_item(265, 120, 60, 20, COLOR_5, COLOR_MENUBAR, "64A5A5", sett_id, set_menubar_color, COLOR_5);
	window_add_item(5, 145, 60, 20, COLOR_6, COLOR_MENUBAR, "FFFFFF", sett_id, set_menubar_color, COLOR_6);
	window_add_item(70, 145, 60, 20, COLOR_7, COLOR_MENUBAR, "669900", sett_id, set_menubar_color, COLOR_7);
	window_add_item(135, 145, 60, 20, COLOR_8, COLOR_MENUBAR, "4C6161", sett_id, set_menubar_color, COLOR_8);
	window_add_item(200, 145, 60, 20, COLOR_9, COLOR_MENUBAR, "00CC99", sett_id, set_menubar_color, COLOR_9);
	window_add_item(5, 170, 220, 20, COLOR_BACKGROUND, COLOR_MENUBAR, "Menu bar position", sett_id, 0, 0);
	window_add_item(5, 195, 20, 15, COLOR_WHITE, COLOR_MENUBAR, "Up", sett_id, set_menubar_start, 1);
	window_add_item(30, 195, 40, 15, COLOR_WHITE, COLOR_MENUBAR, "Down", sett_id, set_menubar_start, 2);
	window_add_item(5, 220, 80, 20, COLOR_BACKGROUND, COLOR_MENUBAR, "Time", sett_id, 0, 0);
	window_add_item(5, 245, 80, 20, COLOR_WHITE, COLOR_MENUBAR, "12-hour", sett_id, set_time, 12);
	window_add_item(90, 245, 80, 20, COLOR_WHITE, COLOR_MENUBAR, "24-hour", sett_id, set_time, 24);
}
