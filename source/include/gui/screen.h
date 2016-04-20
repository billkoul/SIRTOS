#ifndef SCREEN_H
#define SCREEN_H

#define VGA_CRTC_INDEX       0x3D4
#define VGA_CRTC_DATA        0x3D5

#define VGA_COLOR_BLACK     0
#define VGA_COLOR_BLUE      1
#define VGA_COLOR_GREEN     2
#define VGA_COLOR_AQUA      3
#define VGA_COLOR_RED       4
#define VGA_COLOR_PURPLE    5
#define VGA_COLOR_YELLOW    6
#define VGA_COLOR_WHITE     7
#define VGA_COLOR_GRAY      8
#define VGA_COLOR_LTBLUE    9
#define VGA_COLOR_LTGREEN   10
#define VGA_COLOR_LTAQUA    11
#define VGA_COLOR_LTRED     12
#define VGA_COLOR_LTPURPLE  13
#define VGA_COLOR_LTYELLOW  14
#define VGA_COLOR_LTWHITE   15

void showCursor();
void hideCursor();
void updateCursor();
void _cls(unsigned char ch);
unsigned char _putch(unsigned char ch);
void putsnocr(const char *text, ...);
void _puts(char *text);
void _setTextColor(char foreground, char background);
void goToxy(int x, int y);
void drawProgressBar();

#endif
