#ifndef MANAGER_THREAD_H__
#define MANAGER_THREAD_H__


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

#endif /*MONITOR_THREAD_H__*/