#include <gui/fonts/8x8.h>
#include <gui/gfx.h>
#include <gui/window.h>

extern volatile int g_cursor_x, g_cursor_y;
extern unsigned g_wd;
extern int shift_flag, caps_flag;
extern bool usermode;
char* font;
static void get_font_char(unsigned char character);
void get_font(unsigned char character);
unsigned COLOR_FONT = COLOR_WHITE;
extern window_t current_window;

void draw_char(int x, int y, unsigned char character, int c)
{
	int i, l;
	int j = x;
	int h = y;
	
	if(usermode)
		get_font_char(character);
	else
		get_font(character);
	
	for (l = 0; l < 8; l++)
	{
		for (i = 8; i > 0; i--)
		{
            		j++;
			if ((font[l] & (1<<i)))
			{
				putPixel_32bpp(j, h, c);
            		}
		}
		h++;
		j = x;
	}
}

void draw_text(int x, int y, char *text, int c)
{
	int i, l;
	int j = x;
	int h = y;
	int xpos = x;
	
	for(int no=0; no<strlen(text); no++)
	{
		xpos += 9;
		if(usermode)
			get_font_char(text[no]);
		else
			get_font(text[no]);
	
		for (l = 0; l < 8; l++)
		{
			for (i = 8; i > 0; i--)
			{
            			j++;
				if ((font[l] & (1<<i)))
				{
					putPixel_32bpp(xpos+j, h, c);
            			}
			}
			h++;
			j = x;
		}
		h = y;
	}
}

static void get_font_char(unsigned char character)
{
    	switch(character)
	{
         case 'A':
              if(shift_flag || caps_flag)
              font = font_8x8_A;
              else
              font = font_8x8_a;
         break;
         case 'B':
              if(shift_flag || caps_flag)
              font = font_8x8_B;
              else
              font = font_8x8_b;
         break;
         case 'C':
              if(shift_flag || caps_flag)
              font = font_8x8_C;
              else
              font = font_8x8_c;
         break;
         case 'D':
              if(shift_flag || caps_flag)
              font = font_8x8_D;
              else
              font = font_8x8_d;
         break;
         case 'E':
              if(shift_flag || caps_flag)
              font = font_8x8_E;
              else
              font = font_8x8_e;
         break;
         case 'F':
              if(shift_flag || caps_flag)
              font = font_8x8_F;
              else
              font = font_8x8_f;
         break;
         case 'G':
              if(shift_flag || caps_flag)
              font = font_8x8_G;
              else
              font = font_8x8_g;
         break;
         case 'H':
              if(shift_flag || caps_flag)
              font = font_8x8_H;
              else
              font = font_8x8_h;
         break;
         case 'I':
              if(shift_flag || caps_flag)
              font = font_8x8_I;
              else
              font = font_8x8_i;
         break;
         case 'J':
              if(shift_flag || caps_flag)
              font = font_8x8_J;
              else
              font = font_8x8_j;
         break;
         case 'K':
              if(shift_flag || caps_flag)
              font = font_8x8_K;
              else
              font = font_8x8_k;
         break;
         case 'L':
              if(shift_flag || caps_flag)
              font = font_8x8_L;
              else
              font = font_8x8_l;
         break;
         case 'M':
              if(shift_flag || caps_flag)
              font = font_8x8_M;
              else
              font = font_8x8_m;
         break;
         case 'N':
              if(shift_flag || caps_flag)
              font = font_8x8_N;
              else
              font = font_8x8_n;
         break;
         case 'O':
              if(shift_flag || caps_flag)
              font = font_8x8_O;
              else
              font = font_8x8_o;
         break;
         case 'P':
              if(shift_flag || caps_flag)
              font = font_8x8_P;
              else
              font = font_8x8_p;
         break;
         case 'Q':
              if(shift_flag || caps_flag)
              font = font_8x8_Q;
              else
              font = font_8x8_q;
         break;
         case 'R':
              if(shift_flag || caps_flag)
              font = font_8x8_R;
              else
              font = font_8x8_r;
         break;
         case 'S':
              if(shift_flag || caps_flag)
              font = font_8x8_S;
              else
              font = font_8x8_s;
         break;
         case 'T':
              if(shift_flag || caps_flag)
              font = font_8x8_T;
              else
              font = font_8x8_t;
         break;
         case 'U':
              if(shift_flag || caps_flag)
              font = font_8x8_U;
              else
              font = font_8x8_u;
         break;
         case 'V':
              if(shift_flag || caps_flag)
              font = font_8x8_V;
              else
              font = font_8x8_v;
         break;
         case 'W':
              if(shift_flag || caps_flag)
              font = font_8x8_W;
              else
              font = font_8x8_w;
         break;
         case 'X':
              if(shift_flag || caps_flag)
              font = font_8x8_X;
              else
              font = font_8x8_x;
         break;
         case 'Y':
              if(shift_flag || caps_flag)
              font = font_8x8_Y;
              else
              font = font_8x8_y;
         break;
         case 'Z':
              if(shift_flag || caps_flag)
              font = font_8x8_Z;
              else
              font = font_8x8_z;
         break;
         case 'a':
              font = font_8x8_a;
         break;
         case 'b':
              font = font_8x8_b;
         break;
         case 'c':
              font = font_8x8_c;
         break;
         case 'd':
              font = font_8x8_d;
         break;
         case 'e':
              font = font_8x8_e;
         break;
         case 'f':
              font = font_8x8_f;
         break;
         case 'g':
              font = font_8x8_g;
         break;
         case 'h':
              font = font_8x8_h;
         break;
         case 'i':
              font = font_8x8_i;
         break;
         case 'j':
              font = font_8x8_j;
         break;
         case 'k':
              font = font_8x8_k;
         break;
         case 'l':
              font = font_8x8_l;
         break;
         case 'm':
              font = font_8x8_m;
         break;
         case 'n':
              font = font_8x8_n;
         break;
         case 'o':
              font = font_8x8_o;
         break;
         case 'p':
              font = font_8x8_p;
         break;
         case 'q':
              font = font_8x8_q;
         break;
         case 'r':
              font = font_8x8_r;
         break;
         case 's':
              font = font_8x8_s;
         break;
         case 't':
              font = font_8x8_t;
         break;
         case 'u':
              font = font_8x8_u;
         break;
         case 'v':
              font = font_8x8_v;
         break;
         case 'w':
              font = font_8x8_w;
         break;
         case 'x':
              font = font_8x8_x;
         break;
         case 'y':
              font = font_8x8_y;
         break;
         case 'z':
              font = font_8x8_z;
         break;
         case '0':
              font = font_8x8_0;
         break;
         case '1':
              if(shift_flag)
              font = font_8x8_exclamation;
              else
              font = font_8x8_1;
         break;
         case '2':
              font = font_8x8_2;
         break;
         case '3':
              font = font_8x8_3;
         break;
         case '4':
              if(shift_flag)
              font = font_8x8_dollar;
              else
              font = font_8x8_4;
         break;
         case '5':
              font = font_8x8_5;
         break;
         case '6':
              font = font_8x8_6;
         break;
         case '7':
              font = font_8x8_7;
         break;
         case '8':
              font = font_8x8_8;
         break;
         case '9':
              font = font_8x8_9;
         break;
         case ' ':
              font = font_8x8_space;
         break;
         case '.':
              font = font_8x8_period;
         break;
         case '[':
              font = font_8x8_left_square_brace;
         break;
         case ']':
              font = font_8x8_right_square_brace;
         break;
         case '$':
              font = font_8x8_dollar;
         break;
         case '-':
              font = font_8x8_minus;
         break;
         case '+':
              font = font_8x8_plus;
         break;
         case '=':
              font = font_8x8_equals;
         break;
         case '<':
              font = font_8x8_less;
         break;
         case '>':
              font = font_8x8_greater;
         break;
         case '*':
              font = font_8x8_star;
         break;
         case '/':
              font = font_8x8_slash;
         break;
         case ',':
              font = font_8x8_comma;
         break;
         case '!':
              font = font_8x8_exclamation;
         break;
         case '"':
              font = font_8x8_double_quote;
         break;
         case '(':
              font = font_8x8_left_paren;
         break;
         case ')':
              font = font_8x8_right_paren;
         break;
         case '|':
              font = font_8x8_or;
         break;
         case '_':
              font = font_8x8_underline;
         break;
         case '@':
              font = font_8x8_at;
         break;
         case '&':
              font = font_8x8_ampersand;
         break;
         case '%':
              font = font_8x8_percent;
         break;
         case '\\':
              font = font_8x8_backslash;
         break;
         case '~':
              font = font_8x8_skato;
         break;
         case ':':
              font = font_8x8_anokato;
         break;
         case '\n':
              g_cursor_y+=10;
		if(current_window.x)
              		g_cursor_x=current_window.x;
		else
			g_cursor_x=0;
              font = font_8x8_space;
         break;
         case '\b':
              if(g_cursor_x - 9 < 1){
              }else{
                   g_cursor_x-=9;
              }
              font = font_8x8_bspace;
         break;
         case '\t':
              g_cursor_x+=0;
              font = font_8x8_bspace;
         break;
         default:
              font = font_8x8_question_mark;//Question mark for unknown
         break;
    }
}

void get_font(unsigned char character)
{
   	 switch(character)
	{
         case 'A':
              font = font_8x8_A;
         break;
         case 'B':
              font = font_8x8_B;
         break;
         case 'C':
              font = font_8x8_C;
         break;
         case 'D':
              font = font_8x8_D;
         break;
         case 'E':
              font = font_8x8_E;
         break;
         case 'F':
              font = font_8x8_F;
         break;
         case 'G':
              font = font_8x8_G;
         break;
         case 'H':
              font = font_8x8_H;
         break;
         case 'I':
              font = font_8x8_I;
         break;
         case 'J':
              font = font_8x8_J;
         break;
         case 'K':
              font = font_8x8_K;
         break;
         case 'L':
              font = font_8x8_L;
         break;
         case 'M':
              font = font_8x8_M;
         break;
         case 'N':
              font = font_8x8_N;
         break;
         case 'O':
              font = font_8x8_O;
         break;
         case 'P':
              font = font_8x8_P;
         break;
         case 'Q':
              font = font_8x8_Q;
         break;
         case 'R':
              font = font_8x8_R;
         break;
         case 'S':
              font = font_8x8_S;
         break;
         case 'T':
              font = font_8x8_T;
         break;
         case 'U':
              font = font_8x8_U;
         break;
         case 'V':
              font = font_8x8_V;
         break;
         case 'W':
              font = font_8x8_W;
         break;
         case 'X':
              font = font_8x8_X;
         break;
         case 'Y':
              font = font_8x8_Y;
         break;
         case 'Z':
              font = font_8x8_Z;
         break;
         case 'a':
              font = font_8x8_a;
         break;
         case 'b':
              font = font_8x8_b;
         break;
         case 'c':
              font = font_8x8_c;
         break;
         case 'd':
              font = font_8x8_d;
         break;
         case 'e':
              font = font_8x8_e;
         break;
         case 'f':
              font = font_8x8_f;
         break;
         case 'g':
              font = font_8x8_g;
         break;
         case 'h':
              font = font_8x8_h;
         break;
         case 'i':
              font = font_8x8_i;
         break;
         case 'j':
              font = font_8x8_j;
         break;
         case 'k':
              font = font_8x8_k;
         break;
         case 'l':
              font = font_8x8_l;
         break;
         case 'm':
              font = font_8x8_m;
         break;
         case 'n':
              font = font_8x8_n;
         break;
         case 'o':
              font = font_8x8_o;
         break;
         case 'p':
              font = font_8x8_p;
         break;
         case 'q':
              font = font_8x8_q;
         break;
         case 'r':
              font = font_8x8_r;
         break;
         case 's':
              font = font_8x8_s;
         break;
         case 't':
              font = font_8x8_t;
         break;
         case 'u':
              font = font_8x8_u;
         break;
         case 'v':
              font = font_8x8_v;
         break;
         case 'w':
              font = font_8x8_w;
         break;
         case 'x':
              font = font_8x8_x;
         break;
         case 'y':
              font = font_8x8_y;
         break;
         case 'z':
              font = font_8x8_z;
         break;
         case '0':
              font = font_8x8_0;
         break;
         case '1':
              font = font_8x8_1;
         break;
         case '2':
              font = font_8x8_2;
         break;
         case '3':
              font = font_8x8_3;
         break;
         case '4':
              font = font_8x8_4;
         break;
         case '5':
              font = font_8x8_5;
         break;
         case '6':
              font = font_8x8_6;
         break;
         case '7':
              font = font_8x8_7;
         break;
         case '8':
              font = font_8x8_8;
         break;
         case '9':
              font = font_8x8_9;
         break;
         case ' ':
              font = font_8x8_space;
         break;
         case '.':
              font = font_8x8_period;
         break;
         case '[':
              font = font_8x8_left_square_brace;
         break;
         case ']':
              font = font_8x8_right_square_brace;
         break;
         case '$':
              font = font_8x8_dollar;
         break;
         case '-':
              font = font_8x8_minus;
         break;
         case '+':
              font = font_8x8_plus;
         break;
         case '=':
              font = font_8x8_equals;
         break;
         case '<':
              font = font_8x8_less;
         break;
         case '>':
              font = font_8x8_greater;
         break;
         case '*':
              font = font_8x8_star;
         break;
         case '/':
              font = font_8x8_slash;
         break;
         case ',':
              font = font_8x8_comma;
         break;
         case '!':
              font = font_8x8_exclamation;
         break;
         case '"':
              font = font_8x8_double_quote;
         break;
         case '(':
              font = font_8x8_left_paren;
         break;
         case ')':
              font = font_8x8_right_paren;
         break;
         case '|':
              font = font_8x8_or;
         break;
         case '_':
              font = font_8x8_underline;
         break;
         case '@':
              font = font_8x8_at;
         break;
         case '&':
              font = font_8x8_ampersand;
         break;
         case '%':
              font = font_8x8_percent;
         break;
         case '\\':
              font = font_8x8_backslash;
         break;
         case '~':
              font = font_8x8_skato;
         break;
         case ':':
              font = font_8x8_anokato;
         break;
         case '\n':
              	g_cursor_y+=10;
		if(current_window.x)
              		g_cursor_x=current_window.x;
		else
			g_cursor_x=0;
              	font = font_8x8_space;
         break;
         case '\b':
              	if(g_cursor_x - 9 < 1)
	    	{}
	      	else
                   	g_cursor_x-=9;
              	font = font_8x8_bspace;
         	break;
         case '\t':
              g_cursor_x+=0;
              font = font_8x8_bspace;
         break;
         default:
              font = font_8x8_question_mark;//Question mark for unknown
         break;
    }
}
