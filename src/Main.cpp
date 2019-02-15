#include <string>
#include <stdio.h>
#include <syslog.h>
#include <pthread.h>
#include "Debug.h"
#include "Thread.h"
#include "ManagerThread.h"

int main()
{
	ManagerThread InsManager;
	InsManager.start();
	InsManager.wait();
	return 0;
}
