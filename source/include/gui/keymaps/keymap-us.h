#ifndef KEYMAP_H
#define KEYMAP_H

//US keymap
unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    1,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   1,				/* Right shift */
  '*',
    1,	/* Alt */
  ' ',	/* Space bar */
    1,	/* Caps lock */
    1,	/* 59 - F1 key ... > */
    1,   1,   1,   1,   1,   1,   1,   1,
    1,	/* < ... F11 */
    1,	/* 69 - Num lock*/
    1,	/* Scroll Lock */
    1,	/* Home key */
    1,	/* 72 Up Arrow */
    1,	/* Page Up */
   '-',
    1,	/* 75 Left Arrow */
    1,
    1,	/* 77 Right Arrow */
  '+',
    1,	/* 79 - End key*/
    1,	/* 80 Down Arrow */
    1,	/* Page Down */
    1,	/* Insert Key */
    1,	/* Delete Key */
    1,   1,   '\\',
    1,	/* F11 Key */
    1,	/* F12 Key */
    1,	/* All other keys are undefined */
};		

#endif
