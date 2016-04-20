#include <lib/types.h>
#include <gui/window.h>
#include <gui/gfx.h>

extern bool graphical_mode;
/* These define our textpointer, our background and foreground
*  colors (attributes), and x and y cursor coordinates */
unsigned short *textmemptr  = (unsigned short *)0xB8000;
int attrib = 0x0F;
volatile int cursor_x = 0, cursor_y = 0;
volatile int g_cursor_x = 0;
volatile int g_cursor_y = 0;
extern unsigned g_wd, g_ht;
extern volatile window_t current_window;
extern volatile window_t window_list[];
extern unsigned COLOR_FONT;

void set_cursor_window()
{
		g_cursor_x = current_window.x + 5;
		g_cursor_y = current_window.y + 20;
}

static void scroll()
{

	if(graphical_mode)
		return;
   	// Get a space character with the default colour attributes.
   	u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
   	u16int blank = 0x20 /* space */ | (attributeByte << 8);

   	// Row 25 is the end, this means we need to scroll up
   	if(cursor_y >= 25)
   	{
       		// Move the current text chunk that makes up the screen
       		// back in the buffer by a line
       		int i;
       		for (i = 0*80; i < 24*80; i++)
       		{
           		textmemptr[i] = textmemptr[i+80];
       		}

       		// The last line should now be blank. Do this by writing
       		// 80 spaces to it.
       		for (i = 24*80; i < 25*80; i++)
       		{
           		textmemptr[i] = blank;
       		}
       		// The cursor should now be on the last line.
       		cursor_y = 24;
   }
}

void cls()
{
	if(graphical_mode)
		return;

	u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
       	u16int blank = 0x20 /* space */ | (attributeByte << 8);

       	for (int i = 0*80; i < 25*80; i++)
       	{
           	textmemptr[i] = blank;
       	}
       	cursor_y = 1;
}

void k_gotoxy(int x, int y)
{
	unsigned cur_pos = cursor_y * 80 + cursor_x;
        // Round x, y to CRT bounds
        if (x != -1)
                x = x % g_wd;
        else
                x = cur_pos % g_ht;
 
        if (y != -1)
                y = y % g_ht;
        else
                y = cur_pos / g_wd;
 
        cli();
        cur_pos = y*g_wd + x;
 
        move_cursor_at(cur_pos);
	sti();
}

void move_cursor_at(unsigned temp)
{
   	//Graphical mode
    	if(graphical_mode)
		return;

    	outb(0x3D4, 14);
    	outb(0x3D5, temp >> 8);
    	outb(0x3D4, 15);
    	outb(0x3D5, temp);
}

static void move_csr(void)
{
    	//Graphical mode
    	if(graphical_mode)
		return;
 
    	unsigned temp;

    	/* The equation for finding the index in a linear
    	*  chunk of memory can be represented by:
    	*  Index = [(y * width) + x] */
    	temp = cursor_y * 80 + cursor_x;

    	/* This sends a command to indicies 14 and 15 in the
    	*  CRT Control Register of the VGA controller. These
    	*  are the high and low bytes of the index that show
    	*  where the hardware cursor is to be 'blinking'. To
    	*  learn more, you should look up some VGA specific
    	*  programming documents. A great start to graphics:
    	*  http://www.brackeen.com/home/vga */
    	outb(0x3D4, 14);
    	outb(0x3D5, temp >> 8);
    	outb(0x3D4, 15);
    	outb(0x3D5, temp);
}

void monitor_put(unsigned char c)
{
    	if(graphical_mode)
    	{
         	draw_char(g_cursor_x, g_cursor_y, c, COLOR_FONT);
         	return;
   	}
    
    	unsigned short *where;
    	unsigned att = attrib << 8;

    	// Handle a backspace, by moving the cursor back one space then printing a blank character
    	if(c == '\b')
    	{
        	if(cursor_x != 0) 
			cursor_x--;
        	printf(" ");
        	if(cursor_x != 0)
			cursor_x--;
    	}

   	// Handles a tab by incrementing the cursor's x
    	else if(c == 0x09)
    	{
        	cursor_x = (cursor_x + 8) & ~(8 - 1);
    	}
    	else if(c == '\r')
    	{
        	cursor_x = 0;
    	}
    	else if(c == '\n')
    	{
        	cursor_x = 0;
        	cursor_y++;
    	}
    	/* Any character greater than and including a space, is a
    	*  printable character. The equation for finding the index
    	*  in a linear chunk of memory can be represented by:
    	*  Index = [(y * width) + x] */
    	else if(c >= ' ')
    	{
        	where = textmemptr + (cursor_y * 80 + cursor_x);
        	*where = c | att;	// Character AND attributes: color
        	cursor_x++;
    	}

    	// If the cursor has reached the end of the screen's width, insert a new line
    	if(cursor_x >= 80)
    	{
        	cursor_x = 0;
        	cursor_y++;
    	}

    	// Scroll the screen if needed, and finally move the cursor
    	scroll();
    	move_csr();
}

void monitor_write(unsigned char *text)
{
    	if(graphical_mode)
		return;
    
    	int i;

    	for (i = 0; i < strlen(text); i++)
        	monitor_put(text[i]);
}

void settextcolor(unsigned char forecolor, unsigned char backcolor)
{
    	if(graphical_mode)
		return;
    
    	// Top 4 bytes are the background, bottom 4 bytes are the foreground color
    	attrib = (backcolor << 4) | (forecolor & 0x0F);
}

void console_prinf(const char *text)
{
	if(graphical_mode)
		return;
	else
		printf(text);
}

void printf (const char *format,...)
{
    	char **arg = (char **) &format;
    	char *p;
    	int c;
    	char buf[20];
     
   	arg++;
     
    	if(graphical_mode)
    	{
    		while ((c = *format++) != 0)
    		{
           		if (c != '%')
           		{
				if(window_list[current_window.id].type == 2)
				{
					for(int i = 0; i < strlen(format); i++)
					{
	                   			window_list[current_window.id].buffer[window_list[current_window.id].buffer_counter] = format[i]; 
						window_list[current_window.id].buffer_counter++;
					}
				}
                		//draw_text(g_cursor_x, g_cursor_y, format, COLOR_FONT);
				break;
          		}
           		else
           		{
               			char *p;
               			c = *format++;
               			switch (c)
               			{
                 			case 'd':
                 			case 'u':
                 			case 'x':
                   				itoa(buf, c, *((int *) arg++));
                   				p = buf;
                   				goto g_string;
                   				break;
     
                 			case 's':
                  				p = *arg++;
                   				if (! p)
                     					p = "(null)";
     
                 				g_string:
						if(window_list[current_window.id].type == 2)
						{
							for(int i = 0; i < strlen(format); i++)
							{
	                   					window_list[current_window.id].buffer[window_list[current_window.id].buffer_counter] = p; 
								window_list[current_window.id].buffer_counter++;
							}
						}
						//draw_text(g_cursor_x,g_cursor_y,p, COLOR_FONT);
                   				break;
     
                 			default:
						if(window_list[current_window.id].type == 2)
						{
							for(int i = 0; i < strlen(format); i++)
							{
	                   					window_list[current_window.id].buffer[window_list[current_window.id].buffer_counter] = format[i]; 
								window_list[current_window.id].buffer_counter++;
							}
						}
                     				//draw_text(g_cursor_x,g_cursor_y,format, COLOR_FONT);
                   				break;
               			}
          		}
    		}
    		return;
    	}
    	else if(!graphical_mode)
    	{
    		while((c = *format++) != 0)
    		{
			if(c != '%')
				monitor_put(c);
            		else
            		{
                 		c = *format++;
                 		switch (c)
                 		{
					case 'd':
					case 'u':
					case 'x':
						itoa (buf, c, *((int *) arg++));
						p = buf;
						goto string;
					break;
     
					case 's':
						p = *arg++;
						if (! p)
						p = "(null)";
     
						string:
						while (*p)
							monitor_put (*p++);
						break;
     
					default:
						monitor_put (*((int *) arg++));
						break;
                 		}
             		}
		}
    	}
}
