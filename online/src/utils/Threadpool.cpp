 ///
 /// @file    Threadpool.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-24 21:38:45
 ///
#include "Threadpool.h"
#include "Thread.h"
#include <stdio.h>

namespace wd
{
Threadpool::Threadpool(size_t threadsNum,size_t bufNum)
:_threadsNum(threadsNum)
,_buf(bufNum)
,_isExit(false)
{printf("Threadpool()\n");}
Threadpool::~Threadpool()
{
	if(!_isExit)
		stop();
}

void Threadpool::start()
{
	for(size_t idx=0;idx!=_threadsNum;++idx)
	{
		//创建线程，然后存放到容器中
		Thread *pthread=new Thread(std::bind(&Threadpool::threadFunc,this));
		_vecThreads.push_back(pThread);
	}
	std::vector<Thread *>::iterator it;
	for(it=_vecThreads.begin();it!=_vecThreads.end();++it)
	{
		(*it)->start();
	}
	printf("Threadpool::start()\n");
}
void Threadpool::stop()
{
	if(!_isExit)
	{
		_isExit=true;
		_buf.set_flag(false);
		_buf.wakeup_empty();

		std::vector<Thread *>::iterator it;
		for(it=_vecThreads.begin();it!=_vecThreads.end();++it)
		{
			(*it)->join();
			delete (*it);
		}
		_vecThreads.clear();
	}
}
void Threadpool::addTask(Task task)
{
	printf("Threadpool::addTask()\n");
	_buf.push(task);
}
Threadpool::Task Threadpool::getTask()//从缓冲区中拿任务
{
	return _buf.pop();
}

void Threadpool::threadFunc()
{
	while(!_isExit)
	{
		Task task=_buf.pop();
		if(task)
		{
			task();
		}
	}
}
}
