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

    timeout(50);			/* 输入阻塞50ms */
    //nodelay(stdscr, true);  /* 输入非阻塞 */

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

void Display::draw_map()
{
    box(stdscr, 0, 0);
}

int Display::get_char()
{
    return getch();
}

int Display::mv_addch(int y, int x, char ch)
{
	return mvaddch(y, x, ch);
}

void Display::add_print(const char *cmd, ...)
{
    va_list args;       //定义一个va_list类型的变量，用来储存单个参数
    va_start(args,cmd); //使args指向可变参数的第一个参数
    printw(cmd,args);   //必须用vprintf等带V的
    va_end(args);       //结束可变参数的获取
}
