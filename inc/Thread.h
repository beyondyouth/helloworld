#ifndef THREAD_H_
#define THREAD_H_


#define STATUS_OK 0
#define STATUS_ERROR -1

class Thread
{
public:
	Thread(const char* name = "unknow");
	virtual ~Thread();
	const char* getThreadName() const;

	int start();
	int exit();
	int wait();
	bool isStoped();
	bool isRunning();
	void msleep(unsigned int timeout);

protected:
	virtual void run() = 0;

private:
	static void* entryPoint(void* ptr);
	static void destory(int sig);

private:
	pthread_t _threadId;
	std::string _threadName;
	bool _isJoining;
};

#endif /* THREAD_H_ */
