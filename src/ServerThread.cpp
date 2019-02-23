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
#include "Display.h"
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

	while(GAME_EXIT != _game_state)
	{
		if(-1 != _pInsUdp->recvData(tempBuf, MAXITEMLENSIZE))
		{
			//printf("recvfrom [%s]:%s\n", inet_ntoa(_pInsUdp->getClientAddr().sin_addr), tempBuf);
			if(!memcmp("I'm at here!", tempBuf, 16))
			{	/* broadcast */
				updateUserMap(_pInsUdp->getClientAddr());
			}
			else
			{	/* unicast */
				pRecvQueue->Queue_Put(tempBuf, MAXITEMLENSIZE);
			}
		}
		
		memset(tempBuf, 0, MAXITEMLENSIZE);
		msleep(50);
	}
}

int ServerThread::updateUserMap(sockaddr_in clientAddr)
{
	S_user temp;
	temp.user_time = 3;
	std::map<uint32_t, S_user>::iterator iter;

    iter = usermap.find(clientAddr.sin_addr.s_addr);  
  
    if(iter != usermap.end())  
	{
		//update time
		iter->second.user_time = temp.user_time;
	}
    else  
	{
		//insert
		usermap.insert(std::pair<uint32_t, S_user>(clientAddr.sin_addr.s_addr, temp));
	}
	return 0;
}