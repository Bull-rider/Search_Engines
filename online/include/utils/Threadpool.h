 ///
 /// @file    Threadpool.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-24 21:02:36
 ///
#ifndef _WD_THREADPOOL_H
#define _WD_THREADPOOL_H

#include "Buffer.h"
#include <vector>
#include <functional>

namespace wd
{
class Thread;
class Threadpool
{
public:
	typedef std::function<void()> Task;
	Threadpool(size_t threadsNum,size_t bufNum);
	~Threadpool();
	void start();
	void stop();
	void addTask(Task task);//向线程池中添加任务
private:
	void threadFunc();
	Task getTask();//从缓冲区取任务
private:
	size_t	_threadsNum;//线程数量
	std::vector<Thread *> _vecThreads;//线程对象容器
	Buffer	_buf;//缓冲区对象
	bool	_isExit;
};
}
#endif
