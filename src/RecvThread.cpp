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
#include "UdpServer.h"
#include "RecvThread.h"


//extern void setLinkState(L_state s);


RecvThread::RecvThread()
{
	_buflen = MAXITEMLENSIZE;
	pRecvQueue = new Queue(MAXQUEUELENGTH, MAXITEMLENSIZE);
	UdpServer* pInsUdp = new UdpServer();
	_pSock = (Socket*)pInsUdp;
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
	_pSock->init(6789);
	_pSock->setSocketBlock();

	while(1) {
		_pSock->recvData(tempBuf, 6);
		if(tempBuf[0] > 0)
			printf("recv:%s\n", tempBuf);
		memset(tempBuf, 0, 6);
		msleep(50);
	}
#if 0
	while(GAME_EXIT != _game_state)
	{
		if(-1 == _pSock->recvData(tempBuf, _buflen))
		{
			eprintf("error:%s %d",__FILE__, __LINE__);
			_link_state = LINK_ABORT;
			continue;
		}
		pRecvQueue->Queue_Put(tempBuf, MAXITEMLENSIZE);
		memset(tempBuf, 0, MAXITEMLENSIZE);

		msleep(50);
	}
#endif
}