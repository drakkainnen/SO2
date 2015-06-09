#ifndef RUNNABLE_CLASS
#define RUNNABLE_CLASS
#include <pthread.h>
#include <iostream>

class Runnable
{
	pthread_mutex_t stopMutex;
	bool stop;

	static pthread_mutex_t pauseMutex;
	static pthread_cond_t pauseCond;
	static bool pauseFlag;

public:
	virtual ~Runnable();
	Runnable();

	static void init();
	static void destroy();

	static void* starter(void* args);
	virtual void* run() = 0;

	static void join(pthread_t& thread);
	static void tryJoin(pthread_t& thread);
	
	void stopThread();
	bool isStoped();

	static void pause();
	static void reasume();
	static void checkAndSuspend();
};

#endif // RUNNABLE_CLASS
