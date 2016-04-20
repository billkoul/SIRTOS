/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	CONTROL PANEL
*/
#include <gui/window.h>
#include <gui/gfx.h>
#include <system/version.h>
#include <system/user.h>

extern volatile window_t current_window;
extern u32int memsize;
extern user_t current_user;
extern char *cpuid_detect();

char mem_usage[10];
char total_mem[10];

void init_control_panel()
{
	int id = create_window(400, 400, "Control Panel",  COLOR_BACKGROUND, COLOR_BLACK, 1);
          
	window_add_item(5, 20, 100, 12, COLOR_BACKGROUND, COLOR_MENUBAR, "ShoRTOS", id, 0, 0);
        window_add_item(105, 20, 150, 12, COLOR_BACKGROUND, COLOR_MENUBAR, OS_VERSION, id, 0, 0);

        window_add_item(5, 40, 100, 12, COLOR_BACKGROUND, COLOR_MENUBAR, "Build Date", id, 0, 0);
        window_add_item(105, 40, 150, 12, COLOR_BACKGROUND, COLOR_MENUBAR, OS_BUILD_DATE, id, 0, 0);
        window_add_item(160, 40, 150, 12, COLOR_BACKGROUND, COLOR_MENUBAR, OS_BUILD_TIME, id, 0, 0);

        window_add_item(5, 60, 100, 12, COLOR_BACKGROUND, COLOR_MENUBAR, "Username", id, 0, 0);
        window_add_item(105, 60, 150, 12, COLOR_BACKGROUND, COLOR_MENUBAR, current_user.name, id, 0, 0);

	int i = get_memory_usage()/1024/1024;
	itoa(mem_usage, 10, i);
	int j = memsize/1024/1024;
	itoa(total_mem, 10, j);

        window_add_item(5, 80, 150, 12, COLOR_BACKGROUND, COLOR_MENUBAR, "Memory usage (Mb)", id, 0, 0);
        window_add_item(155, 80, 50, 12, COLOR_BACKGROUND, COLOR_MENUBAR, mem_usage, id, 0, 0);
        window_add_item(5, 100, 150, 12, COLOR_BACKGROUND, COLOR_MENUBAR, "Total memory (Mb)", id, 0, 0);
        window_add_item(155, 100, 50, 12, COLOR_BACKGROUND, COLOR_MENUBAR, total_mem, id, 0, 0);

	char *cpuid = cpuid_detect();
        window_add_item(5, 120, 150, 12, COLOR_BACKGROUND, COLOR_MENUBAR, "CPU", id, 0, 0);
        window_add_item(5, 140, 300, 12, COLOR_BACKGROUND, COLOR_MENUBAR, cpuid, id, 0, 0);
}
