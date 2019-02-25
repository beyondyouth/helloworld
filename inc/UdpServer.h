#ifndef UDP_SERVER_H_
#define UDP_SERVER_H_


class UdpServer : public Socket
{
public:
	UdpServer(void);
	~UdpServer(void);
public:
	virtual int init(const char *serverIp, uint16_t serverPort);
	virtual int recvData(char *buf, uint32_t len);
	virtual int sendData(const char *buf, uint32_t len);
	virtual int setSocketBlock(void);
	virtual int setSocketNonblock(void);
	virtual int exit(void);
	virtual sockaddr_in getClientAddr(void);
	virtual sockaddr_in getServerAddr(void);

	int check_ip(void);
private:
};


#endif /* UDP_SERVER_H_ */