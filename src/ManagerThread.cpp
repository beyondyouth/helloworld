#include <string>
#include <stdint.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "Thread.h"
#include "Socket.h"
#include "RecvThread.h"
#include "SendThread.h"
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
	RecvThread InsRecv;
	SendThread InsSend;
    UserThread InsUser;

	InsRecv.start();
	InsSend.start();
	InsUser.start();

	InsUser.wait();
	InsSend.wait();
	InsRecv.wait();
}

