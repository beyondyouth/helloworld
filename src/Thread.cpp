#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "Debug.h"
#include "Thread.h"



#define THREAD_SUCCES 0
#define THREADID_NO_INIT 0

Thread::Thread(const char* name)
{
	_threadId = THREADID_NO_INIT;
	_threadName = name;
	_isJoining = -1;
}

Thread::~Thread()
{
}

const char* Thread::getThreadName() const
{
	return _threadName.c_str();
}

int Thread::start()
{
	/*创建线程，函数入口点entryPoint*/
	sleep(1);
	if (THREAD_SUCCES == pthread_create(&_threadId, NULL, entryPoint, this))
	{
		return STATUS_OK;
	}
	eprintf(" Thread Create Fail, errno = %d\n", errno);
	return STATUS_ERROR;
}

int Thread::isRunning()
{
	if (THREADID_NO_INIT == _threadId)
		return -1;
	if(_isJoining)
		return 0;
	dprintf("pthread_kill(---%s\n", _threadName.c_str());
	/*判断线程是否存在*/
	int kill_rc = pthread_kill(_threadId, 0);
	if(ESRCH == kill_rc)
		return -1;
	else
		return 0;
}

int Thread::wait()
{
	if (THREADID_NO_INIT == _threadId)
	{
		return STATUS_OK;
	}
	_isJoining = 0;
//	dprintf("pthread_join...%s\n", _threadName.c_str());
	if (THREAD_SUCCES ==  pthread_join(_threadId, NULL))
	{
		_isJoining = -1;
		dprintf("pthread_join exited.\n");
		return STATUS_OK;
	}
	_isJoining = -1;
	dprintf(" Thread_join failed, errno = %d\n", errno);
	return STATUS_ERROR;
}

int Thread::exit()
{
	if (THREADID_NO_INIT == _threadId)
	{
		return STATUS_OK;
	}
	if (THREAD_SUCCES == pthread_kill(_threadId, SIGQUIT));
	{
		return STATUS_OK;
	}
	eprintf(" Thread kill failed, errno = %d\n", errno);
	return STATUS_ERROR;
}

int Thread::isStoped()
{
	if (THREADID_NO_INIT == _threadId)
		return 0;
	if(_isJoining)
		return -1;
	int pt = pthread_kill(_threadId, 0);
	if (THREAD_SUCCES == pt)
		return -1;
	else
	{
		if (ESRCH != pt)
			eprintf(" Thread Check Fail, errno = %d\n",errno);
		return 0;
	}
}

void Thread::msleep(unsigned int timeout)
{
	usleep(timeout * 1000);
}

void* Thread::entryPoint(void* ptr)
{
	Thread* pThread = (Thread*)ptr;
	/*调用run函数*/
	pThread->run();
	return (void*)0;
}

void Thread::destory(int sig)
{
	/*退出线程*/
	pthread_exit(0);
}
