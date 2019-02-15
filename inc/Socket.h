#ifndef SOCKET_H_
#define SOCKET_H_


#define MAXDATASIZE 256

class Socket
{
public:
//	Socket(void);
//	virtual ~Socket(void);
public:
	virtual int init(const char* localIp, uint16_t localPort) = 0;

	virtual int readData(char *buf, uint32_t bufLen) = 0;
	virtual int writeData(const char *buf, uint32_t dataLen) = 0;
	virtual int setSocketBlock() = 0;
	virtual int setSocketNonblock() = 0;
//	virtual void closes();
protected:			
	int _sockfd;

	char _localIp[32];
	uint16_t _localPort;
	
	struct sockaddr_in _clientAddr;
	struct sockaddr_in _serverAddr;
};

#endif/*SOCKET_H_*/