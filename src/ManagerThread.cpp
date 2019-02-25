#include <string>
#include <map>
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
#include "Display.h"
#include "UdpServer.h"
#include "UdpClient.h"
#include "ServerThread.h"
#include "ClientThread.h"
#include "UserThread.h"
#include "ManagerThread.h"

L_state _link_state = LINK_ERR;
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
	ServerThread InsServer;
	ClientThread InsClient;
    UserThread InsUser;

	InsServer.start();
	InsClient.start();
	InsUser.start();

	InsUser.wait();
	InsClient.wait();
	InsServer.wait();
}

