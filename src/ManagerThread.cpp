#include <string>
#include <pthread.h>
#include "Thread.h"
#include "UserThread.h"
#include "ManagerThread.h"


ManagerThread::ManagerThread()
{
}

ManagerThread::~ManagerThread()
{
}

void ManagerThread::run()
{
    UserThread InsUser;
	InsUser.start();
	InsUser.wait();
}

