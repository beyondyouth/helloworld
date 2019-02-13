#ifndef UDP_CLIENT_H__
#define UDP_CLIENT_H__


class UdpClient : public Socket
{
public:
	UdpClient(void);
	virtual ~UdpClient(void);
public:
	virtual bool init(const char* localIp, uint16_t localPort);
	virtual bool readData(uint8_t *buf,uint32_t len);
	virtual bool writeData(const uint8_t *buf,uint32_t len);
	virtual bool setSocketBlock(void);
	virtual bool setSocketNonblock(void);
	bool init(uint16_t servPort);
	bool disConn(void);
	bool sendto2(const char* serverIp, uint16_t serverPort, const uint8_t* buf, uint32_t len);
private:

};

#endif/*UDP_CLIENT_H__*/