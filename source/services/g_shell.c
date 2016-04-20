/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	GRAPHICAL SHELL
*/
#include <gui/window.h>
#include <gui/gfx.h>

extern volatile bool draw_keys;
int shell_id;
extern volatile window_t window_list[];
extern volatile window_t current_window;
extern void set_cursor_window();
int items = 0;

void g_shell_parse_input()
{
}

void init_g_shell()
{
	shell_id = create_window(400, 400, "Shell",  COLOR_BLACK, COLOR_WHITE, 2);
	draw_keys = true;
}
