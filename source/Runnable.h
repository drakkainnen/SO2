#ifndef RUNNABLE_CLASS
#define RUNNABLE_CLASS
#include <pthread.h>
#include <iostream>
#include <string>

class Runnable
{
	bool stop;
	std::string message;
	std::string descryptor;

	static pthread_mutex_t pauseMutex;
	static pthread_cond_t pauseCond;
	static bool pauseFlag;

public:
	pthread_t thread;

	Runnable(std::string descryptor);
	virtual ~Runnable();

	static void init();
	static void destroy();

	static void* starter(void* args);
	virtual void* run() = 0;

	static void join(pthread_t& thread);
	static bool tryJoin(pthread_t& thread);	
	void stopThread();
	bool isStoped();

	static void pause();
	static void reasume();
	static void checkAndSuspend();

	std::string getMessageAndClean();
	void setMessage(std::string message);
};

#endif // RUNNABLE_CLASS
