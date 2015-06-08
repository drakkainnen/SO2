#ifndef RUNNABLE_CLASS
#define RUNNABLE_CLASS
#include <pthread.h>
#include <iostream>

class Runnable
{
	pthread_mutex_t stopMutex;
	bool stop;

public:
	virtual ~Runnable()
	{
		pthread_mutex_destroy(&stopMutex);
	}

	Runnable()
	{
		pthread_mutex_init(&stopMutex, NULL); 
	}

	static void* starter(void* args)
	{
		return ((Runnable*)args)->run();
	}

	virtual void* run() = 0;

	void join(pthread_t& thread)
	{
		void* status;
		pthread_join(thread, &status);
		if(status == nullptr)
		{
			std::cout << "all fine\n";
		}
	}
	
	void stopThread()
	{
		pthread_mutex_lock(&stopMutex);
		stop = true;
		pthread_mutex_unlock(&stopMutex);
	}

	bool isStoped()
	{
		return stop;
	}
};

#endif // RUNNABLE_CLASS
