 ///
 /// @file    Thread.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-24 22:49:57
 ///
#include "Thread.h"
#include <iostream>

namespace wd
{
Thread::Thread(ThreadCallback cb)
:_pthid(0)
,_isRunning(false)
,_cb(cb)
{
	std::cout<<"Thread()"<<std::endl;
}
void Thread::start()
{
	pthread_create(&_pthid,NULL,threadFunc,this);
	_isRunning=true;
}
void Thread::join()
{
	pthread_join(_pthid,NULL);
	_isRunning=false;
}
void *Thread::threadFunc(void *arg)
{
	Thread *pThread=static_cast<Thread *>(arg);
	if(pThread!=NULL)
	{
		pThread->_cb();
		return pThread;
	}
	return NULL;
}
Thread::~Thread()
{
	if(_isRunning)
	{
		pthread_detach(_pthid);
	}
	std::cout<<"~Thread()"<<std::endl;
}
