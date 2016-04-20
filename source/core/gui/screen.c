#include <gui/screen.h>

unsigned short *screen = (unsigned short *)0xB8000;
unsigned short attr = 0x0F00;
int cursorx = 0, cursory = 0;

void *_memset(void *ptr, unsigned char value, unsigned int num)
{
	unsigned char *pos = (unsigned char *)ptr;
	while(num--) *pos++ = value;
	return ptr;
}

void *_memsetw(void *ptr, unsigned short value, unsigned int num)
{
	unsigned short *pos = (unsigned short *)ptr;
	while(num--) *pos++ = value;
	return ptr;
}

void _outportb(unsigned short port, unsigned char data)
{
    __asm__("outb %1, %0" : : "d"(port), "a" (data));
}

unsigned char _inportb(unsigned short port)
{
	unsigned char data;
	__asm__("inb %1, %0" : "=a" (data) : "d" (port));
	return data;
}

void updateCursor()
{
	unsigned int index = (cursory * 80) + cursorx;
	_outportb(VGA_CRTC_INDEX, 14);
	_outportb(VGA_CRTC_DATA,  (index >> 8));
	_outportb(VGA_CRTC_INDEX, 15);
	_outportb(VGA_CRTC_DATA,  (index & 0xFF));
}

void showCursor()
{
	_outportb(VGA_CRTC_INDEX, 10);
	_outportb(VGA_CRTC_DATA, (_inportb(VGA_CRTC_DATA) & ~0x20));
}

void hideCursor()
{
	_outportb(VGA_CRTC_INDEX, 10);
	_outportb(VGA_CRTC_DATA, (_inportb(VGA_CRTC_DATA) | 0x20));
}

void _cls(unsigned char ch)
{
    _memsetw(screen, (attr | ch), 80 * 25);
    cursorx = 0; cursory = 0;
    updateCursor();
}

unsigned char _putch(unsigned char ch)
{
    switch(ch){
        case '\b': if(cursorx > 0) cursorx--;          break;
        case '\t': cursorx = (cursorx + 4) & ~(4 - 1); break;
        case '\r': cursorx = 0;                        break;
        case '\n': cursorx = 0; cursory++;             break;
          default: if(ch < ' ')                        break;
            *(screen + (cursory * 80) + cursorx) = (attr | ch);
            cursorx++;
    }
    if(cursorx >= 80){
        cursorx = 0; cursory++;
    }
    updateCursor();
    return ch;
}

void putsnocr(const char *text, ...)
{
    goToxy(4, cursory + 1);
    while(*text)
	_putch(*text++);
}

void putTitle(const char *text, ...)
{
    while(*text)
	_putch(*text++);
}

void _puts(char *text)
{
    putsnocr(text);
	_putch('\n');
}

void _setTextColor(char fore, char back)
{
    attr = ((back << 12) | ((fore & 0x0F) << 8));
}

void goToxy(int x, int y)
{
    cursorx = x; cursory = y;
    updateCursor();
}

int _strlen(char *str)
{
	char *end = str;
	while(*end++);
	return (end - str);
}

void drawDesktop(unsigned char ch, char fore, char back)
{
	// clear screen with foreground and background
	// and also with desired character
	_setTextColor(fore, back);
	_cls(ch);
}

void drawPoint(int xpos, int ypos, int width, int height, char fore, char back)
{	
	int xextent = (xpos + width - 1), yextent = (ypos + height - 1);
	
	// draw window
	for(int y = ypos; y <= yextent; y++){
		_setTextColor(fore, back);
		goToxy(xpos, y);
		
		if(y == ypos){
			// draw top caption bar
			_setTextColor(VGA_COLOR_LTYELLOW, VGA_COLOR_GREEN);
			for(int x = xpos; x <= xextent; x++){
				if(x == xpos) _putch(196);
				else if(x == xextent) _putch(196);
				else _putch(196);
			}
			
		}else if(y < yextent){
			// draw middle lines
			for(int x = xpos; x <= xextent; x++){
				if(x == xpos || x == xextent) _putch(196);
				else _putch(' ');
			}
			
		}else{
			// draw bottom edge
			for(int x = xpos; x <= xextent; x++){
				if(x == xpos) _putch(196);
				else if(x == xextent) _putch(196);
				else _putch(196);
			}
		}
	}
	// position cursor inside window
	goToxy(xpos + 4, ypos + 2);
	_setTextColor(fore, back);
}

void drawProgress(int xpos, int ypos, int width, int height, char fore, char back)
{
	int xextent = (xpos + width - 1), yextent = (ypos + height - 1);
	
	// draw window
	for(int y = ypos; y <= yextent; y++){
		_setTextColor(fore, back);
		goToxy(xpos, y);
		
		if(y == ypos){
			// draw top caption bar
			_setTextColor(VGA_COLOR_LTYELLOW, VGA_COLOR_RED);
			for(int x = xpos; x <= xextent; x++){
				if(x == xpos) _putch(213);
				else if(x == xextent) _putch(254);
				else _putch(205);
			}
			
		}else if(y < yextent){
			// draw middle lines
			for(int x = xpos; x <= xextent; x++){
				if(x == xpos || x == xextent) _putch(179);
				else _putch(' ');
			}
			
		}else{
			// draw bottom edge
			for(int x = xpos; x <= xextent; x++){
				if(x == xpos) _putch(192);
				else if(x == xextent) _putch(217);
				else _putch(196);
			}
		}
	}
	// position cursor inside window
	goToxy(xpos + 4, ypos + 2);
	_setTextColor(fore, back);
}

void drawWindow(char *title, int xpos, int ypos, int width, int height, char fore, char back)
{
	int xextent = (xpos + width - 1), yextent = (ypos + height - 1);
	
	// draw window
	for(int y = ypos; y <= yextent; y++){
		_setTextColor(fore, back);
		goToxy(xpos, y);
		
		if(y == ypos){
			// draw top caption bar
			_setTextColor(VGA_COLOR_LTYELLOW, VGA_COLOR_RED);
			for(int x = xpos; x <= xextent; x++){
				if(x == xpos) _putch(213);
				else if(x == xextent) _putch(254);
				else _putch(205);
			}
			goToxy(xpos + ((width - _strlen(title)) / 2), y);
			putTitle(title);
			
		}else if(y < yextent){
			// draw middle lines
			for(int x = xpos; x <= xextent; x++){
				if(x == xpos || x == xextent) _putch(179);
				else _putch(' ');
			}
			
		}else{
			// draw bottom edge
			for(int x = xpos; x <= xextent; x++){
				if(x == xpos) _putch(192);
				else if(x == xextent) _putch(217);
				else _putch(196);
			}
		}
	}
	
	// draw vertical shadow
	for(int y = ypos; y <= yextent; y++){
		goToxy(xextent + 1, y + 1);
		_setTextColor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
		_putch(177);
	}
	
	// draw horizontal shadow
	for(int x = xpos; x <= xextent; x++){
		goToxy(x + 1, yextent + 1);
		_setTextColor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
		_putch(177);
	}
	
	// position cursor inside window
	goToxy(xpos + 4, ypos + 2);
	_setTextColor(fore, back);
}
