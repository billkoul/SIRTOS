#ifndef STDIO_H
#define STDIO_H

void printf (const char *format,...);
void drawf (const char *format,...);
void settextcolor(unsigned char forecolor, unsigned char backcolor);
static void getfontchar(unsigned char character);
void scroll_window();
void monitor_write(unsigned char *text);
void monitor_put(unsigned char c);
#endif
