#include <string>
#include <stdint.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "Mutex.h"
#include "Thread.h"
#include "Socket.h"
#include "Queue.h"
#include "Common.h"
#include "RecvThread.h"
#include "SendThread.h"
#include "UserThread.h"
#include "ManagerThread.h"

L_state _link_state = LINK_FAILED;
G_state _game_state = GAME_EXIT;

ManagerThread::ManagerThread()
{
}

ManagerThread::~ManagerThread()
{
}

void ManagerThread::run()
{
	_game_state = GAME_START;
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

