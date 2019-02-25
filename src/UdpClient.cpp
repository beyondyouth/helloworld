#include <string>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "Debug.h"
#include "Socket.h"
#include "UdpClient.h"


UdpClient::UdpClient()
{
}

UdpClient::~UdpClient()
{
}


int UdpClient::init(const char *serverIp, uint16_t serverPort)
{
	_serverPort = serverPort;
	_sockfd = socket(AF_INET, SOCK_DGRAM, 0); /*IPv4协议 数据报socket*/
	if (-1 == _sockfd)
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}
#if 0
	_clientAddr.sin_family = AF_INET;
	_clientAddr.sin_port = htons(5555);
	_clientAddr.sin_addr.s_addr = inet_addr("10.0.2.15");
 
	if(-1 == bind(_sockfd,(struct sockaddr *)&_clientAddr,sizeof(_clientAddr)))
	{
		close(_sockfd);
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}
#endif
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_serverPort);
	_serverAddr.sin_addr.s_addr = inet_addr(serverIp);
	//memset(&(_serverAddr.sin_zero), 0, 8);

	return 0;
}


int UdpClient::setSocketBlock()
{
	fcntl(_sockfd, F_SETFL, fcntl(_sockfd, F_GETFL) & ~O_NONBLOCK);
	fcntl(_sockfd, F_SETFD, FD_CLOEXEC);
	return 0;
}

int UdpClient::setSocketNonblock()
{
	fcntl(_sockfd, F_SETFL, fcntl(_sockfd, F_GETFL) | O_NONBLOCK);
	return 0;
}


int UdpClient::exit()
{
	//dprintf("Sockfd = %d,connecte status= %d, now disconnect socket....\n",_sockfd,_bConnected);
	shutdown(_sockfd, SHUT_RDWR);
	close(_sockfd);
	_sockfd = -1;

	return 0;
}


sockaddr_in UdpClient::getClientAddr()
{
	return _clientAddr;
}


sockaddr_in UdpClient::getServerAddr()
{
	return _serverAddr;
}


int UdpClient::recvData(char *buf,uint32_t len)
{
	if(len > MAXITEMLENSIZE)
	{
		len = MAXITEMLENSIZE;
	}
	int nlen = sizeof(_serverAddr);
	if(-1 == recvfrom(_sockfd, buf, len, 0, (sockaddr*)&_serverAddr, (socklen_t*)&nlen))
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}
	return 0;
}

int UdpClient::sendData(const char *buf, uint32_t len)
{
	if(len > MAXITEMLENSIZE)
	{
		len = MAXITEMLENSIZE;
	}
	int nlen = sizeof(_serverAddr);
	if(-1 == sendto(_sockfd, buf, len, 0, (sockaddr*)&_serverAddr, nlen))
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}
	return 0;
}

int UdpClient::sendBroadcast(uint16_t serverPort, const char* buf, uint32_t len)
{
	struct sockaddr_in serverAddr;
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockfd)
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}
	
	const int opt = 1;
	if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt)))
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	serverAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	memset(&(serverAddr.sin_zero), 0, 8);

	int nlen = sizeof(serverAddr);
	if(-1 == sendto(sockfd, buf, len, 0, (sockaddr*)&serverAddr, nlen))
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
	}
	shutdown(sockfd, SHUT_RDWR);
	close(sockfd);
	
	return 0;
}
