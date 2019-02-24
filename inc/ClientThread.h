#ifndef CLIENT_THREAD_H_
#define CLIENT_THREAD_H_


extern Queue *pSendQueue;
extern UdpClient* _pInsUdp;

class ClientThread : public Thread
{
public:
	ClientThread();
	~ClientThread();
protected:
	virtual void run();
	
private:
	uint32_t _buflen;
};


#endif /* CLIENT_THREAD_H_ */