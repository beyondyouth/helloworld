#include <stdio.h>
#include <string>
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
#include "UdpClient.h"
#include "SendThread.h"


Socket* pUdpSock = NULL;


SendThread::SendThread()
{
	_pSock = pUdpSock;
	_buflen = MAXITEMLENSIZE;
	pSendQueue = new Queue(MAXQUEUELENGTH, MAXITEMLENSIZE);
}

SendThread::~SendThread()
{
	if(NULL != pSendQueue)
	{
		delete(pSendQueue);
	}
}

void SendThread::run()
{
	char tempBuf[MAXITEMLENSIZE] = {0};

	UdpClient* pInsUdp = new UdpClient();
	pUdpSock = (Socket*)pInsUdp;

	//while(GAME_EXIT != getGameState())
	{
		if(0 != pSendQueue->Queue_Count())
		{
			pSendQueue->Queue_Get(tempBuf, MAXITEMLENSIZE);
			if(-1 == _pSock->writeData(tempBuf, _buflen))
			{
				eprintf("error:%s %d",__FILE__, __LINE__);
			}
			memset(tempBuf, 0, MAXITEMLENSIZE);
		}
		msleep(50);
	}
}