#include <stdio.h>
#include <string>
#include <syslog.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "Debug.h"
#include "Common.h"
#include "Socket.h"
#include "Thread.h"
#include "Mutex.h"
#include "Queue.h"
#include "UdpClient.h"
#include "ClientThread.h"


ClientThread::ClientThread()
{
	_buflen = MAXITEMLENSIZE;
	pSendQueue = new Queue(MAXQUEUELENGTH, MAXITEMLENSIZE);
	_pInsUdp = new UdpClient();
}

ClientThread::~ClientThread()
{
	if(NULL != _pInsUdp)
	{
		delete(_pInsUdp);
	}
	if(NULL != pSendQueue)
	{
		delete(pSendQueue);
	}
}

void ClientThread::run()
{
	char tempBuf[MAXITEMLENSIZE] = {0};

	_pInsUdp->init(6789);
	_pInsUdp->setSocketNonblock();

	UdpClient pHeart;
	
#if 1
	int ii = 0;
	while(GAME_EXIT != _game_state)
	{
		if(ii == 20)
		{
			pHeart.sendBroadcast(6789, "I'm at here!", 16); /* fa song heart */
			ii = 0;
		}
		if(0 != pSendQueue->Queue_Count())
		{
			pSendQueue->Queue_Get(tempBuf, MAXITEMLENSIZE);
			//memcpy(tempBuf, "123456", 6);
			_pInsUdp->sendData(tempBuf, strlen(tempBuf));
			memset(tempBuf, 0, MAXITEMLENSIZE);
		}
		msleep(50);
		ii++;
		
	}
#endif
}