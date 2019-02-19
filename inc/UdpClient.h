#ifndef UDP_CLIENT_H_
#define UDP_CLIENT_H_


class UdpClient : public Socket
{
public:
	UdpClient(void);
	virtual ~UdpClient(void);
public:
	virtual int init(uint16_t servPort);
	virtual int recvData(char *buf,uint32_t len);
	virtual int sendData(const char *buf,uint32_t len);
	virtual int setSocketBlock(void);
	virtual int setSocketNonblock(void);
	virtual int exit(void);
	int sendBroadcast(uint16_t serverPort, const char* buf, uint32_t len);
private:

};

#endif /* UDP_CLIENT_H_ */