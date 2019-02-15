#ifndef RECV_THREAD_H_
#define RECV_THREAD_H_


class RecvThread : public Thread
{
public:
	RecvThread();
	
protected:
	virtual void run();
	
private:
	Socket* _pSock;
	uint32_t _buflen;
};

int pushRxQuene(char* buf, uint32_t len, uint32_t offset = 0);
int pullRxQuene(char* buf, uint32_t len, uint32_t offset = 0);

#endif /* RECV_THREAD_H_ */