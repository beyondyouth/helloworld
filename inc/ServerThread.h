#ifndef SERVER_THREAD_H_
#define SERVER_THREAD_H_


class ServerThread : public Thread
{
public:
	ServerThread();
	~ServerThread();
	Queue *pRecvQueue = NULL;
protected:
	virtual void run();
	
private:
	UdpServer* _pInsUdp;
	uint32_t _buflen;
};


#endif /* SERVER_THREAD_H_ */