/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

YOU MAY USE THIS CODE FOR PERSONAL PURPOSES ONLY.

	WINDOW HEADER
*/
#ifndef WINDOW_H
#define WINDOW_H

#include <lib/types.h>

typedef struct window_item
{
	int id;
	int parent_id;
        volatile int x;
        volatile int y;
        int width;
        int height;
        volatile char *name;
        int arg;
        unsigned background;
        unsigned forecolor;
	void (*func)(void*)

} window_item_t;

typedef struct window_graphic
{
	int id;
	int parent_id;
        volatile int x;
        volatile int y;
        volatile int width;
        volatile int height;
        unsigned color;
	bool fixed_x;
	bool fixed_y;

} window_graphic_t;

typedef struct window
{
        int id;
        volatile int x;
        volatile int y;
        volatile int z;
        volatile int width;
        volatile int height;
        char *title;
        unsigned background;
        unsigned forecolor;
	window_item_t item[100];
	int _items;
	window_graphic_t graphic[2000];
	int _graphics;
	bool visible;
	int type;
	volatile char buffer[1000];
	volatile int buffer_counter;
	bool maximized;

} window_t;

typedef struct info_window
{
        int id;
        volatile int x;
        volatile int y;
        volatile int z;
        volatile int width;
        volatile int height;
        char *title;
	char *info;
        unsigned background;
        unsigned forecolor;
	int type;
} info_window_t;

#endif
