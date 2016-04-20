/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	MEMORY BROWSER
*/
#include <gui/window.h>
#include <gui/gfx.h>

extern volatile window_t current_window;
extern volatile window_t window_list[];
int browser_id =0;
extern volatile int empty_id[];

extern u32int memsize;
char memtotal[10];
char memused[10];

void init_mem_browser()
{
	browser_id = create_window(500, 400, "Memory Browser",  COLOR_BACKGROUND, COLOR_BLACK, 1);

	int mu = get_memory_usage();
	itoa(memused, 10, mu);
	int ms = memsize;
	itoa(memtotal, 10, ms);

	int c = 0;
	int r = 0;

	for(int i=0; i<1900; i++)
	{
		if(c >= 49)
		{
			c == 0;
			r++;
		}
		else
		{
			c++;
		}

		if(i<=mu)
		{
       			 window_add_item(5+(10*c), 10+(10*r), 150, 20, COLOR_BLACK, COLOR_MENUBAR, "1", browser_id, 0, 0);
		}
		else
		{
        		window_add_item(5+(10*c), 10+(10*r), 150, 20, COLOR_BLACK, COLOR_MENUBAR, "0", browser_id, 0, 0);
		}
	}
}
