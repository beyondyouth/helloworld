#include <string>
#include <map>
#include <stdint.h>
#include <syslog.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Common.h"
#include "Debug.h"
#include "Display.h"
#include "Thread.h"
#include "UserThread.h"

std::map<uint32_t, S_user>usermap;

uint32_t lx = 0; //local start x
uint32_t ly = 0; //local start y
uint32_t rx = 0; //remote start x
uint32_t ry = 0; //remote start y

void UserThread::run()
{
    Display InsDisplay;
    InsDisplay.draw_map();
    InsDisplay.refresh();
    int t = 0;
    char ch = 0;
    while(GAME_EXIT != _game_state)
    {
        ch = InsDisplay.get_char();
        if(3 == ch)
        {
            _game_state = GAME_EXIT;
            break;
        }

        if(t == 20)
        {
            t = 0;
            updateUserMap();
        }
        msleep(50);
        t++;
    }
}

int UserThread::showUserMap(void *tmp)
{
	std::map<uint32_t, S_user>::iterator iter;
    Display *pInsDisp = (Display *)tmp;
    in_addr temp;
    for(iter = usermap.begin(); iter != usermap.end(); iter++)
    {
        temp.s_addr = iter->first;
        pInsDisp->addprintf(inet_ntoa(temp));
        //printf("[IP]:%s\n", );
    }
    return 0;
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
