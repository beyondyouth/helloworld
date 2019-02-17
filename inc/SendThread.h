#ifndef SEND_THREAD_H_
#define SEND_THREAD_H_


class SendThread : public Thread
{
public:
	SendThread();

protected:
	virtual void run();
	
private:
	Socket* _pSock;
	uint32_t _buflen;
};

int pushTxQueue(char* buf, uint32_t len, uint32_t offset = 0);
int pullTxQueue(char* buf, uint32_t len, uint32_t offset = 0);

#endif /* SEND_THREAD_H_ */