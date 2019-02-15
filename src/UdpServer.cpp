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

int UdpServer::init(const char* localIp, uint16_t localPort)
{
	return -1;
}

int UdpServer::init(uint16_t localPort)
{
	_localPort = localPort;
	
	_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == _sockfd)
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}

	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(localPort);
	_serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(_serverAddr.sin_zero), 0, 8);
	
	_clientAddr.sin_family = AF_INET;  
    _clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);  
    _clientAddr.sin_port = htons(localPort);
	memset(&(_clientAddr.sin_zero), 0, 8);
	
	const int opt = 1;
	if(-1 == setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)))
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

int UdpServer::readData(char *buf,uint32_t len)
{
	if(len > MAXDATASIZE)
		len = MAXDATASIZE;
	int nlen = sizeof(sockaddr_in);
	if(-1 == recvfrom(_sockfd, buf, len, 0, (sockaddr*)&_clientAddr, (socklen_t*)&nlen))
	{
		eprintf("error:%s %d\n",__FILE__, __LINE__);
		return -1;
	}
	return 0;
}

int UdpServer::writeData(const char *buf,uint32_t len)
{
	if(len > MAXDATASIZE)
		len = MAXDATASIZE;
	if(-1 == sendto(_sockfd, buf, len, 0, (sockaddr*)&_clientAddr, sizeof(sockaddr_in)))
	{
		eprintf("error:%s %d\n", __FILE__, __LINE__);
		return -1;
	}
	return 0;
}

int UdpServer::closeConn()
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