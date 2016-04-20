/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	FILE MANAGER
*/
#include <gui/window.h>
#include <gui/gfx.h>
#include <initrd/initrd.h>
#include <fs/fs.h>

extern volatile window_t current_window;

void init_file_manager()
{
	int id = create_window(300, 300, "File Manager",  COLOR_BACKGROUND, COLOR_BLACK, 1);	
	
	fs_node_t *file1 = readdir_fs(fs_root, 0);
	fs_node_t *file2 = readdir_fs(fs_root, 1);
	fs_node_t *file3 = readdir_fs(fs_root, 2);
	fs_node_t *file4 = readdir_fs(fs_root, 3);
	fs_node_t *file5 = readdir_fs(fs_root, 4);

 	window_add_item(5, 20+(25*0), 90, 20, COLOR_WHITE, COLOR_MENUBAR, file1->name, id, 0, 0);
 	window_add_item(5, 20+(25*1), 90, 20, COLOR_WHITE, COLOR_MENUBAR, file2->name, id, 0, 0);
 	window_add_item(5, 20+(25*2), 90, 20, COLOR_WHITE, COLOR_MENUBAR, file3->name, id, 0, 0);
 	window_add_item(5, 20+(25*3), 90, 20, COLOR_WHITE, COLOR_MENUBAR, file4->name, id, 0, 0);
 	window_add_item(5, 20+(25*4), 90, 20, COLOR_WHITE, COLOR_MENUBAR, file5->name, id, 0, 0);
}
