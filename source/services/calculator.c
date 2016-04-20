/* Copyright (c) 2020, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	CALCULATOR
*/
#include <gui/window.h>
#include <gui/gfx.h>

extern volatile window_item_t item_list[];
extern volatile window_t current_window;
extern volatile int item_id;

int a, b = 0;
bool seta, setb;
char *na ="";
char *nb = "";
char todo;
char *_result;
int result;
static int calc_id;

void calculator_print(char *num)
{
	for(int i=1;i<item_id; i++)
	{
		if(item_list[i].parent_id == calc_id && item_list[i].width == 135) 
		{
			item_list[i].name = num;
			break;
		}
	}
}

void calculator_calculate()
{
	a = atoi(na);
	b = atoi(nb);

	switch(todo)
	{
		case 'p':
			result = a + b;
			break;
		case 'm':
			result = a - b;
			break;
		case 'd':
			result = a / b;
			break;
		case '*':
			result = a * b;
			break;
	}

	itoa(_result, 10, result);
	calculator_print(_result);
	a = 0;
	b = 0;
	result = 0;
        na = "";
        nb = "";
}

void press_calculator_key(int button)
{	
	if(current_window.id != calc_id)
		return;
	switch(button)
	{
		case 1:
			if(!seta)
			{
				na = '1';
				calculator_print(na);
			}
			else
			{
				nb = '1';
				calculator_print(nb);
				
			}
			break;
		case 2:
			if(!seta)
			{
				nb = '2';
				calculator_print(na);
				
			}
			else
			{
				nb = '2';
				calculator_print(nb);
				
			}
			break;
		case 3:
			if(!seta)
			{
				na = '3';
				calculator_print(na);
				
			}
			else
			{
				nb = '3';
				calculator_print(nb);
				
			}
			break;
		case 4:
			if(!seta)
			{
				na = '4';
				calculator_print(na);
				
			}
			else
			{
				nb = '4';
				calculator_print(nb);
				
			}
			break;
		case 5:
			if(!seta)
			{
				na = '5';
				calculator_print(na);
				
			}
			else
			{
				nb = '5';
				calculator_print(nb);
				
			}
			break;
		case 6:
			if(!seta)
			{
				na = '6';
				calculator_print(na);
				
			}
			else
			{
				nb = '6';
				calculator_print(nb);
				
			}
			break;
		case 7:
			if(!seta)
			{
				na = '7';
				calculator_print(na);
				
			}
			else
			{
				nb = '7';
				calculator_print(nb);
				
			}
			break;
		case 8:
			if(!seta)
			{
				na = '8';
				calculator_print(na);
				
			}
			else
			{
				nb = '8';
				calculator_print(nb);
				
			}
			break;
		case 9:
			if(!seta)
			{
				na = '9';
				calculator_print(na);
				
			}
			else
			{
				nb =  + '9';
				calculator_print(nb);
				
			}
			break;
		case 10:
			if(!seta)
			{
				na = '0';
				calculator_print(na);
				
			}
			else
			{
				nb = '0';
				calculator_print(nb);
				
			}
			break;
		case 14:
			todo = 'p';
			if(!seta)
			{
				seta = true;
			}
			break;
		case 15:
			todo = 'm';
			if(!seta)
			{
				seta = true;
			}
			break;
		case 16:
			todo = 'd';
			if(!seta)
			{
				seta = true;
			}
			break;
		case 17:
			todo = '*';
			if(!seta)
			{
				seta = true;
			}
			break;
		case 13:	
			if(seta)
				calculator_calculate();
			break;				
	}
}

void init_calculator()
{
	calc_id = create_window(150, 150, "Calculator",  COLOR_BACKGROUND, COLOR_BLACK, 1);
        window_add_item(5, 20, 135, 20, COLOR_WHITE, COLOR_MENUBAR, "0.", calc_id, 0, 0); 
        window_add_item(5, 45, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "1", calc_id, press_calculator_key, 1);
        window_add_item(30, 45, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "2", calc_id, press_calculator_key, 2);
        window_add_item(55, 45, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "3", calc_id, press_calculator_key, 3);
        window_add_item(80, 45, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "4", calc_id, press_calculator_key, 4);
        window_add_item(105, 45, 30, 20, COLOR_WHITE, COLOR_MENUBAR, "[<]", calc_id, press_calculator_key, 11);
        window_add_item(5, 70, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "5", calc_id, press_calculator_key, 5);
        window_add_item(30, 70, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "6", calc_id, press_calculator_key, 6);
        window_add_item(55, 70, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "7", calc_id, press_calculator_key, 7);
        window_add_item(80, 70, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "8", calc_id, press_calculator_key, 8);
        window_add_item(5, 95, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "9", calc_id, press_calculator_key, 9);
        window_add_item(30, 95, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "0", calc_id, press_calculator_key, 10);
        window_add_item(55, 95, 20, 20, COLOR_WHITE, COLOR_MENUBAR, ".", calc_id, press_calculator_key, 12);
        window_add_item(80, 95, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "=", calc_id, press_calculator_key, 13);
        window_add_item(5, 120, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "+", calc_id, press_calculator_key, 14);
        window_add_item(30, 120, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "-", calc_id, press_calculator_key, 15);
        window_add_item(55, 120, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "/", calc_id, press_calculator_key, 16);
        window_add_item(80, 120, 20, 20, COLOR_WHITE, COLOR_MENUBAR, "*", calc_id, press_calculator_key, 17);
}
