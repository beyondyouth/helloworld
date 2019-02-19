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
#include "SendThread.h"


SendThread::SendThread()
{
	_buflen = MAXITEMLENSIZE;
	pSendQueue = new Queue(MAXQUEUELENGTH, MAXITEMLENSIZE);
	UdpClient* pInsUdp = new UdpClient();
	_pSock = (Socket*)pInsUdp;
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

	_pSock->init(6789);
	_pSock->setSocketNonblock();
	_pSock->sendData("123456", 6);
	//printf("send:%s\n", "123456");

	UdpClient pHeart;
	
#if 1
	int ii = 0;
	while(GAME_EXIT != _game_state)
	{
		if(ii == 20)
		{
			pHeart.sendBroadcast(6789, "I'm at here!\n", 16); /* fa song heart */
			ii = 0;
		}
//		if(0 != pSendQueue->Queue_Count())
		{
			//pSendQueue->Queue_Get(tempBuf, MAXITEMLENSIZE);
			memcpy(tempBuf, "123456", 6);
			_pSock->sendData(tempBuf, 6);
			memset(tempBuf, 0, MAXITEMLENSIZE);
		}
		msleep(50);
		ii++;
		
	}
#endif
}