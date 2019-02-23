#ifndef SERVER_THREAD_H_
#define SERVER_THREAD_H_

extern Queue *pRecvQueue;

class ServerThread : public Thread
{
public:
	ServerThread();
	~ServerThread();
	int updateUserMap(sockaddr_in clientAddr);
protected:
	virtual void run();
	
private:
	UdpServer* _pInsUdp;
	uint32_t _buflen;
};


#endif /* SERVER_THREAD_H_ */