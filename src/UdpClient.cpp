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


int UdpClient::init(const char* localIp, uint16_t localPort)
{
	return -1;
}

int UdpClient::init(uint16_t servPort)
{
	memset(_localIp, 0, sizeof(_localIp));
	_localPort = 0;
	
	_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == _sockfd)
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}
	
	const int opt = 1;
	if(-1 == setsockopt(_sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt)))
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}

	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(servPort);
	_serverAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	memset(&(_serverAddr.sin_zero), 0, 8);

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


int UdpClient::disConn()
{
	//dprintf("Sockfd = %d,connecte status= %d, now disconnect socket....\n",_sockfd,_bConnected);
	shutdown(_sockfd, SHUT_RDWR);
	close(_sockfd);
	_sockfd = -1;

	return 0;
}

int UdpClient::readData(char *buf,uint32_t len)
{
	if(len > MAXDATASIZE)
		len = MAXDATASIZE;
	int nlen = sizeof(_serverAddr);
	if(-1 == recvfrom(_sockfd, buf, len, 0, (sockaddr*)&_serverAddr, (socklen_t*)&nlen))
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}
	return 0;
}

int UdpClient::writeData(const char *buf, uint32_t len)
{
	if(len > MAXDATASIZE)
		len = MAXDATASIZE;
	int nlen = sizeof(_serverAddr);
	if(-1 == sendto(_sockfd, buf, len, 0, (sockaddr*)&_serverAddr, nlen))
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}
	return 0;
}

int UdpClient::sendto2(const char* serverIp, uint16_t serverPort, const uint8_t* buf, uint32_t len)
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
		//return -1;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	serverAddr.sin_addr.s_addr = inet_addr(serverIp);
	memset(&(serverAddr.sin_zero), 0, 8);

	int nlen = sizeof(serverAddr);
	if(-1 == sendto(sockfd, buf, len, 0, (sockaddr*)&serverAddr, nlen))
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		//return -1;
	}
	shutdown(sockfd, SHUT_RDWR);
	close(sockfd);
	
	return 0;
}