#include <stdio.h>
#include <string>
#include <pthread.h>
#include "Thread.h"
#include "ManagerThread.h"

int main()
{
	ManagerThread InsManager;
	InsManager.start();
	InsManager.wait();
	return 0;
}
