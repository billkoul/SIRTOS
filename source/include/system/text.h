#ifndef TEXT_H
#define TEXT_H

// Write a single character out to the screen.
void monitor_put(unsigned char c);

void k_gotoxy(int x, int y);

// Output an ASCII string to the monitor.
void monitor_write(unsigned char *text);

// Sets text colours
void settextcolor(unsigned char forecolor, unsigned char backcolor);

// Printf() for text mode
void kprintf (const char *format, ...);

#endif
