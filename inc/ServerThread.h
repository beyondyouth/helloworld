#ifndef SERVER_THREAD_H_
#define SERVER_THREAD_H_

extern Queue *pRecvQueue;

class ServerThread : public Thread
{
public:
	ServerThread();
	~ServerThread();
	int insertUserMap(sockaddr_in clientAddr);
protected:
	virtual void run();
};


#endif /* SERVER_THREAD_H_ */
