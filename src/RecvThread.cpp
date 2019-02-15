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
#include "RecvThread.h"

static char _recvBuf[MAXDATASIZE] = {0};
static Mutex* pRecvMux = new Mutex();

extern Socket* pUdpSock;
//extern void setLinkState(L_state s);

int pullRxQuene(char* buf, uint32_t len, uint32_t offset)
{
	pRecvMux->lock();
	if(offset + len > MAXDATASIZE)
	{
		pRecvMux->unlock();
		return -1;
	}
	memcpy(buf, _recvBuf + offset, len);
	pRecvMux->unlock();
	return 0;
}

int pushRxQuene(char* buf, uint32_t len, uint32_t offset)
{
	pRecvMux->lock();
	if(offset + len > MAXDATASIZE)
	{
		pRecvMux->unlock();
		return -1;
	}
	memcpy(_recvBuf + offset, buf, len);
	pRecvMux->unlock();
	return 0;
}

RecvThread::RecvThread()
{
	_pSock = pUdpSock;
	_buflen = MAXDATASIZE;
}

void RecvThread::run()
{
	char tempBuf[MAXDATASIZE] = {0};
//	while(GAME_EXIT != getGameState())
	{
		if(-1 == _pSock->readData(tempBuf, _buflen))
		{
			eprintf("error:%s %d",__FILE__, __LINE__);
//			setLinkState(LINK_ABORT);
		}
		if(0 != tempBuf[0])
		{
			pushRxQuene(tempBuf, MAXDATASIZE);
			bzero(tempBuf, MAXDATASIZE);
		}
		else
		{
//			setSignal(SIGN_EXIT);
		}
		msleep(50);
	}
}