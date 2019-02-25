#ifndef CLIENT_THREAD_H_
#define CLIENT_THREAD_H_


extern Queue *pSendQueue;

class ClientThread : public Thread
{
public:
	ClientThread();
	~ClientThread();
protected:
	virtual void run();
};


#endif /* CLIENT_THREAD_H_ */