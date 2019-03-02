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
#include "List.h"
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

in_addr serverAddr;

void UserThread::run()
{
    _game_state = GAME_SELECT;
    select_loop();
    fight_loop();
    gameover_loop();
}

int UserThread::showUserMap(Display& InsDisplay, int y, int x)
{
	std::map<uint32_t, S_user>::iterator iter;
    if(0 == usermap.size())
    {
        return -1;
    }
    in_addr temp;
    for(iter = usermap.begin(); iter != usermap.end(); iter++)
    {
        temp.s_addr = iter->first;
        InsDisplay.mv_printf(y++, x, "%s", inet_ntoa(temp));
    }
    return 0;
}

int UserThread::ageingUserMap()						/* 更新用户列表 */
{
	std::map<uint32_t, S_user>::iterator iter;

    for(iter = usermap.begin(); iter != usermap.end(); iter++)
    {
        iter->second.user_time--;
        if(0 == iter->second.user_time)				/* 自动老化 */
        {
            usermap.erase(iter);
        }
    }
    return 0;
}

int UserThread::select_loop()						/* 选择对方循环 */
{
    int t = 0;
    char ch = 0;
    Display InsDisplay;
    const int base_x = COLS/2 - 7;
    const int base_y = LINES/2;
    int lx = base_x;
    int ly = base_y;
    sock_item_t tempSock;
    showUserMap(InsDisplay, base_y, base_x);
    while(GAME_SELECT == _game_state)
    {
        std::map<uint32_t, S_user>::iterator iter = usermap.begin();
        InsDisplay.mv_addch(ly, lx - 2, '>');
        ch = InsDisplay.get_char();					/* 非阻塞读取输入 */
        switch (ch)
        {
        case 'w':									/* 上移光标 */
        case 'W':
            InsDisplay.mv_addch(ly, lx - 2, ' ');
            if(ly > base_y)
            {
                iter--;
                ly--;
            }
            InsDisplay.mv_addch(ly, lx - 2, '>');
            break;
        case 's':									/* 下移光标 */
        case 'S':
            InsDisplay.mv_addch(ly, lx - 2, ' ');
            if(ly - base_y < (int)usermap.size() - 1)
            {
                
                iter++;
                ly++;
            }
            InsDisplay.mv_addch(ly, lx - 2, '>');
            break;
        case 'j':									/* 向选中的ip发送对战请求 */
        case 'J':
            if(LINK_REQ == _link_state)
            {
                memset(&tempSock, 0, sizeof(sock_item_t));
                serverAddr.s_addr = iter->first;
                tempSock.psock = new UdpClient();
                tempSock.psock->init(inet_ntoa(serverAddr), 6789);
                tempSock.psock->setSocketNonblock();
                tempSock.data = (char *)malloc(fight_req.length());
                if(NULL == tempSock.data)
                {
                    eprintf("malloc error!\n");
                    break;
                }
                tempSock.len = fight_req.length();
                memcpy(tempSock.data, fight_req.c_str(), fight_req.length());
                if(NULL != pSendQueue)
                {
                    pSendQueue->Queue_Put(&tempSock, sizeof(sock_item_t));
                    _link_state = LINK_RSP;
                }
            }
            break;
        case 'y':									/* 确认对战请求 */
        case 'Y':
            if(LINK_WAIT == _link_state)
            {
                memset(&tempSock, 0, sizeof(sock_item_t));
                tempSock.psock = new UdpClient();
                tempSock.psock->init(inet_ntoa(serverAddr), 6789);
                tempSock.psock->setSocketNonblock();
                tempSock.data = (char *)malloc(fight_rsp.length());
                memset(tempSock.data, 0, fight_rsp.length());
                if(NULL == tempSock.data)
                {
                    eprintf("malloc error!\n");
                    return -1;
                }
                tempSock.len = fight_rsp.length();
                memcpy(tempSock.data, fight_rsp.c_str(), fight_rsp.length());
                if(NULL != pSendQueue)
                {
                    pSendQueue->Queue_Put(&tempSock, sizeof(sock_item_t));
                    _link_state = LINK_OK;
                    _game_state = GAME_FIGHT;
                }
            }
            break;
        case 'n':									/* 拒绝对战请求 */
        case 'N':
            if(LINK_WAIT == _link_state)
            {
                memset(&tempSock, 0, sizeof(sock_item_t));
                tempSock.psock = new UdpClient();
                tempSock.psock->init(inet_ntoa(serverAddr), 6789);
                tempSock.psock->setSocketNonblock();
                tempSock.data = (char *)malloc(fight_rej.length());
                memset(tempSock.data, 0, fight_rej.length());
                if(NULL == tempSock.data)
                {
                    eprintf("malloc error!\n");
                    return -1;
                }
                tempSock.len = fight_rej.length();
                memcpy(tempSock.data, fight_rej.c_str(), fight_rej.length());
                if(NULL != pSendQueue)
                {
                    pSendQueue->Queue_Put(&tempSock, sizeof(sock_item_t));
                    _link_state = LINK_REQ;
                }
            }
            break;
        default:
            break;
        }
        if(NULL != pRecvQueue && pRecvQueue->Queue_Count())
        {
            if(LINK_REQ == _link_state || LINK_RSP == _link_state)
            {
                pRecvQueue->Queue_Get(&tempSock, sizeof(sock_item_t));
                if(!memcmp(tempSock.data, fight_req.c_str(), fight_req.length()))
                {
                    serverAddr = tempSock.psock->getServerAddr().sin_addr;
                    _link_state = LINK_WAIT;
                }
                if(!memcmp(tempSock.data, fight_rsp.c_str(), fight_rsp.length()))
                {
                    if(serverAddr.s_addr == tempSock.psock->getServerAddr().sin_addr.s_addr)//如果接收到的rsp来自已发送req的对手
                    {
                        _link_state = LINK_OK;
                        _game_state = GAME_FIGHT;
                    }
                }
                if(NULL != tempSock.data)
                {
                    free(tempSock.data);
                }
                if(NULL != tempSock.psock)
                {
                    tempSock.psock->exit();
                    delete(tempSock.psock);
                }
            }
        }
        InsDisplay.refresh();
        if(t == 20)
        {
            t = 0;
            ageingUserMap();                        /* 1s */
            InsDisplay.clean();
            showUserMap(InsDisplay, base_y, base_x);
            //InsDisplay.refresh();
        }
        msleep(50);
        t++;
    }
    return 0;
}

int UserThread::fight_loop()
{
    int t = 0;
    char ch = 0;
    Display InsDisplay;
    _cols = COLS - 2;
    _lines = LINES - 2;
    int lx = _cols/2; //local start x
    int ly = _lines; //local start y
    dir ld = UP;//local direction
    int ls = 0; //local shoot
    int rx = _cols/2; //remote start x
    int ry = 1; //remote start y
    dir rd = DOWN;//remote direction
    int rs = 0; //remote shoot
    sock_item_t tempSock;
    _myself_bullet_list = new List();               /* 创建子弹链表 */
    _others_bullet_list = new List();
    InsDisplay.fight_map();							/* 绘制背景边框 */

    while(GAME_FIGHT == _game_state)
    {
        ls = 0;
        ch = InsDisplay.get_char();					/* 非阻塞读取输入 */
        LNode_t *s = NULL;
        switch (ch)
        {
        case 'w':									/* 上移光标 */
        case 'W':
            InsDisplay.mv_addins(ly, lx, ld, ' ');
            if(ly > 1)ly--;
            ld = UP;
            break;
        case 'a':									/* 左移光标 */
        case 'A':
            InsDisplay.mv_addins(ly, lx, ld, ' ');
            if(lx > 1)lx--;
            ld = LEFT;
            break;
        case 's':									/* 下移光标 */
        case 'S':
            InsDisplay.mv_addins(ly, lx, ld, ' ');
            if(ly < _lines)ly++;
            ld = DOWN;
            break;
        case 'd':									/* 右移光标 */
        case 'D':
            InsDisplay.mv_addins(ly, lx, ld, ' ');
            if(lx < _cols)lx++;
            ld = RIGHT;
            break;
        case 'j':									/* 发射子弹 */
        case 'J':
            ls = 1;
            s = (LNode_t *)malloc(sizeof(LNode_t));
            s->data.x = lx;
            s->data.y = ly;
            s->data.d = ld;
            _myself_bullet_list->List_Insert_After(_myself_bullet_list->List_GetTail(), s); /* 插入子弹 */
            break;
        default:
            break;
        }
        InsDisplay.mv_addins(ly, lx, ld, 254);           /* 显示移动之后的光标 */

        if(pRecvQueue != NULL)
        {
            if(0 != pRecvQueue->Queue_Count())
            {
                pRecvQueue->Queue_Get(&tempSock, sizeof(sock_item_t));
                /* 解析数据包 */
                ry = strtol(tempSock.data + 3, NULL, 0);
                rx = strtol(tempSock.data + 8, NULL, 0);
                rd = (dir)strtol(tempSock.data + 13, NULL, 0);
                rs = strtol(tempSock.data + 18, NULL, 0);
                if(1 == rs)
                {
                    LNode_t *s = (LNode_t *)malloc(sizeof(LNode_t));
                    s->data.x = rx;
                    s->data.y = ry;
                    s->data.d = rd;
                    _others_bullet_list->List_Insert_After(_others_bullet_list->List_GetTail(), s); /* 插入子弹 */
                }
                if(NULL != tempSock.data)
                {
                    free(tempSock.data);
                }
                if(NULL != tempSock.psock)
                {
                    tempSock.psock->exit();
                    delete(tempSock.psock);
                }
            }
        }

        InsDisplay.mv_addch(ry, rx, 'r');           /* 显示移动之后的光标 */
        move_myself_bullet_list(InsDisplay);				/* 显示移动之后的子弹 */
        move_others_bullet_list(InsDisplay);				/* 显示移动之后的子弹 */
        if(pSendQueue != NULL)
        {
            tempSock.psock = new UdpClient();
            tempSock.psock->init(inet_ntoa(serverAddr), 6789);
            tempSock.psock->setSocketNonblock();
            tempSock.data = (char *)malloc(MAXITEMLENSIZE);
            if(NULL == tempSock.data)
            {
                eprintf("malloc error!\n");
                break;
            }
            memset(tempSock.data, 0, MAXITEMLENSIZE);
            sprintf(tempSock.data, "ly:%2d, lx:%2d, ld:%2d, ls:%2d", ly, lx, (int)ld, ls);
            pSendQueue->Queue_Put(&tempSock, sizeof(sock_item_t));
        }
        InsDisplay.refresh();
        
        if(t == 20)
        {
            t = 0;
            ageingUserMap();                        /* 1s */
        }
        msleep(50);
        t++;
    }
    delete _myself_bullet_list;
    delete _others_bullet_list;
    return 0;
}

int UserThread::gameover_loop(void)
{
    return 0;
}

int UserThread::move_myself_bullet_list(Display& ins)
{
    LNode_t *p = NULL;
    LNode_t *q = NULL;
    for(p = _myself_bullet_list->List_GetHead(); p->next != NULL; )
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


int UserThread::move_others_bullet_list(Display& ins)
{
    LNode_t *p = NULL;
    LNode_t *q = NULL;
    for(p = _others_bullet_list->List_GetHead(); p->next != NULL; )
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

