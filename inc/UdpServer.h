#ifndef UDP_SERVER_H_
#define UDP_SERVER_H_


class UdpServer : public Socket
{
public:
	UdpServer(void);
	virtual ~UdpServer(void);
public:
	virtual bool init(const char* localIp, uint16_t localPort);
	virtual bool readData(uint8_t *buf, uint32_t len);
	virtual bool writeData(const uint8_t *buf, uint32_t len);
	virtual bool setSocketBlock();
	virtual bool setSocketNonblock();
	bool init(uint16_t localPort);
	bool closeConn();
	
	bool acceptConn(uint16_t localPort);
	sockaddr_in getClientAddr(void);
private:
	struct sockaddr_in _serverAddr;
	struct sockaddr_in _clientAddr;
};


#endif /* UDP_SERVER_H_ */