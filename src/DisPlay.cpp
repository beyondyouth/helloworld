#include <ncurses.h>
#include <ctype.h>
#include "DisPlay.h"

DisPlay::DisPlay()
{
	initscr();
	cbreak();/*行缓冲禁止，传递所有控制信息*/
	keypad(stdscr, TRUE);/*程序需要使用F1功能键*/
	curs_set(0);
}

DisPlay::~DisPlay()
{
    refresh();
	endwin();
}

int DisPlay::check_ncurse()
{
    if(LINES < 20 || COLS < 20)
        return -1;
}
WINDOW *DisPlay::create_win(int height,int width,int starty,int startx)
{
	WINDOW* local_win;
	local_win = newwin(height,width,starty,startx);
	box(local_win, 0, 0);/*0,0是字符默认的行列起始位置*/
	wrefresh(local_win);/*刷新窗口缓冲，显示box*/
	return local_win;
}

void DisPlay::destroy_win(WINDOW* local_win)
{
	wborder(local_win,' ',' ',' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}

void DisPlay::create_fightwin()
{
    int startx, starty, width, height;
	height = LINES - 2;
	width = COLS;
	starty = (LINES-height)/2 + 1;/*计算窗口中心位置的行数*/
	startx = (COLS-width)/2;/*计算窗口中心位置的列数*/
//	printw("Press F1 to exit");
	refresh();
	fightwin = create_win(height,width,starty,startx);
}