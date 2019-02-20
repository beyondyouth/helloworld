#ifndef UDP_SERVER_H_
#define UDP_SERVER_H_


class UdpServer : public Socket
{
public:
	UdpServer(void);
	virtual ~UdpServer(void);
public:
	virtual int init(uint16_t serverPort);
	virtual int recvData(char *buf, uint32_t len);
	virtual int sendData(const char *buf, uint32_t len);
	virtual int setSocketBlock(void);
	virtual int setSocketNonblock(void);
	virtual int exit(void);
	
	sockaddr_in getClientAddr(void);
	int check_ip(void);
private:
	struct sockaddr_in _serverAddr;
	struct sockaddr_in _clientAddr;
};


#endif /* UDP_SERVER_H_ */