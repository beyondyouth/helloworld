#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <ctype.h>
#include "Display.h"

#define GRASS     ' '
#define EMPTY     '.'
#define WATER     '~'
#define MOUNTAIN  '^'
#define PLAYER    '*'

#define TTY_PATH            "/dev/tty"
#define STTY_US             "stty raw -echo -F "
#define STTY_DEF            "stty -raw echo -F "

Display::Display()
{
	initscr();
	cbreak();				/* 行缓冲禁止，传递所有控制信息 */
	noecho();				/* 禁止输入回显 */
	
	keypad(stdscr, TRUE);	/* 允许使用功能键 */
	//curs_set(0);			/* 光标不可显 */

    //system(STTY_US TTY_PATH);

	clear();
}

Display::~Display()
{
	endwin();
}

void Display::refresh()
{
	refresh();
}

void Display::draw_map()
{
    int y, x;
    /* 绘制探索地图 */
    /* 背景 */
    for (y = 0; y < LINES; y++) {
        mvhline(y, 0, GRASS, COLS);
    }
#if 1
    /* 山和山道 */
    for (x = COLS / 2; x < COLS * 3 / 4; x++) {
        mvvline(0, x, MOUNTAIN, LINES);
    }
    mvhline(LINES / 4, 0, GRASS, COLS);
    /* 湖 */
    for (y = 1; y < LINES / 2; y++) {
        mvhline(y, 1, WATER, COLS / 3);
    }
#endif
}

int Display::get_char()
{
    fd_set rfds;
    struct timeval tv;
    int ch = 0;

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 20; //设置等待超时时间

    //检测键盘是否有输入
    if (select(1, &rfds, NULL, NULL, &tv) > 0)
	{
        ch = getch();
    }
    return ch;
}

int Display::move(int y, int x)
{
	return move(y, x);
}

int Display::moveaddch(int y, int x, char ch)
{
	return mvaddch(y, x, ch);
}

int Display::movedelch(int y, int x)
{
	return mvdelch(y, x);
}

int Display::addprintf(char *str)
{
    return printw("%s", str);
}
