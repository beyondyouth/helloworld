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
#include "UdpServer.h"

UdpServer::UdpServer()
{
}

UdpServer::~UdpServer()
{
}

int UdpServer::init(uint16_t serverPort)
{
	_serverPort = serverPort;
	_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if(-1 == _sockfd)
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}

	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_serverPort);
	_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(_serverAddr.sin_zero), 0, 8);

	int opt = 1;
	if(-1 == setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt))) /*set reuse socket*/
    {
        close(_sockfd);
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1; 
    }

	if(-1 == bind(_sockfd,(struct sockaddr *)&_serverAddr,sizeof(_serverAddr)))
	{
		close(_sockfd);
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}

	return 0;
}

int UdpServer::setSocketBlock()
{
	fcntl(_sockfd, F_SETFL, fcntl(_sockfd, F_GETFL) & ~O_NONBLOCK);
	fcntl(_sockfd, F_SETFD, FD_CLOEXEC);
	return 0;
}

int UdpServer::setSocketNonblock()
{
	fcntl(_sockfd, F_SETFL, fcntl(_sockfd, F_GETFL) | O_NONBLOCK);
	return 0;
}

int UdpServer::recvData(char *buf,uint32_t len)
{
	if(len > MAXITEMLENSIZE)
	{
		len = MAXITEMLENSIZE;
	}
	int nlen = sizeof(sockaddr_in);
	if(-1 == recvfrom(_sockfd, buf, len, 0, (sockaddr*)&_clientAddr, (socklen_t*)&nlen))
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}
	return 0;
}

int UdpServer::sendData(const char *buf,uint32_t len)
{
	if(len > MAXITEMLENSIZE)
	{
		len = MAXITEMLENSIZE;
	}
	if(-1 == sendto(_sockfd, buf, len, 0, (sockaddr*)&_clientAddr, sizeof(sockaddr_in)))
	{
		eprintf("error:%s %d\n", __FILE__, __LINE__);
		return -1;
	}
	return 0;
}

int UdpServer::exit()
{
	shutdown(_sockfd, SHUT_RDWR);
	close(_sockfd);
	_sockfd = -1;
	return 0;
}

sockaddr_in UdpServer::getClientAddr()
{
	return _clientAddr;
}