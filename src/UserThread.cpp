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
#include "UdpServer.h"
#include "Thread.h"
#include "ClientThread.h"
#include "ServerThread.h"
#include "UserThread.h"

std::map<uint32_t, S_user>usermap;


int lx = 0; //local start x
int ly = 0; //local start y
dir ld = UP;//local direction
int ls = 0; //local shoot
int rx = 0; //remote start x
int ry = 0; //remote start y
dir rd = DOWN;//remote direction
int rs = 0; //remote shoot

void UserThread::run()
{
    Display InsDisplay;
    _cols = COLS - 2;
    _lines = LINES - 2;
    create_myself_bullet_list();                    /* 创建子弹链表 */
    create_others_bullet_list();
    InsDisplay.draw_map();							/* 绘制背景边框 */
    int t = 0;
    char ch = 0;
    char tempBuf[MAXITEMLENSIZE];
    lx = _cols/2;
    ly = _lines;									/* 初始位置 */
    rx = _cols/2;
    ry = 1;
    while(GAME_EXIT != _game_state)
    {
        ls = 0;
        ch = InsDisplay.get_char();					/* 非阻塞读取输入 */
        switch (ch)
        {
        case 'w':									/* 上移光标 */
        case 'W':
            InsDisplay.mv_addch(ly, lx, ' ');
            if(ly > 1)ly--;
            ld = UP;
            break;
        case 'a':									/* 左移光标 */
        case 'A':
            InsDisplay.mv_addch(ly, lx, ' ');
            if(lx > 1)lx--;
            ld = LEFT;
            break;
        case 's':									/* 下移光标 */
        case 'S':
            InsDisplay.mv_addch(ly, lx, ' ');
            if(ly < _lines)ly++;
            ld = DOWN;
            break;
        case 'd':									/* 右移光标 */
        case 'D':
            InsDisplay.mv_addch(ly, lx, ' ');
            if(lx < _cols)lx++;
            ld = RIGHT;
            break;
        case 'j':									/* 发射子弹 */
        case 'J':
            ls = 1;
            insert_myself_bullet_list(ly, lx, ld);			/* 插入子弹 */
            break;
        default:
            break;
        }
        InsDisplay.mv_addch(ly, lx, 'l');           /* 显示移动之后的光标 */

        if(pRecvQueue != NULL)
        {
            if(0 != pRecvQueue->Queue_Count())
            {
                memset(tempBuf, 0, MAXITEMLENSIZE);
                pRecvQueue->Queue_Get(tempBuf, MAXITEMLENSIZE);
                /* 解析数据包 */
                ry = strtol(tempBuf + 3, NULL, 0);
                rx = strtol(tempBuf + 8, NULL, 0);
                rd = (dir)strtol(tempBuf + 13, NULL, 0);
                rs = strtol(tempBuf + 18, NULL, 0);
                if(1 == rs)
                {
                    insert_others_bullet_list(ry, rx, rd);
                }
            }
        }

        InsDisplay.mv_addch(ry, rx, 'r');           /* 显示移动之后的光标 */
        move_myself_bullet_list(InsDisplay);				/* 显示移动之后的子弹 */
        move_others_bullet_list(InsDisplay);				/* 显示移动之后的子弹 */
        if(pSendQueue != NULL)
        {
            memset(tempBuf, 0, MAXITEMLENSIZE);
            sprintf(tempBuf, "ly:%2d, lx:%2d, ld:%2d, ls:%2d", ly, lx, (int)ld, ls);
		    pSendQueue->Queue_Put(tempBuf, sizeof(tempBuf));
        }
        InsDisplay.refresh();
        if(t == 20)
        {
            t = 0;
            updateUserMap();                        /* 1s */
        }
        t++;
    }
    destroy_myself_bullet_list();
    destroy_others_bullet_list();
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

int UserThread::create_myself_bullet_list()
{
    _myself_bullet_list_head.next = NULL;
    return 0;
}

int UserThread::insert_myself_bullet_list(int y, int x, dir d)
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
    for(p = &_myself_bullet_list_head; p->next != NULL; p = p->next);	/* 遍历到队尾插入 */
    p->next = tmp;
    return 0;
}

int UserThread::move_myself_bullet_list(Display& ins)
{
    bullet_list_t *p = NULL;
    bullet_list_t *q = NULL;
    for(p = &_myself_bullet_list_head; p->next != NULL; )
    {
        ins.mv_addch(p->next->data.y, p->next->data.x, ' ');
        switch(p->next->data.d)						/* 根据子弹发射时的方向移动子弹 */
        {
            case UP:
            p->next->data.y-=2;						/* 子弹移动速度为光标移速两倍 */
            break;
            case LEFT:
            p->next->data.x-=2;
            break;
            case DOWN:
            p->next->data.y+=2;
            break;
            case RIGHT:
            p->next->data.x+=2;
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

int UserThread::destroy_myself_bullet_list()				/* 删除子弹链表 */
{
    bullet_list_t *p = NULL;
    bullet_list_t *q = NULL;
    for(p = &_myself_bullet_list_head; p->next != NULL;)
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

int UserThread::create_others_bullet_list()
{
    _others_bullet_list_head.next = NULL;
    return 0;
}

int UserThread::insert_others_bullet_list(int y, int x, dir d)
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
    for(p = &_others_bullet_list_head; p->next != NULL; p = p->next);	/* 遍历到队尾插入 */
    p->next = tmp;
    return 0;
}

int UserThread::move_others_bullet_list(Display& ins)
{
    bullet_list_t *p = NULL;
    bullet_list_t *q = NULL;
    for(p = &_others_bullet_list_head; p->next != NULL; )
    {
        ins.mv_addch(p->next->data.y, p->next->data.x, ' ');
        switch(p->next->data.d)						/* 根据子弹发射时的方向移动子弹 */
        {
            case UP:
            p->next->data.y-=2;						/* 子弹移动速度为光标移速两倍 */
            break;
            case LEFT:
            p->next->data.x-=2;
            break;
            case DOWN:
            p->next->data.y+=2;
            break;
            case RIGHT:
            p->next->data.x+=2;
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

int UserThread::destroy_others_bullet_list()				/* 删除子弹链表 */
{
    bullet_list_t *p = NULL;
    bullet_list_t *q = NULL;
    for(p = &_others_bullet_list_head; p->next != NULL;)
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
