#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "Socket.h"
#include "UdpClient.h"


UdpClient::UdpClient()
{
}

UdpClient::~UdpClient()
{
}


bool UdpClient::init(const char* localIp, uint16_t localPort)
{
	return false;
}

bool UdpClient::init(uint16_t servPort)
{
	strcpy(_localIp,"");
	_localPort = 0;
	
	_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == _sockfd)
	{
		printf("error:%s %d\n",__FILE__, __LINE__);
		return false;
	}
	
	const int opt = 1;
	if(-1 == setsockopt(_sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt)))
	{
		printf("error:%s %d\n",__FILE__, __LINE__);
		return false;
	}

	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(servPort);
	_serverAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	bzero(&(_serverAddr.sin_zero), 8);

	return true;
}


bool UdpClient::setSocketBlock()
{
	fcntl(_sockfd, F_SETFL, fcntl(_sockfd, F_GETFL) & ~O_NONBLOCK);
	fcntl(_sockfd, F_SETFD, FD_CLOEXEC);
	return true;
}

bool UdpClient::setSocketNonblock()
{
	fcntl(_sockfd, F_SETFL, fcntl(_sockfd, F_GETFL) | O_NONBLOCK);
	return true;
}


bool UdpClient::disConn()
{
	//printf("Sockfd = %d,connecte status= %d, now disconnect socket....\n",_sockfd,_bConnected);
	shutdown(_sockfd, SHUT_RDWR);
	close(_sockfd);
	_sockfd = -1;

	return true;
}

bool UdpClient::readData(uint8_t *buf,uint32_t len)
{
	if(len > MAXDATASIZE)
		len = MAXDATASIZE;
	int nlen = sizeof(_serverAddr);
	if(-1 == recvfrom(_sockfd, buf, len, 0, (sockaddr*)&_serverAddr, (socklen_t*)&nlen))
	{
		printf("error:%s %d\n",__FILE__, __LINE__);
		return false;
	}
	return true;
}

bool UdpClient::writeData(const uint8_t *buf, uint32_t len)
{
	if(len > MAXDATASIZE)
		len = MAXDATASIZE;
	int nlen = sizeof(_serverAddr);
	if(-1 == sendto(_sockfd, buf, len, 0, (sockaddr*)&_serverAddr, nlen))
	{
		printf("error:%s %d\n",__FILE__, __LINE__);
		return false;
	}
	return true;
}

bool UdpClient::sendto2(const char* serverIp, uint16_t serverPort, const uint8_t* buf, uint32_t len)
{
	struct sockaddr_in serverAddr;
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sockfd)
	{
		printf("error:%s %d\n",__FILE__, __LINE__);
		return false;
	}
	
	const int opt = 1;
	if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt)))
	{
		printf("error:%s %d\n",__FILE__, __LINE__);
		//return false;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	serverAddr.sin_addr.s_addr = inet_addr(serverIp);
	bzero(&(serverAddr.sin_zero), 8);

	int nlen = sizeof(serverAddr);
	if(-1 == sendto(sockfd, buf, len, 0, (sockaddr*)&serverAddr, nlen))
	{
		printf("error:%s %d\n",__FILE__, __LINE__);
		//return false;
	}
	shutdown(sockfd, SHUT_RDWR);
	close(sockfd);
	
	return true;
}