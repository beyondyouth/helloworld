#ifndef MANAGER_THREAD_H_
#define MANAGER_THREAD_H_


class ManagerThread : public Thread
{
public:
	ManagerThread();
	~ManagerThread();
//	friend void setAdvance();
protected:
	virtual void run();
private:
	
};

#endif /* MANAGER_THREAD_H_ */