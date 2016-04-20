#ifndef KEYB_H
#define KEYB_H

#define	KB_PORT_DATA		0x60
#define	KB_PORT_CMD			0x64

#define	KB_CMD_REBOOT		0xFE
#define	KB_CMD_SETLEDS		0xED

#define	KB_MASK_LEDS		0x07
#define	KB_MASK_BREAK		0x80
#define	KB_MASK_KEY			0x7F
#define	KB_MASK_SCRLLOCK	0x01
#define	KB_MASK_NUMLOCK	0x02
#define	KB_MASK_CAPSLOCK	0x04
#define	KB_MASK_CTRL		0x08
#define	KB_MASK_ALT			0x10
#define	KB_MASK_SHIFT		0x20

#define 	KB_KEY_LCTRL		0x1D
#define 	KB_KEY_LSHIFT		0x2A
#define 	KB_KEY_RSHIFT		0x36
#define 	KB_KEY_LALT			0x38
#define 	KB_KEY_CAPSLOCK		0x3A
#define 	KB_KEY_NUMLOCK		0x45
#define	KB_KEY_SCRLLOCK		0x46

#define	KB_KEY_F1			0x80
#define	KB_KEY_F2			0x81
#define	KB_KEY_F3			0x82
#define	KB_KEY_F4			0x83
#define 	KB_KEY_F5			0x84
#define	KB_KEY_F6			0x85
#define	KB_KEY_F7			0x86
#define	KB_KEY_F8			0x87
#define	KB_KEY_F9			0x88
#define	KB_KEY_F10			0x89
#define	KB_KEY_F11			0x8A
#define	KB_KEY_F12			0x8B
#define	KB_KEY_INSERT		0x90
#define	KB_KEY_DELETE		0x91
#define	KB_KEY_HOME			0x92
#define	KB_KEY_END			0x93
#define	KB_KEY_PGUP			0x94
#define	KB_KEY_PGDN			0x95
#define	KB_KEY_LEFT			0x96
#define	KB_KEY_UP			0x97
#define	KB_KEY_DOWN		0x98
#define	KB_KEY_RIGHT		0x99
#define	KB_KEY_PRINT			0x9A
#define	KB_KEY_PAUSE		0x9B
#define	KB_KEY_LWIN			0x9C
#define	KB_KEY_RWIN			0x9D
#define	KB_KEY_MENU			0x9E
#define	KB_KEY_ESCAPE		0x27

extern unsigned char getch();

#endif
