#include <system.h>
#include <gui/gfx.h>
#include <gui/keymaps/keymap-us.h>

//external functions
extern volatile int g_cursor_x, g_cursor_y;
extern unsigned long g_wd, g_ht;
extern int graphical_mode;

extern volatile int mouse_x;
extern volatile int mouse_y;

volatile bool shift_flag=0;
volatile bool caps_flag=0;
volatile bool ctrl_flag=0;
volatile bool alt_flag=0;

volatile char* buffer; //For storing strings
volatile char* buffer2;
volatile u32int kb_count = 0; //Position in buffer
volatile bool gets_flag = false;

volatile bool draw_keys = false;
static void do_gets();

unsigned char kb_map_en_us[2][128] =
{
	{
		0, KB_KEY_ESCAPE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',  'q',
		'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd',
		'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b',
		'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0,	KB_KEY_F1, KB_KEY_F2, KB_KEY_F3, KB_KEY_F4, KB_KEY_F5, KB_KEY_F6,
		KB_KEY_F7, KB_KEY_F8, KB_KEY_F9, KB_KEY_F10, 0, 0, KB_KEY_HOME, KB_KEY_UP, KB_KEY_PGUP, '-', KB_KEY_LEFT, '5', KB_KEY_RIGHT, '+', KB_KEY_END, KB_KEY_DOWN,
		KB_KEY_PGDN, KB_KEY_INSERT, KB_KEY_DELETE, 0, 0, 0, KB_KEY_F11, KB_KEY_F12, 0
	},
	{
		0,  KB_KEY_ESCAPE, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
		0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '`', 0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0,
		KB_KEY_F1, KB_KEY_F2, KB_KEY_F3, KB_KEY_F4, KB_KEY_F5, KB_KEY_F6, KB_KEY_F7, KB_KEY_F8, 
		KB_KEY_F9, KB_KEY_F10, 0, 0, KB_KEY_HOME, KB_KEY_UP, KB_KEY_PGUP, '-', KB_KEY_LEFT, '5', KB_KEY_RIGHT, '+', KB_KEY_END,
		KB_KEY_DOWN, KB_KEY_PGDN, KB_KEY_INSERT, KB_KEY_DELETE, 0, 0, 0, KB_KEY_F11, KB_KEY_F12, 0
	},
};

// output data to the data port
int kb_write(unsigned char data)
{
	unsigned long timeout = 500000;
	while(--timeout){
		if(!(inportb(KB_PORT_CMD) & 0x02)){
			break;
		}
	}
	if(timeout > 0){
		outportb(KB_PORT_DATA, data);
		return 1;
	}
	return 0;
}

// set appropriate keyboad LED's
void kb_setleds(unsigned long status)
{
	// todo: check keyboard controller isn't busy
	// while(!((inportb(KB_PORT_CMD) & 2) == 0);
	status &= KB_MASK_LEDS;
	kb_write(KB_CMD_SETLEDS);
	kb_write((unsigned char)status);
}

void keyboard_handler()
{
	static unsigned long kb_status = 0;
	unsigned char scancode;
	int keycode;
	
	// read data from keyboard
	keycode  = -1;
	scancode = inportb(KB_PORT_DATA);
	
	// key up
	if(scancode & KB_MASK_BREAK){
		scancode &= KB_MASK_KEY;
		switch(scancode)
		{
			// remove bits from the status variable depending
			case KB_KEY_LCTRL:          
				kb_status &= ~KB_MASK_CTRL;          
				break;
			case KB_KEY_LALT:           
				kb_status &= ~KB_MASK_ALT;           
				break;
			case KB_KEY_LSHIFT:
			case KB_KEY_RSHIFT:         
				kb_status &= ~KB_MASK_SHIFT;         
				break;
		}
	}
	// key down
	else{
		scancode &= KB_MASK_KEY;
		switch(scancode)
		{
			// add bits to the status variable depending
			case KB_KEY_LCTRL:          
				kb_status |= KB_MASK_CTRL;          
				break;
			case KB_KEY_LALT:           
				kb_status |= KB_MASK_ALT;           
				break; 
			case KB_KEY_LSHIFT:
			case KB_KEY_RSHIFT:         
				kb_status |= KB_MASK_SHIFT;         
				break;
			// toggle bits from the status variable depending
			case KB_KEY_SCRLLOCK: kb_status ^= KB_MASK_SCRLLOCK; 
				kb_setleds(kb_status); 
				break;
			case KB_KEY_NUMLOCK:  kb_status ^= KB_MASK_NUMLOCK;  
				kb_setleds(kb_status);
				break;
			case KB_KEY_CAPSLOCK: kb_status ^= KB_MASK_CAPSLOCK;
				kb_setleds(kb_status); 
				break;
			// for all other keys
			default:
				// if capslock or shift is on, use shift keymap
				if(kb_status & KB_MASK_SHIFT || kb_status & KB_MASK_CAPSLOCK)
					keycode = kb_map_en_us[1][scancode];
				// else, use normal keymap
				else
					keycode = kb_map_en_us[0][scancode];
				break;
		}
	}
	if(keycode != -1){
		// if ctrl-alt-del, then reboot
		if((kb_status & KB_MASK_CTRL) && (kb_status & KB_MASK_ALT) && (keycode == KB_KEY_DELETE))
			reboot();
		// else push key into stack
		else
			if(draw_keys)
			{
        			//Print key
        			if(graphical_mode)
            				printf(g_cursor_x,g_cursor_y,(kb_map_en_us[0][scancode]), COLOR_BLACK); 
				else
             				monitor_put(kbdus[scancode]);
			}
	}
}

void init_keyboard(void)
{
	// install the keyboard interupt handler
	install_interrupt_handler(IRQ1, &keyboard_handler);
}

//Gets a key
unsigned char getch()
{
     	unsigned char getch_char;
     
     	if(kbdus[inb(0x60)] != 0) //Not empty
     		outb(0x60,0xf4); //Clear buffer
     
     	while(kbdus[inb(0x60)] == 0); //While buffer is empty
     		getch_char = kbdus[inb(0x60)];
     	outb(0x60,0xf4); //Leave it emptying
     	return getch_char;
}

unsigned char getascii()
{
     	unsigned char get_ascii;
     
     	if(kbdus[inb(0x60)] != 0) //Not empty
     		outb(0x60,0xf4); //Clear buffer
     
     	while(kbdus[inb(0x60)] == 0); //While buffer is empty
     		get_ascii = inb(0x60);
     	outb(0x60,0xf4); //Leave it emptying
     	return get_ascii;
}

char* gets()
{ 
     	gets_flag = 0;
     		while(gets_flag == 0);
     	return (char*)buffer2;
}

static void do_gets()
{
     	buffer[kb_count++] = 0; //Null terminated
     	for(;kb_count; kb_count--)
          	buffer2[kb_count] = buffer[kb_count];
     	return;
}
