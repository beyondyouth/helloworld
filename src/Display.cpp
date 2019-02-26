#include <stdarg.h>
#include <ncurses.h>
#include <ctype.h>
#include "Display.h"

#define GRASS     ' '
#define EMPTY     '.'
#define WATER     '~'
#define MOUNTAIN  '^'
#define PLAYER    '*'


Display::Display()
{
	initscr();
	cbreak();				/* 行缓冲禁止，传递所有控制信息 */
	noecho();				/* 禁止输入回显 */
	
	keypad(stdscr, TRUE);	/* 允许使用功能键 */
	curs_set(0);			/* 光标不可显 */

    timeout(0);				/* 输入阻塞0ms */

	clear();
}

Display::~Display()
{
	endwin();
}

void Display::refresh()
{
	wrefresh(stdscr);
}

void Display::fight_map()
{
    box(stdscr, 0, 0);
}

int Display::get_char()
{
    return getch();
}

int Display::mv(int y, int x)
{
	return mvwin(stdscr, y, x);
}

int Display::mv_addch(int y, int x, char ch)
{
	return mvaddch(y, x, ch);
}

int Display::mv_addins(int y, int x, int d, char ch)
{
	mvaddch(y, x, ch);
	switch(d)
	{
		case 0:
		mvaddch(y-1, x, ch);
		break;
		case 1:
		mvaddch(y, x-1, ch);
		break;
		case 2:
		mvaddch(y+1, x, ch);
		break;
		case 3:
		mvaddch(y, x+1, ch);
		break;
		default:
		break;
	}
	return 0;
}

void Display::add_print(const char *cmd, ...)
{
    va_list args;       //定义一个va_list类型的变量，用来储存单个参数
    va_start(args,cmd); //使args指向可变参数的第一个参数
    printw(cmd,args);   //必须用vprintf等带V的
    va_end(args);       //结束可变参数的获取
}
