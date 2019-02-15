#ifndef UDP_SERVER_H_
#define UDP_SERVER_H_


class UdpServer : public Socket
{
public:
	UdpServer(void);
	virtual ~UdpServer(void);
public:
	virtual int init(const char* localIp, uint16_t localPort);
	virtual int readData(char *buf, uint32_t len);
	virtual int writeData(const char *buf, uint32_t len);
	virtual int setSocketBlock();
	virtual int setSocketNonblock();
	int init(uint16_t localPort);
	int closeConn();
	
	int acceptConn(uint16_t localPort);
	sockaddr_in getClientAddr(void);
private:
	struct sockaddr_in _serverAddr;
	struct sockaddr_in _clientAddr;
};


#endif /* UDP_SERVER_H_ */