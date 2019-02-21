#include <string>
#include <map>
#include <stdint.h>
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>
#include <ctype.h>
#include "Debug.h"
#include "DisPlay.h"
#include "Thread.h"
#include "UserThread.h"

std::map<uint32_t, S_user>usermap;

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
    system(STTY_US TTY_PATH);
    refresh();
//	mvwprintw(testwin, 1, 1, "r");
//    InsDisplay.destroy_win(testwin);
    int key = 0;
    int t = 0;
    for(;;) {
        key = get_char();
        switch(key) {
            case KEY_RIGHT:
            mvprintw(0, 30, "%s", "KEY_RIGHT");
            break;
            case KEY_LEFT:
            mvprintw(0, 30, "%s", "KEY_LEFT");
            break;
            case KEY_UP:
            mvprintw(0, 30, "%s","KEY_UP");
            break;
            case KEY_DOWN:
            mvprintw(0, 30, "%s","KEY_DOWN");
            break;
            case 3:
            system(STTY_DEF TTY_PATH);
            return;
            default:
            mvprintw(0, 30, "Unmached %3d",key);
            break;
        }
        mvprintw(0, 50, "key %3d",key);
        msleep(50);
        t++;
        if(t == 20)
        {
            t = 0;
            updateUserMap();
        }
    }
}

int UserThread::get_char()
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

int UserThread::updateUserMap()
{
	std::map<uint32_t, S_user>::iterator iter;

    for(iter = usermap.begin(); iter != usermap.end(); iter++)
    {
        iter->second.user_time--;
        if(iter->second.user_time < 0)
        {
            usermap.erase(iter);
        }
    }
    return 0;
}