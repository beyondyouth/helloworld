#include <syslog.h>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>
#include <ctype.h>
#include "Debug.h"
#include "DisPlay.h"
#include "Thread.h"
#include "UserThread.h"

uint32_t lx = 0; //local start x
uint32_t ly = 0; //local start y
uint32_t rx = 0; //remote start x
uint32_t ry = 0; //remote start y

void UserThread::run()
{
    DisPlay InsDisplay;
    InsDisplay.create_fightwin();

    mvprintw(0, 0, "l X:%2d Y:%2d", lx, ly);
    mvprintw(1, 0, "r X:%2d Y:%2d", rx, ry);
    refresh();
//	mvwprintw(testwin, 1, 1, "r");
//    InsDisplay.destroy_win(testwin);
    sleep(5);
}