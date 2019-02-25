#include <string>
#include <stdlib.h>
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


Queue *pSendQueue = NULL;

ClientThread::ClientThread()
{
	pSendQueue = new Queue(MAXQUEUELENGTH, sizeof(sock_item_t));
}

ClientThread::~ClientThread()
{
	if(NULL != pSendQueue)
	{
		delete(pSendQueue);
	}
}

void ClientThread::run()
{
	UdpClient pHeart;
	sock_item_t tempSock;
	
#if 1
	int ii = 0;
	while(GAME_EXIT != _game_state)
	{
		if(ii == 20)
		{
			pHeart.sendBroadcast(6789, heart_req.c_str(), heart_req.length()); /* fa song heart */
			ii = 0;
		}
		if(pSendQueue->Queue_Count())
		{
			pSendQueue->Queue_Get(&tempSock, sizeof(sock_item_t));
			if(NULL != tempSock.psock && NULL != tempSock.data)
			{
				tempSock.psock->sendData(tempSock.data, tempSock.len);
			}
			if(NULL != tempSock.data)
			{
				free(tempSock.data);
			}
			if(NULL != tempSock.psock)
			{
				delete(tempSock.psock);
			}
			memset(&tempSock, 0, sizeof(sock_item_t));
		}
		msleep(50);
		ii++;
		
	}
#endif
}