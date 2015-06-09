#include "Runnable.h"

pthread_cond_t Runnable::pauseCond;
pthread_mutex_t Runnable::pauseMutex;
bool Runnable::pauseFlag = false;

Runnable::~Runnable()
{
	pthread_mutex_destroy(&stopMutex);
}

Runnable::Runnable()
{
	pthread_mutex_init(&stopMutex, NULL); 
}

void* Runnable::starter(void* args)
{
	return ((Runnable*)args)->run();
}

void Runnable::join(pthread_t& thread)
{
	void* status;
	pthread_join(thread, &status);
	if((long)status == 1)
	{
		std::cout << "all fine\n";
	}
	else
	{
		std::cout << "not killed yet\n";
	}
}

void Runnable::tryJoin(pthread_t& thread)
{
	void* status;
	pthread_tryjoin_np(thread, &status);
	if((long)status == 1)
	{
		std::cout << "all fine\n";
	}
	else
	{
		std::cout << "not killed yet\n";
	}
}

void Runnable::stopThread()
{
	reasume();
	pthread_mutex_lock(&stopMutex);
	stop = true;
	pthread_mutex_unlock(&stopMutex);
}

bool Runnable::isStoped()
{
	bool result;
	pthread_mutex_lock(&stopMutex);
	result  = stop;
	pthread_mutex_unlock(&stopMutex);
	return result;
}

void Runnable::pause()
{
	pthread_mutex_lock(&pauseMutex);
	pauseFlag = 1;
	pthread_mutex_unlock(&pauseMutex);	 
}

void Runnable::reasume()
{
	pthread_mutex_lock(&pauseMutex);
	pauseFlag = 0;
	pthread_cond_broadcast(&pauseCond);
	pthread_mutex_unlock(&pauseMutex);	 
}

void Runnable::checkAndSuspend()
{
	pthread_mutex_lock(&pauseMutex);
	while (pauseFlag == true)
	{
	   	pthread_cond_wait(&pauseCond, &pauseMutex);
	}
	pthread_mutex_unlock(&pauseMutex);
}
void Runnable::init()
{
	pthread_cond_init(&pauseCond, NULL);
	pthread_mutex_init(&pauseMutex, NULL);
}

void Runnable::destroy()
{
	pthread_cond_destroy(&pauseCond);
	pthread_mutex_destroy(&pauseMutex);
}
