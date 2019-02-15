#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include "Debug.h"
#include "Mutex.h"

#define MUTEX_SUCCESS 0

Mutex::Mutex()
{
	if (MUTEX_SUCCESS == pthread_mutex_init(&_mutexId, NULL))
	{
		return ;
	}
	eprintf(" Mutex Init Fail, errno = %d ",errno);
	return;
}

int Mutex::lock()
{
	if (MUTEX_SUCCESS == pthread_mutex_lock(&_mutexId))
	{
		return 0;
	}
	eprintf(" Mutex Lock Fail, errno = %d ",errno);
	return -1;
}

int Mutex::trylock()
{
	if (MUTEX_SUCCESS == pthread_mutex_trylock(&_mutexId))
	{
		return 0;
	}
	eprintf(" Mutex Trylock Fail, errno = %d ",errno);
	return -1;
}

int Mutex::unlock()
{
	if (MUTEX_SUCCESS == pthread_mutex_unlock(&_mutexId))
	{
		return 0;
	}
	eprintf(" Mutex Unlock Fail, errno = %d ",errno);
	return -1;
}

pthread_mutex_t Mutex::getId()
{
	return _mutexId;
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&_mutexId);
}

MutexLocker::MutexLocker(Mutex* mutex) : _mutex(NULL)
{
	if (!mutex)
	{
		eprintf(" Mutexlocker Creat Fail, errno = %d ",errno);
		return;
	}
	_mutex = mutex;
	_mutex->lock();
}

MutexLocker::~MutexLocker()
{
	if (!_mutex)
	{
		return;
	}
	_mutex->unlock();
}
