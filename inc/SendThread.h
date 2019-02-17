#ifndef SEND_THREAD_H_
#define SEND_THREAD_H_


class SendThread : public Thread
{
public:
	SendThread();
	~SendThread();
	Queue *pSendQueue = NULL;
protected:
	virtual void run();
	
private:
	Socket* _pSock;
	uint32_t _buflen;
};


#endif /* SEND_THREAD_H_ */