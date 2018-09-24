 ///
 /// @file    Thread.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-24 22:50:06
 ///
#ifndef _WD_THREAD_H
#define _WD_THREAD_H

#include "Noncopyable.h"
#include <pthread.h>
#include <functional>

namespace wd
{
class Thread:Noncopyable
{
public:
	typedef std::function<void()> ThreadCallback;
	Thread(ThreadCallback cb);
	~Thread();

	void start();
	void join();
	static void *threadFunc(void *arg);
private:
	pthead_t	_pthid;
	bool		_isRunning;
	ThreadCallback _cb;
};
}
#endif
