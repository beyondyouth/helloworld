#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
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
#include "UserThread.h"
#include "ServerThread.h"


//extern void setLinkState(L_state s);


ServerThread::ServerThread()
{
	_buflen = MAXITEMLENSIZE;
	pRecvQueue = new Queue(MAXQUEUELENGTH, MAXITEMLENSIZE);
	_pInsUdp = new UdpServer();
}

ServerThread::~ServerThread()
{
	if(NULL != _pInsUdp)
	{
		delete(_pInsUdp);
	}
	if(NULL != pRecvQueue)
	{
		delete(pRecvQueue);
	}
}

void ServerThread::run()
{
	char tempBuf[MAXITEMLENSIZE] = {0};
	_pInsUdp->init(6789);
	_pInsUdp->setSocketBlock();

	while(1) {
		if(-1 == _pInsUdp->recvData(tempBuf, MAXITEMLENSIZE))
		{

		}
		else
		{
			printf("recvfrom [%s]:%s\n", inet_ntoa(_pInsUdp->getClientAddr().sin_addr), tempBuf);
            S_user* temp = (S_user*)malloc(sizeof(S_user));
            temp->user_addr = _pInsUdp->getClientAddr().sin_addr.s_addr;
            memcpy(temp->user_name, "unknown", sizeof("unknown"));
            temp->user_time = 1;
            mapuser.insert(std::pair<int, S_user>(1, *temp));
		}
		
		
		memset(tempBuf, 0, MAXITEMLENSIZE);
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
