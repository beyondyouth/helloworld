#include <string>
#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "Debug.h"
#include "Socket.h"
#include "Thread.h"
#include "Mutex.h"
#include "Queue.h"
#include "Common.h"
#include "RecvThread.h"

extern Socket* pUdpSock;
//extern void setLinkState(L_state s);


RecvThread::RecvThread()
{
	_pSock = pUdpSock;
	_buflen = MAXITEMLENSIZE;
	pRecvQueue = new Queue(MAXQUEUELENGTH, MAXITEMLENSIZE);
}

RecvThread::~RecvThread()
{
	if(NULL != pRecvQueue)
	{
		delete(pRecvQueue);
	}
}

void RecvThread::run()
{
	char tempBuf[MAXITEMLENSIZE] = {0};
	while(GAME_EXIT != _game_state)
	{
		if(-1 == _pSock->readData(tempBuf, _buflen))
		{
			eprintf("error:%s %d",__FILE__, __LINE__);
			_link_state = LINK_ABORT;
		}
		if(0 != pRecvQueue->Queue_Count())
		{
			pRecvQueue->Queue_Put(tempBuf, MAXITEMLENSIZE);
			memset(tempBuf, 0, MAXITEMLENSIZE);
		}
		msleep(50);
	}
}