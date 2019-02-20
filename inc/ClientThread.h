#ifndef CLIENT_THREAD_H_
#define CLIENT_THREAD_H_


class ClientThread : public Thread
{
public:
	ClientThread();
	~ClientThread();
	Queue *pSendQueue = NULL;
protected:
	virtual void run();
	
private:
	UdpClient* _pInsUdp;
	uint32_t _buflen;
};


#endif /* CLIENT_THREAD_H_ */