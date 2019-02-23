#include <string>
#include <map>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ncurses.h>
#include "Common.h"
#include "Mutex.h"
#include "Queue.h"
#include "Socket.h"
#include "Debug.h"
#include "Display.h"
#include "UdpClient.h"
#include "Thread.h"
#include "ClientThread.h"
#include "UserThread.h"

std::map<uint32_t, S_user>usermap;


int lx = 0; //local start x
int ly = 0; //local start y
dir ld = UP;//local direction
int rx = 0; //remote start x
int ry = 0; //remote start y
dir rd = DOWN;//remote direction

void UserThread::run()
{
    Display InsDisplay;
    _cols = COLS - 2;
    _lines = LINES - 2;
    create_bullet_list();                           /* 创建子弹链表 */
    InsDisplay.draw_map();							/* 绘制背景边框 */
    int t = 0;
    char ch = 0;
    char tempBuf[MAXITEMLENSIZE];
    lx = _cols/2;
    ly = _lines;									/* 初始位置 */
    while(GAME_EXIT != _game_state)
    {
        ch = InsDisplay.get_char();					/* 非阻塞读取输入 */
        switch (ch)
        {
        case 'w':									/* 上移光标 */
        case 'W':
            InsDisplay.mv_addch(ly, lx, ' ');
            if(ly > 1)ly--;
            ld = UP;
            InsDisplay.mv_addch(ly, lx, 'l');
            break;
        case 'a':									/* 左移光标 */
        case 'A':
            InsDisplay.mv_addch(ly, lx, ' ');
            if(lx > 1)lx--;
            ld = LEFT;
            InsDisplay.mv_addch(ly, lx, 'l');
            break;
        case 's':									/* 下移光标 */
        case 'S':
            InsDisplay.mv_addch(ly, lx, ' ');
            if(ly < _lines)ly++;
            ld = DOWN;
            InsDisplay.mv_addch(ly, lx, 'l');
            break;
        case 'd':									/* 右移光标 */
        case 'D':
            InsDisplay.mv_addch(ly, lx, ' ');
            if(lx < _cols)lx++;
            ld = RIGHT;
            InsDisplay.mv_addch(ly, lx, 'l');
            break;
        case 'j':									/* 发射子弹 */
        case 'J':
            insert_bullet_list(ly, lx, ld);			/* 插入子弹 */
            break;
        default:
            InsDisplay.mv_addch(ly, lx, 'l');
            break;
        }
        move_bullet_list(InsDisplay);				/* 显示移动之后的子弹 */
        memset(tempBuf, 0, MAXITEMLENSIZE);
        sprintf(tempBuf, "[location]ly:%2d, lx:%2d, ld:%2d", ly, lx, (int)ld);
        if(pSendQueue != NULL)
        {
		    pSendQueue->Queue_Put(tempBuf, sizeof(tempBuf));
        }
        InsDisplay.refresh();
        if(t == 20)
        {
            t = 0;
            updateUserMap();
        }
        t++;
    }
    destroy_bullet_list();
}

int UserThread::showUserMap(void *tmp)
{
	std::map<uint32_t, S_user>::iterator iter;
    Display *pInsDisp = (Display *)tmp;
    in_addr temp;
    for(iter = usermap.begin(); iter != usermap.end(); iter++)
    {
        temp.s_addr = iter->first;
        pInsDisp->add_print("%s", inet_ntoa(temp));
        //printf("[IP]:%s\n", );
    }
    return 0;
}

int UserThread::updateUserMap()						/* 更新用户列表 */
{
	std::map<uint32_t, S_user>::iterator iter;

    for(iter = usermap.begin(); iter != usermap.end(); iter++)
    {
        iter->second.user_time--;
        if(iter->second.user_time < 0)				/* 自动老化 */
        {
            usermap.erase(iter);
        }
    }
    return 0;
}

int UserThread::create_bullet_list()
{
    _head.next = NULL;
    return 0;
}

int UserThread::insert_bullet_list(int y, int x, int d)
{
    bullet_list_t *tmp = NULL;
    bullet_list_t *p = NULL;
    tmp = (bullet_list_t*)malloc(sizeof(bullet_list_t));
    if(NULL == tmp)
    {
        return -1;
    }
    tmp->data.x = x;
    tmp->data.y = y;
    tmp->data.d = d;
    tmp->next = NULL;
    for(p = &_head; p->next != NULL; p = p->next);	/* 遍历到队尾插入 */
    p->next = tmp;
    return 0;
}

int UserThread::move_bullet_list(Display& ins)
{
    bullet_list_t *p = NULL;
    bullet_list_t *q = NULL;
    for(p = &_head; p->next != NULL; )
    {
        ins.mv_addch(p->next->data.y, p->next->data.x, ' ');
        switch(p->next->data.d)						/* 根据子弹发射时的方向移动子弹 */
        {
            case 0:
            p->next->data.y--;
            break;
            case 1:
            p->next->data.x--;
            break;
            case 2:
            p->next->data.y++;
            break;
            case 3:
            p->next->data.x++;
            break;
            default:
            break;
        }
        if(p->next->data.x < 1 || p->next->data.x > _cols || p->next->data.y < 1 || p->next->data.y > _lines)
        {											/* 移动到边界的子弹从子弹链表中删除 */
            q = p->next->next;
            if(NULL != p->next)
            {
                free(p->next);
            }
            p->next = q;
            continue;
        }
        ins.mv_addch(p->next->data.y, p->next->data.x, 'o');//显示p->next
        p = p->next;
    }
    return 0;
}

int UserThread::destroy_bullet_list()				/* 删除子弹链表 */
{
    bullet_list_t *p = NULL;
    bullet_list_t *q = NULL;
    for(p = &_head; p->next != NULL;)
    {
        q = p->next->next;
        if(NULL != p->next)
        {
            free(p->next);
        }
        p->next = q;
    }
    return 0;
}