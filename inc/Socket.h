#ifndef SOCKET_H_
#define SOCKET_H_


#define MAXITEMLENSIZE 256
#define MAXQUEUELENGTH 1024


class Socket
{
public:
	virtual ~Socket(){}
	virtual int init(const char *serverIp, uint16_t serverPort) = 0;
	virtual int recvData(char *buf, uint32_t len) = 0;
	virtual int sendData(const char *buf, uint32_t len) = 0;
	virtual int setSocketBlock(void) = 0;
	virtual int setSocketNonblock(void) = 0;
	virtual int exit(void) = 0;
	virtual sockaddr_in getClientAddr(void) = 0;
	virtual sockaddr_in getServerAddr(void) = 0;
protected:			
	int _sockfd;

	char _serverIp[32];
	uint16_t _serverPort;
	
	struct sockaddr_in _clientAddr;
	struct sockaddr_in _serverAddr;
};

typedef struct
{
	Socket *psock;
	char *data;
	int len;
}sock_item_t;

const std::string fight_req("fight req");
const std::string fight_rsp("fight rsp");
const std::string fight_rej("fight rej");
const std::string heart_req("heart req");

#endif/*SOCKET_H_*/