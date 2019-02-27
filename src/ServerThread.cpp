#include <string>
#include <map>
#include <stdlib.h>
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


Queue *pRecvQueue = NULL;


ServerThread::ServerThread()
{
	pRecvQueue = new Queue(MAXQUEUELENGTH, sizeof(sock_item_t));
}

ServerThread::~ServerThread()
{
	if(NULL != pRecvQueue)
	{
		delete(pRecvQueue);
	}
}

void ServerThread::run()
{
	sock_item_t tempSock;

	while(GAME_EXIT != _game_state)
	{
        struct sockaddr_in clientAddr;
        clientAddr.sin_addr.s_addr = 0x0a000001;
        insertUserMap(clientAddr);
        #if 0
		tempSock.psock = new UdpServer();
		tempSock.psock->init(NULL, 6789);
		tempSock.psock->setSocketBlock();
		tempSock.data = (char *)malloc(MAXITEMLENSIZE);
		if(NULL == tempSock.data)
		{
			eprintf("malloc error!\n");
			break;
		}
		memset(tempSock.data, 0, MAXITEMLENSIZE);
		if(-1 != tempSock.psock->recvData(tempSock.data, MAXITEMLENSIZE))
		{
			//printf("recvfrom [%s]:%s\n", inet_ntoa(_pInsUdp->getClientAddr().sin_addr), tempBuf);
			if(!memcmp(heart_req.c_str(), tempSock.data, heart_req.length()))
			{	/* broadcast */
				insertUserMap(tempSock.psock->getClientAddr());
				if(NULL == tempSock.data)
				{
					free(tempSock.data);
				}
				if(NULL == tempSock.psock)
				{
					delete(tempSock.psock);
				}
			}
			else
			{	/* unicast */
				tempSock.len = strlen(tempSock.data);
				pRecvQueue->Queue_Put(&tempSock, sizeof(sock_item_t));
			}
		}
		#endif
		msleep(50);
	}
}

int ServerThread::insertUserMap(sockaddr_in clientAddr)
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
