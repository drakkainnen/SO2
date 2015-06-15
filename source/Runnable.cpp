#include "Runnable.h"

using namespace std;

pthread_cond_t Runnable::pauseCond;
pthread_mutex_t Runnable::pauseMutex;
bool Runnable::pauseFlag = false;
int Runnable::threadNumber = 0;

Runnable::~Runnable()
{
	pthread_mutex_destroy(&stopMutex);
}

Runnable::Runnable(std::string descryptor)
{
	this->descryptor = descryptor + " " + to_string(++threadNumber);
	pthread_mutex_init(&stopMutex, NULL);

}

void Runnable::createThread(Runnable& runnable)
{	
	pthread_attr_t attr; //moze detach bylby lepszy
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&runnable.thread, &attr, runnable.starter, (void*)&runnable);
	pthread_attr_destroy(&attr);
}

void* Runnable::starter(void* args)
{
	return ((Runnable*)args)->run();
}

void Runnable::join(Runnable* object)
{
	void* status;
	pthread_join(object->thread, &status);
	if((long)status == 1)
	{
		std::cout << "joined " + object->descryptor + "\n";
	}
	else
	{
		std::cout << "already joined\n";
	}
}

bool Runnable::tryJoin(pthread_t& thread)
{
	void* status;
	pthread_tryjoin_np(thread, &status);
	return (long)status == 1 ? true : false;
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
	return stop;
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

std::string Runnable::getMessageAndClean()
{
	string result;
	if(message != "")
	{
		result = descryptor + message;
		message = "";
	}
	return move(result);
}

void Runnable::setMessage(std::string message)
{
	this->message = message;
}
