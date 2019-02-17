#ifndef RECV_THREAD_H_
#define RECV_THREAD_H_


class RecvThread : public Thread
{
public:
	RecvThread();
	~RecvThread();
	Queue *pRecvQueue = NULL;
protected:
	virtual void run();
	
private:
	Socket* _pSock;
	uint32_t _buflen;
};


#endif /* RECV_THREAD_H_ */