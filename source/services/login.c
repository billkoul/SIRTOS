/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	USER LOGIN
*/
#include <gui/window.h>
#include <gui/gfx.h>

extern volatile window_t current_window;

void init_login()
{
	int id = create_window(300, 300, "User Login",  COLOR_BACKGROUND, COLOR_BLACK, 1);
        window_add_item(5, 25, 120, 20, COLOR_WHITE, COLOR_MENUBAR, "Username", id, 0, 0);
        window_add_item(5, 50, 120, 20, COLOR_WHITE, COLOR_MENUBAR, "Password", id, 0, 0);
}
