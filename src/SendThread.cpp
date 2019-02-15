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
#include "UdpClient.h"
#include "SendThread.h"


static char _sendBuf[MAXDATASIZE] = {0};
static Mutex* pSendMux = new Mutex();

Socket* pUdpSock = NULL;

int pushTxQuene(char* buf, uint32_t len, uint32_t offset)
{
	pSendMux->lock();
	if(offset + len > MAXDATASIZE)
	{
		pSendMux->unlock();
		return -1;
	}
	
	memcpy(_sendBuf + offset, buf, len);
	pSendMux->unlock();
	return 0;
}


int pullTxQuene(char* buf, uint32_t len, uint32_t offset)
{
	pSendMux->lock();
	if(offset + len > MAXDATASIZE)
	{
		pSendMux->unlock();
		return -1;
	}
	
	memcpy(buf, _sendBuf + offset, len);
	pSendMux->unlock();
	return 0;
}

SendThread::SendThread()
{
	_pSock = pUdpSock;
	_buflen = MAXDATASIZE;
}

void SendThread::run()
{
	char tempBuf[MAXDATASIZE] = {0};

	UdpClient* pInsUdp = new UdpClient();
	pUdpSock = (Socket*)pInsUdp;

	//while(GAME_EXIT != getGameState())
	{
		if(0 != _sendBuf[0])
		{
			pullTxQuene(tempBuf, MAXDATASIZE);
			if(-1 == _pSock->writeData(tempBuf, _buflen))
			{
				eprintf("error:%s %d",__FILE__, __LINE__);
			}
			bzero(tempBuf, MAXDATASIZE);
		}
		msleep(50);
	}
}