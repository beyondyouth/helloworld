#ifndef UDP_CLIENT_H_
#define UDP_CLIENT_H_


class UdpClient : public Socket
{
public:
	UdpClient(void);
	virtual ~UdpClient(void);
public:
	virtual int init(const char* localIp, uint16_t localPort);
	virtual int readData(char *buf,uint32_t len);
	virtual int writeData(const char *buf,uint32_t len);
	virtual int setSocketBlock(void);
	virtual int setSocketNonblock(void);
	int init(uint16_t servPort);
	int disConn(void);
	int sendto2(const char* serverIp, uint16_t serverPort, const uint8_t* buf, uint32_t len);
private:

};

#endif /* UDP_CLIENT_H_ */