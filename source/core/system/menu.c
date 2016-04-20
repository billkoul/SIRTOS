/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

YOU MAY USE THIS CODE FOR PERSONAL PURPOSES ONLY.

	SIMPLE VGA MENU
*/
#include <gui/screen.h>
#include <lib/types.h>
#include <drivers/keyb.h>
#include <arch/task.h>

extern void init_shell();

void show_menu()
{
	cls();
	//draw_keys = false for this to work
	drawDesktop(177, VGA_COLOR_BLACK, VGA_COLOR_WHITE);
	drawWindow(" ShortRTOS v1.01 ", 2, 2, 75, 20, VGA_COLOR_LTWHITE, VGA_COLOR_BLUE);
	putsnocr("Menu");
	putsnocr("1. Console Mode");
	putsnocr("2. Graphic Mode");
	putsnocr("3. Shutdown");
	putsnocr("4. Reboot");
	hideCursor();
	getKey_menu();
	exit();
}

void getKey_menu()
{
	bool move_on = false;
	bool exit = false;

	while(move_on == false)
	{
		char choice = getch();
		
		switch(choice)
		{
			case '1': 
				drawDesktop(177, VGA_COLOR_BLACK, VGA_COLOR_WHITE); 
				move_on = true; 
				start_task(init_shell, 0, deadline_MEDIUM, resources_LOW);
				break;
			case '2': 
				move_on = true; 
				init_simple_vga();
				break;
			case '3': 
				move_on = true; 
				shutdown();
				break;
			case '4': 
				move_on = true; 
				reboot();
				break;
			default: 
				break;
		}		
	}	
}
